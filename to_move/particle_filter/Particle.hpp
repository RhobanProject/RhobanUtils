#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <Eigen/Core>

// Each particle can be considered as a matrix composed of the value of each
// dimension
// Heritage should be private, because we don't want the user to be allowed to
// modify the internal values "by hand", but these means adding some accessors

template<unsigned int D>
class Particle{
protected:
  Eigen::VectorXd values;
public:

  Particle() : values(D) {
    for (unsigned int k = 0; k < D; k++){
      values(k) = 0.0;
    }
  }

  virtual ~Particle() {};

  void setFromVector(const Eigen::VectorXd & v){
    if (v.rows() != D) {
      throw std::runtime_error("Trying to set Particle with a vector of inappropriate dimension");
    }
    values = v;
  }

  Eigen::VectorXd toVector() const{
    return values;
  }

  static unsigned int getDim(){
    return D;
  }

  virtual Particle<D> * clone() const = 0;
};
#endif//PARTICLE_HPP
