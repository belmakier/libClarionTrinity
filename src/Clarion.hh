#ifndef CLARION_TRINITY_CLARION_HH
#define CLARION_TRINITY_CLARION_HH

#include "Clover.hh"
#include "Definitions.hh"

namespace ClarionTrinity {
  class ClarionConf {
  public:
    int nClovers; //number of clovers
    int CloverID[MAX_CLOVERS * 4]; //don't know how big this should be: crystal ID's
    int Cr[MAX_CLOVERS][8]; //crate ID    - these are all indexed by 
    int Sl[MAX_CLOVERS][8]; //slot IDs    - 0-3: crystals, 4-6: side channels, 7 : BGO
    int Ch[MAX_CLOVERS][8]; //channel IDs

    int CloverIDMap[CT_MAX_CRATES][CT_MAX_SLOTS_PER_CRATE][CT_MAX_CHANNELS_PER_BOARD];
    int CrystalMap[CT_MAX_CRATES][CT_MAX_SLOTS_PER_CRATE][CT_MAX_CHANNELS_PER_BOARD];

    double offset[MAX_CLOVERS * 4][8];
    double gain[MAX_CLOVERS * 4][8];

    double theta[MAX_CLOVERS][5];
    double phi[MAX_CLOVERS][5];

    bool AddBack = true;
    double BGOVetoTime = 500; //in ns
    double EnThresh = 30; //keV
    double AddBackTDiff = 500; //in ns
    
    ClarionConf() {};
    ClarionConf(std::string conffile);

    void ReadCal(std::string calfile);
    void ReadAngleMap(std::string mapfile);

    void Print();
  };

  class Clarion {
  public:
    ClarionConf conf;
    
    //std::vector<Clover> clovers;
    int nClovers;
    Clover clovers[MAX_CLOVERS];

    Clarion() {};
    Clarion(std::string conffile) : conf(conffile) {};

    void ReadAngleMap(std::string mapfile) { conf.ReadAngleMap(mapfile); }
    void PrintConf() { conf.Print(); }
  };
}

#endif
