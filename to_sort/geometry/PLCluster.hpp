#ifndef PL_CLUSTER_HPP
#define PL_CLUSTER_HPP

#include "ParametricLine.hpp"

class PLCluster {
private:
  std::vector<ParametricLine> lines;
  ParametricLine average;

public:
  PLCluster(const ParametricLine & l): average(l) {lines.push_back(l);}

  unsigned int size() const {return lines.size();}

  const ParametricLine & getAverage() const {return average;}

  bool accept(const ParametricLine & candidate,
              float rhoTol, float thetaTol) const;

  void push(const ParametricLine & l);

  /* Return true if the two ParametricLines are similar according to the parameter
   * used.
   */
  static bool similar(const ParametricLine & l1, const ParametricLine & l2,
                      float rhoTol, float thetaTol);
  
};

/**
 * Add the given line to the clusters, if the line doesn't match any of
 * the clusters, then create a new Cluster
 * if: maxCluster != -1 and clusters.size() >= maxCluster and l do not fit in
 * any cluster, then l is not added to clusters
 */
void addToClusters(const ParametricLine & l,
                   std::vector<PLCluster> & clusters,
                   float rhoTol, float thetaTol,
                   int maxCluster = -1);

#endif//PL_CLUSTER_HPP
