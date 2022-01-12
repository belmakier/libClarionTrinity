#ifndef CLARION_TRINITY_CLOVER_HH
#define CLARION_TRINITY_CLOVER_HH

#include "ClarionHit.hh"
#include "Gamma.hh"
#include "Definitions.hh"

#include <libpixie/measurement.hh>

namespace ClarionTrinity {
  class ClarionConf;
  
  class Clover {
  public:
    int CloverID;

    int mult;
    /*
    std::vector<ClarionHit> hits;
    std::vector<ClarionHit> bgos;
    std::vector<ClarionHit> sidechans;
    std::vector<Gamma> gammas;
    */
    ClarionHit hits[MAX_HITS_PER_CLOVER];
    ClarionHit bgos[MAX_HITS_PER_CLOVER]; //does this need to be a vector?
    ClarionHit sidechans[MAX_HITS_PER_CLOVER];
    Gamma gammas[MAX_GAMMAS];

    int nHits;
    int nBGOs;
    int nSideChans;
    int nGammas;
    int nNonPrompt;
    int nPileUp;
    int nSuppress;

    Clover() : CloverID(-1) {};
    void Reset(int id) {
      nHits = 0;
      nBGOs = 0;
      nSideChans = 0;
      nGammas = 0;
      nNonPrompt = 0;
      nPileUp = 0;
      nSuppress = 0;
      CloverID = id;      
    }
    void AddHit(const PIXIE::Measurement &meas, int cryst, const ClarionConf &conf) {
      if (nHits >= MAX_HITS_PER_CLOVER) {
        std::cout << "Error! More than " << MAX_HITS_PER_CLOVER << " hits in Clover ID " << CloverID << std::endl;
        return;
      }
      if (cryst <= 3) {
        hits[nHits].Set(meas, CloverID, cryst, conf);
        ++nHits;
        //hits.emplace_back(meas, CloverID, cryst, conf);
      }
      else if (4 <= cryst && cryst <= 6) {
        sidechans[nSideChans].Set(meas, CloverID, cryst, conf);
        ++nSideChans;
        //sidechans.emplace_back(meas, CloverID, cryst, conf);
      }
      else {
        bgos[nBGOs].Set(meas, CloverID, cryst, conf);
        ++nBGOs;
        //bgos.emplace_back(meas, CloverID, cryst, conf);
      }        
    }
    void MakeGammas(const ClarionConf &conf);
  };
}

#endif
