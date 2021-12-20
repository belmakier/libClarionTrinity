#include "GAGG.hh"
#include "Trinity.hh"

namespace ClarionTrinity {
  void GAGG::Set1(const PIXIE::Measurement &meas, const TrinityConf &conf) {
    crate1 = meas.crateID;
    slot1 = meas.slotID;
    chan1 = meas.channelNumber;
    fired1 = 1;
    if (conf.pidType == PIDType::kTrace) {
      energy1 = meas.trace_meas[0].datum;
      peak1 = meas.trace_meas[1].datum;
      tail1 = meas.trace_meas[2].datum;
      background1 = meas.trace_meas[3].datum;
      postbackground1 = meas.trace_meas[4].datum;
    }
    else if (conf.pidType == PIDType::kQDC) {
      background1 = 0;
      energy1 = 0;
      peak1 = 0;
      tail1 = 0;
      postbackground1 = 0;
      for (int i=0; i<8; ++i) {
        if (conf.QDCType[i] == Window::kEnergy) {
          energy1 += meas.QDCSums[i];
        }
        else if (conf.QDCType[i] == Window::kPeak) {
          peak1 += meas.QDCSums[i];
        }
        else if (conf.QDCType[i] == Window::kBackground) {
          background1 += meas.QDCSums[i];
        }
        else if (conf.QDCType[i] == Window::kTail) {
          tail1 += meas.QDCSums[i];
        }
        else if (conf.QDCType[i] == Window::kPostBackground) {
          postbackground1 += meas.QDCSums[i];
        }
      }
      energy1 = energy1 - conf.energyWidth/conf.backWidth * background1;
      peak1 = peak1 - conf.peakWidth/conf.backWidth * background1;
      tail1 = tail1 - conf.tailWidth/conf.backWidth * background1;            
    }
    time1 = meas.eventTime;
    traceLength1 = meas.traceLength;
    pu1 = meas.finishCode;
  }

  void GAGG::Set2(const PIXIE::Measurement &meas, const TrinityConf &conf) {
    crate2 = meas.crateID;
    slot2 = meas.slotID;
    chan2 = meas.channelNumber;
    fired2 = 1;
    if (conf.pidType == PIDType::kTrace) {
      energy2 = meas.trace_meas[0].datum;
      peak2 = meas.trace_meas[1].datum;
      tail2 = meas.trace_meas[2].datum;
      background2 = meas.trace_meas[3].datum;
      postbackground2 = meas.trace_meas[4].datum;
    }
    else if (conf.pidType == PIDType::kQDC) {
      background2 = 0;
      energy2 = 0;
      peak2 = 0;
      tail2 = 0;
      postbackground2 = 0;
      for (int i=0; i<8; ++i) {
        if (conf.QDCType[i] == Window::kEnergy) {
          energy2 += meas.QDCSums[i];
        }
        else if (conf.QDCType[i] == Window::kPeak) {
          peak2 += meas.QDCSums[i];
        }
        else if (conf.QDCType[i] == Window::kBackground) {
          background2 += meas.QDCSums[i];
        }
        else if (conf.QDCType[i] == Window::kTail) {
          tail2 += meas.QDCSums[i];
        }
        else if (conf.QDCType[i] == Window::kPostBackground) {
          postbackground2 += meas.QDCSums[i];
        }
      }
      energy2 = energy2 - conf.energyWidth/conf.backWidth * background2;
      peak2 = peak2 - conf.peakWidth/conf.backWidth * background2;
      tail2 = tail2 - conf.tailWidth/conf.backWidth * background2;            
    }
    time2 = meas.eventTime;
    traceLength2 = meas.traceLength;
    pu2 = meas.finishCode;
  }

  void GAGG::Set(const TrinityConf &conf) {
    float gain = conf.gain[GaggID];
    peak2_cal = (float)peak2*gain;
    tail2_cal = (float)tail2*gain;
    energy2_cal = (float)energy2*gain;
    if (fired1 && fired2) {
      time = (time1 + time2)/2;
      peak = ((float)peak1 + peak2_cal)/2.0;
      tail = ((float)tail1 + tail2_cal)/2.0;
      energy = ((float)energy1 + energy2_cal)/2.0;
      tau = (tau1 + tau2)/2.0;
      valid = 1;
      //reject based on board PU detection
      if (conf.rejectPU) {
        if (pu1 == 1 || energy1 == 0) {
          valid = 0;
        }
        if (pu2 == 1 || energy2 == 0) {
          valid = 0;
        }
      }

      //reject based on energy, peak, tail, or background conditions
      if ((energy1 + energy2) == 0) {
        valid = 0;
      } 
      else if ((peak1 + peak2) == 0) {
        valid = 0;
      } 
      else if ((tail1 + tail2) == 0) {
        valid = 0;
      } 
      else if (std::abs((float)(energy1 - energy2)/(float)(energy1 + energy2)) > conf.energySimilarity) {
        valid = 0; 
      }
      else if (std::abs((float)(peak1 - peak2)/(float)(peak1 + peak2)) > conf.peakSimilarity) {
        valid = 0;
      }  
      else if (std::abs((float)(tail1 - tail2)/(float)(tail1 + tail2 + 2000) ) > conf.tailSimilarity) {
        valid = 0;
      }  

      if (std::abs((float)(background1-postbackground1)/(float)(background1)) > conf.backConsistency) {
        valid = 0;
      }
      if (std::abs((float)(background2-postbackground2)/(float)(background2)) > conf.backConsistency) {
        valid = 0;
      }
    }
    else if (fired1 && !fired2) {
      time = time1;
      peak = peak1;
      tail = tail1;
      energy = energy1;
      tau = tau1;
      valid = 0; 
    }
    else if (!fired1 && fired2) {
      time = time2;
      peak = peak2;
      tail = tail2;
      energy = energy2;
      tau = tau2;
      valid = 0; 
    }    
  }
}
