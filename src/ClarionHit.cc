#include "ClarionHit.hh"
#include "Clarion.hh"

namespace ClarionTrinity {
  ClarionHit::ClarionHit(const PIXIE::Measurement &meas, int id, int cryst, const ClarionConf &conf) : CloverID(id), CrystalID(cryst){
    RawEnergy = meas.eventEnergy;
    timestamp = meas.eventTime;
    PileUp = meas.finishCode;
    CFDForce = meas.CFDForce;
    OutOfRange = meas.outOfRange;
    Suppress = false;

    //calibration here?
    Energy = conf.gain[id][cryst]*(RawEnergy + conf.dither) + conf.offset[id][cryst];
  }

  void ClarionHit::Set(const PIXIE::Measurement &meas, int id, int cryst, const ClarionConf &conf) {
    CloverID = id;
    CrystalID = cryst;
    RawEnergy = meas.eventEnergy;
    timestamp = meas.eventTime;
    PileUp = meas.finishCode;
    CFDForce = meas.CFDForce;
    OutOfRange = meas.outOfRange;
    Suppress = false;

    Energy = conf.gain[id][cryst]*(RawEnergy + conf.dither) + conf.offset[id][cryst];
  }
}


