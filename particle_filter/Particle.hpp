#ifndef PARTICLE_HPP
#define PARTICLE_HPP

// Each particle can be considered as a matrix composed of the value of each
// dimension
// Heritage should be private, because we don't want the user to be allowed to
// modify the internal values "by hand", but these means adding some accessors
#include "linear_algebra.h"

template<unsigned int D>
class Particle{
protected:
  Matrix values;
public:

  Particle() : values(D) {
    for (unsigned int k = 0; k < D; k++){
      values.set(k, 0, 0.0);
    }
  }

  virtual ~Particle() {};

  void setFromVector(Matrix m){
    values.clear();
    for (unsigned int i = 0; i < m.size(); i++)
      values.push_back(m[i]);
    values.lig = m.lig;
    values.col = m.col;
  }

  Matrix toVector() const{
    return values;
  }

  static unsigned int getDim(){
    return D;
  }

  virtual Particle<D> * clone() const = 0;
};
#endif//PARTICLE_HPP
