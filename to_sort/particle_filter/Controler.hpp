#ifndef CONTROLER_HPP
#define CONTROLER_HPP

#include "Particle.hpp"
#include "random/tools.h"

//A Control class can control only one type of Particles
template <class P>
class Controler {
public:
  Controler()
    {
      engine = getRandomEngine();
    }
  virtual ~Controler(){};

  /* Apply the predicted move over elapsedTime to the specified particle */
  virtual void move(P & p, double elapsedTime) {
    (void) p; (void) elapsedTime;// Avoiding warnings
  };

  /* Apply an exploration move over elapsedTime to the specified particle */
  virtual void explore(P & p, double elapsedTime){
    (void)p;
    (void)elapsedTime;
  }

protected:
  std::default_random_engine engine;

};

#endif//CONTROLER_HPP
