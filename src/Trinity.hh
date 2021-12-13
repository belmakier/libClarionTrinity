#ifndef CLARION_TRINITY_TRINITY_HH
#define CLARION_TRINITY_TRINITY_HH

#include "GAGG.hh"
#include "Definitions.hh"

namespace ClarionTrinity {
  class TrinityConf {
  public: 
    int nCrystals; //number of crystals
    int GaggID[MAX_GAGG_CRYSTALS]; //don't know how big this should be: crystal ID's
    int Cr1[MAX_GAGG_CRYSTALS]; //crate IDs
    int Cr2[MAX_GAGG_CRYSTALS];
    int Sl1[MAX_GAGG_CRYSTALS]; //slot IDs
    int Sl2[MAX_GAGG_CRYSTALS];
    int Ch1[MAX_GAGG_CRYSTALS]; //PIXIE channel IDs
    int Ch2[MAX_GAGG_CRYSTALS];

    int GaggIDMap[CT_MAX_CRATES][CT_MAX_SLOTS_PER_CRATE][CT_MAX_CHANNELS_PER_BOARD];
    int GaggSensorMap[CT_MAX_CRATES][CT_MAX_SLOTS_PER_CRATE][CT_MAX_CHANNELS_PER_BOARD];    

    float gain[MAX_GAGG_ID]; //this is indexed by GaggID

    float theta[MAX_GAGG_ID];
    float phi[MAX_GAGG_ID];
    
    float energySimilarity;
    float peakSimilarity;
    float tailSimilarity;
    float backConsistency;
    bool rejectPU;

    TrinityConf() : energySimilarity(1e6), peakSimilarity(1e6), tailSimilarity(1e6), backConsistency(1e6) {}
    TrinityConf(std::string conffile);

    void Print();
  };

  class Trinity {
  public: 
    TrinityConf conf;
    GAGG parts[MAX_GAGG_CRYSTALS];
    //GAGG valid_parts[MAX_GAGG_CRYSTALS];
    int nParts;
    //int nValidParts;

    Trinity() {};
    Trinity(std::string conffile) : conf(conffile) { };

    void PrintConf() { conf.Print(); }
  };
}


#endif
