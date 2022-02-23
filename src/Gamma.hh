#ifndef CLARION_TRINITY_GAMMA_HH
#define CLARION_TRINITY_GAMMA_HH

#include <vector>

#include "ClarionHit.hh"
#include "Definitions.hh"

namespace ClarionTrinity {
  class Gamma {
  public:
    double Energy;
    double MaxEn;
    int MaxEnInd;

    unsigned long long int timestamp;
    unsigned long long int ctimestamp;
    
    //std::vector<int> HitInds;
    int HitInds[MAX_HITS_PER_CLOVER];
    int nHits;
    
    double Theta;
    double Phi;

  public:
    Gamma() {};
    Gamma(const ClarionHit &hit, int ind);
    void Set(const ClarionHit &hit, int ind);
  };
}

#endif
