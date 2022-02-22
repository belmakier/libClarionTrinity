#ifndef CLARION_TRINITY_GAGG_HH
#define CLARION_TRINITY_GAGG_HH

#include <libpixie/measurement.hh>

namespace ClarionTrinity {
  class TrinityConf;

  class GAGG {
  public:
    int GaggID;
    
    int crate1;
    int crate2;
    int slot1;
    int slot2;
    int chan1;
    int chan2;

    unsigned long long int time1;
    int peak1;
    int tail1;
    int energy1;
    int tau1;
    int fired1;
    int traceLength1;
    int iMeas1;
    int background1;
    int postbackground1;
    int pu1;
    int oor1;

    unsigned long long int time2;
    int peak2;
    int tail2;
    int energy2;
    int tau2;
    int fired2;
    int traceLength2;
    int iMeas2;
    int background2;
    int postbackground2;
    int pu2;
    int oor2;

    float peak2_cal;
    float tail2_cal;
    float energy2_cal;    

    unsigned long long int time;
    float peak;
    float tail;
    float energy;
    int tau;
    int clean;

    GAGG() {GaggID = -1; fired1 = 0; fired2 = 0; clean = 0; };
    //void Set(PIXIE::Event e, int cr1, int sl1, int ch1, int cr2, int sl2, int ch2);
    void Set1(const PIXIE::Measurement &meas, const TrinityConf &conf);
    void Set2(const PIXIE::Measurement &meas, const TrinityConf &conf);
    void Set(const TrinityConf &conf);
    void Reset(int ID) {GaggID = ID, fired1 = 0; fired2 = 0; clean = 0; } 
  };
    
    
}

#endif
