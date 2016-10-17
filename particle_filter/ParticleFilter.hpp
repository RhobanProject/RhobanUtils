#ifndef PARTICLE_FILTER_HPP
#define PARTICLE_FILTER_HPP

#include <algorithm>

#include "linear_algebra.h"

#include "Particle.hpp"
#include "Controler.hpp"
#include "Observation.hpp"

#include "timing/Benchmark.hpp"

// A real ParticleFilter concerns one particle type
// P must be a Particle Type
// TODO: force bounded genericity:
// http://stackoverflow.com/questions/6803100/achieving-bounded-genericity-in-c
template<class P>
class ParticleFilter {
  protected:
    vector<std::pair<P,double>> particles;

    P representativeParticle;

    double representativeQuality;

    /**
     * Use an observation to update the score of the particles. A weight of x
     * corresponds to using the observation x times, it is possible to use only
     * a part of the particles for this observation, in this case, the ratio
     * of particles to use is specified in the range [0,1] (0 = no particles,
     * 1 = every particle)
     */
    virtual void apply(const Observation<P> & obs,
                       double weight = 1.0, double ratioUsed = 1.0) {
        if (ratioUsed != 1.0) {
            std::random_shuffle(particles.begin(), particles.end());
        }
        unsigned int nbParticlesUsed = ratioUsed * particles.size();
        double totalScore = 0;
        for (unsigned int i = 0; i < nbParticlesUsed; i++) {
            double score = obs.potential(particles[i].first);
            particles[i].second *= pow(score, weight);
            totalScore += score;
        }
        double avgScore = totalScore / nbParticlesUsed;
        for (unsigned int i = nbParticlesUsed; i < particles.size(); i++) {
            particles[i].second *= pow(avgScore, weight);
        }
    }

    /* Resample the particles vector, using the given observation to attribute
       a probability to each particle */
    virtual void select(){
        double sum = 0.0;

        vector<double> particles_probas(particles.size());


        // std::cout<<"DEBUG: NB PARTICLES "<<particles.size()<<std::endl;

        for (int i=0; i<(int) particles.size(); i++) {
            double li = particles[i].second;
            particles_probas[i] = li + sum;
            sum += li;
        }
        if (sum <= 0.0) {
            std::ostringstream oss;
            oss << "Sum of scores in particle filter is: " << sum << std::endl;
            throw std::runtime_error(oss.str());
        }

        // Preparing the new vector of particles
        vector<P> newParticles(particles.size());
        for (unsigned int i = 0; i < particles.size(); i++) {
            double rand_val = uniform_rand(0.0, sum);
            // Finding k with a dichotomic method
            int start = 0;
            int end = particles.size();
            while (start != end){
                int pivot = (end + start) / 2;
                if (particles_probas[pivot] < rand_val){
                    start = pivot + 1;
                }
                else{
                    end = pivot;
                }
            }
            newParticles[i] = particles[start].first;
        }
        // Replacing old Particles
        for (int i = 0; i < (int) particles.size(); i++) {
            particles[i].first = newParticles[i];
            particles[i].second = 1;
        }
    }

    virtual void updateRepresentativeQuality(){
        if (particles.size() == 0) return;
        //TODO implement a default calcul
        representativeQuality = 1.0;
    }

    virtual void updateRepresentativeParticle(){
        if (particles.size() == 0) return;

        int N = particles.size();
        Matrix M = particles[0].first.toVector();
        for (int i = 1; i < N; i++) {
            M = M + particles[i].first.toVector();
        }
        M = (1.0 / (double) N) * M;
        representativeParticle.setFromVector(M);
    }

    /* Update representativeParticle and then representativeParticle */
    virtual void updateInternalValues(){
        updateRepresentativeParticle();
        updateRepresentativeQuality();
    }

  public:

    ParticleFilter<P>() : representativeQuality(0) {};

    virtual ~ParticleFilter() { clear(); };

    ParticleFilter<P>(const ParticleFilter & other){
        representativeParticle = other.representativeParticle.clone();
        representativeQuality = other.representativeQuality;
        for(int p=0; p < (int)other.particles.size(); p++){
            particles.push_back((P)other.particles[p]->clone() );
        }
    }

    void initializeAtUniformRandom(double minVal,
                                   double maxVal,
                                   int particleNb){
        Matrix center(P::getDim());
        for (unsigned int k = 0; k < P::getDim(); k++){
            center[k] = 0.0;
        }
        initializeAtUniformRandom(minVal, maxVal, particleNb, center);
    }

