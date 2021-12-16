#include "GAGG.hh"
#include "Trinity.hh"

namespace ClarionTrinity {
  void GAGG::Set1(const PIXIE::Measurement &meas) {
    crate1 = meas.crateID;
    slot1 = meas.slotID;
    chan1 = meas.channelNumber;
    fired1 = 1;
    //energy1 = meas.trace_meas[0].datum;
    //peak1 = meas.trace_meas[1].datum;
    //tail1 = meas.trace_meas[2].datum;
    //background1 = meas.trace_meas[3].datum;
    //postbackground1 = meas.trace_meas[4].datum;

    energy1 = (meas.QDCSums[2] + meas.QDCSums[3] + meas.QDCSums[4] + meas.QDCSums[5] + meas.QDCSums[6]) - (meas.QDCSums[0] + meas.QDCSums[1])*115./60.;
    peak1 = meas.QDCSums[3] - (meas.QDCSums[0] + meas.QDCSums[1])*20./60.;
    tail1 = meas.QDCSums[5] - (meas.QDCSums[0] + meas.QDCSums[1])*55./60.;
    background1 = meas.QDCSums[0] + meas.QDCSums[1];
    postbackground1 = meas.QDCSums[7];
    time1 = meas.eventTime;
    traceLength1 = meas.traceLength;
    //ffTrig1 = meas.trace_meas[5].datum;
    pu1 = meas.finishCode;
  }

  void GAGG::Set2(const PIXIE::Measurement &meas) {
    crate2 = meas.crateID;
    slot2 = meas.slotID;
    chan2 = meas.channelNumber;
    fired2 = 1;
    //energy2 = meas.trace_meas[0].datum;
    //peak2 = meas.trace_meas[1].datum;
    //tail2 = meas.trace_meas[2].datum;
    //tau1 = meas.trace_meas[3];
    //background2 = meas.trace_meas[3].datum;
    //postbackground2 = meas.trace_meas[4].datum;

    energy2 = (meas.QDCSums[2] + meas.QDCSums[3] + meas.QDCSums[4] + meas.QDCSums[5] + meas.QDCSums[6]) - (meas.QDCSums[0] + meas.QDCSums[1])*115./60.;
    peak2 = meas.QDCSums[3] - (meas.QDCSums[0] + meas.QDCSums[1])*20./60.;
    tail2 = meas.QDCSums[5] - (meas.QDCSums[0] + meas.QDCSums[1])*55./60.;
    background2 = meas.QDCSums[0] + meas.QDCSums[1];
    postbackground2 = meas.QDCSums[7];

    time2 = meas.eventTime;
    traceLength2 = meas.traceLength;
    //ffTrig2 = meas.trace_meas[5].datum;
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
