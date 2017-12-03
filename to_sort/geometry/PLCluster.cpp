#include "PLCluster.hpp"

#include <cmath>

bool PLCluster::accept(const ParametricLine & candidate,
                       float rhoTol, float thetaTol) const
{
  return similar(getAverage(), candidate, rhoTol, thetaTol);
}

void PLCluster::push(const ParametricLine & l)
{
  // Updating average
  int k = size();
  average = ParametricLine::weightedAverage(average, l, k, 1);
  lines.push_back(l);
}

bool PLCluster::similar(const ParametricLine & l1, const ParametricLine & l2,
                        float rhoTol, float thetaTol)
{
  //TODO: handle case where rho ~= 0 and thetaDiff ~= 180
  double rhoDiff = std::fabs(l1.getRho() - l2.getRho());
  double thetaDiff = std::fabs((l1.getTheta() - l2.getTheta()).getSignedValue());
  return rhoDiff < rhoTol && thetaDiff < thetaTol;
}

void addToClusters(const ParametricLine & l,
                   std::vector<PLCluster> & clusters,
                   float rhoTol, float thetaTol, int maxCluster)
{
  bool accepted = false;
  // Insert in a cluster if possible
  for (unsigned int i = 0; i < clusters.size(); i++)
  {
    if (clusters[i].accept(l, rhoTol, thetaTol)) {
      accepted = true;
      clusters[i].push(l);
      break;
    }
  }
  // If no cluster matches, create a new one
  if (!accepted && (unsigned int) maxCluster > clusters.size()) {
    clusters.push_back(PLCluster(l));
  }
}