    void initializeAtUniformRandom(double minVal,
                                   double maxVal,
                                   int particleNb,
                                   Matrix center){
        clear();
        for(int k = 0; k < particleNb; k++) {
            P p;
            Matrix m = center + random_matrix(P::getDim(), 1, minVal, maxVal);
            p.setFromVector(m);
            particles.push_back(std::pair<P, double>(p, 1.0));
        }
    }

    void partialUniformResampling(double resampledRatio,
                                  double * minValues,
                                  double * maxValues) {
        int particleNb = particles.size() * resampledRatio;
        for(int k = 0; k < particleNb; k++) {
            P p;
            Matrix m = random_matrix(P::getDim(), 1, 0, 1);
            for (unsigned int dim = 0; dim < P::getDim(); dim++) {
                double min = minValues[dim];
                double max = maxValues[dim];
                double delta = max - min;
                m.set(dim, 0, min + delta * m.get(dim, 0));
            }
            p.setFromVector(m);
            particles[k] = std::pair<P, double>(p, 1.0);
        }
    }

    // Allows to initialize the particle
    void initializeAtUniformRandom(double * minValues,
                                   double * maxValues,
                                   int particleNb) {
        clear();
        for(int k = 0; k < particleNb; k++) {
            P p;
            Matrix m = random_matrix(P::getDim(), 1, 0, 1);
            for (unsigned int dim = 0; dim < P::getDim(); dim++) {
                double min = minValues[dim];
                double max = maxValues[dim];
                double delta = max - min;
                m.set(dim, 0, min + delta * m.get(dim, 0));
            }
            p.setFromVector(m);
            particles.push_back(std::pair<P, double>(p, 1.0));
        }
    }

    //TODO: ideal cycle would be :
    //      1 - Controlled Move
    //      2 - Selection
    //      3 - Exploration
    /* Mutation involves controled movement and alea exploration */
    virtual void mutate(Controler<P> & ctrl, double elapsedTime){
        if (particles.size() == 0){
            return;
        }

        for (unsigned int i = 0; i < particles.size(); i++){
            ctrl.move(particles[i].first, elapsedTime);
            ctrl.explore(particles[i].first, elapsedTime);
        }
    }

    //TODO: factorization of the step code (using the same function)
    /* If no observation is given, don't compute selection */
    virtual void step(Controler<P> & ctrl,
                      double elapsedTime){
        mutate(ctrl, elapsedTime);
        updateInternalValues();
    }

    /* Perform a full step of the ParticleFilter, usually a mutation followed
       by a selection */
    virtual void step(Controler<P> & ctrl,
                      const Observation<P> & obs,
                      double elapsedTime){
        mutate(ctrl, elapsedTime);
        apply(obs);
        select();
        updateInternalValues();
    }

    /**
     * Perform a full step of the ParticleFilter applying a vector of observations
     */
    virtual void step(Controler<P> & ctrl,
                      const std::vector<Observation<P> * > & observations,
                      double elapsedTime)
    {

        Utils::Timing::Benchmark::open("Mutation");
        mutate(ctrl, elapsedTime);
        Utils::Timing::Benchmark::close("Mutation");


        //We only select if we have new observations
        if(observations.size()>0)
        {
            Utils::Timing::Benchmark::open("Applying observations");
            /*
              double sum = 0.0;
              for (int i=0; i<(int) particles.size(); i++) {
              sum += particles[i].second;
              }
            */

            for (auto& o : observations)
            {
                apply(*o);
            }
            /*
              sum = 0.0;
              for (int i=0; i<(int) particles.size(); i++) {
              sum += particles[i].second;
              }
            */

            Utils::Timing::Benchmark::close("Applying observations");

            Utils::Timing::Benchmark::open("Selecting particles");
            select();
            Utils::Timing::Benchmark::close("Selecting particles");
        }

        Utils::Timing::Benchmark::open("Updating internal state");
        updateInternalValues();
        Utils::Timing::Benchmark::close("Updating internal state");
    }

    unsigned int nbParticles() const{
        return particles.size();
    }

    P getParticle(unsigned int index) const{
        return particles[index].first;
    }

    double getParticleQ(unsigned int index) const{
        return particles[index].second;
    }


    /* Usually, the given particle is the mean of all the particles, but
       this behavior might not fit to the model. */
    virtual P getRepresentativeParticle(){
        return representativeParticle;
    }
    /* Return a value in [0,1] specifying the quality of the representative,
       1 corresponds to a situation where no doubt is allowed and 0 to a
       situation where the information isn't worth anything. */
    virtual double getRepresentativeQuality(){
        return representativeQuality;
    }

    void clear(){
        particles.clear();
    }
};

#endif//PARTICLE_FILTER_HPP
