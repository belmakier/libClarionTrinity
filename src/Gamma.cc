#include "Gamma.hh"
#include "ClarionHit.hh"

namespace ClarionTrinity {
  Gamma::Gamma(const ClarionHit &hit, int ind) {
    Energy = hit.Energy;
    MaxEn = hit.Energy;
    HitInds[0] = ind;
    MaxEnInd = ind;
    timestamp = hit.timestamp;
    ctimestamp = hit.timestamp;
    nHits = 1;
  }
  void Gamma::Set(const ClarionHit &hit, int ind) {
    Energy = hit.Energy;
    MaxEn = hit.Energy;
    HitInds[0] = ind;
    MaxEnInd = ind;
    timestamp = hit.timestamp;
    ctimestamp = hit.timestamp;
    nHits = 1;
  }
}
