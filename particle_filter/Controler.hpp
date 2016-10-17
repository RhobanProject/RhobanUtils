#ifndef CONTROLER_HPP
#define CONTROLER_HPP

#include "Particle.hpp"

//A Control class can control only one type of Particles
template <class P>
class Controler {
private:
  /* The probability distribution used to generate random Moves */
  Density * explorationField;

public:
  Controler() : explorationField(NULL){};
  virtual ~Controler(){};

  virtual void setExplorationField(Density * newExplorationField){
    explorationField = newExplorationField;
  };

  /* Apply the predicted move over elapsedTime to the specified particle */
  virtual void move(P & p, double elapsedTime) {
    (void) p; (void) elapsedTime;// Avoiding warnings
  };

  /* Apply an exploration move over elapsedTime to the specified particle */
  virtual void explore(P & p, double elapsedTime){
    if (explorationField == NULL){
      return;
    }
    //TODO check if random_sample implies random_generate
    Matrix explorationMove = elapsedTime * explorationField->random_sample();
    p.setFromVector(p.toVector() + explorationMove );
  }

};

#endif//CONTROLER_HPP
