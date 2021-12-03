#ifndef CLARION_TRINITY_CLARIONHIT_HH
#define CLARION_TRINITY_CLARIONHIT_HH

#include <libpixie/measurement.hh>

namespace ClarionTrinity {
  class ClarionConf;
  
  class ClarionHit {
  public:
    int CloverID;
    int CrystalID;
    
    double RawEnergy;
    double Energy;

    unsigned long long int timestamp;

    int PileUp;
    int CFDForce;
    int OutOfRange;

    bool Suppress;

    ClarionHit() {};
    ClarionHit(const PIXIE::Measurement &meas, int id, int cryst, const ClarionConf &conf);
    void Set(const PIXIE::Measurement &meas, int id, int cryst, const ClarionConf &conf);
  };
}

#endif
