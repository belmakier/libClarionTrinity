#include "Clover.hh"
#include "Clarion.hh"

namespace ClarionTrinity {
  void Clover::MakeGammas(const ClarionConf &conf) {
    //gammas.clear();
    nGammas = 0;
    mult = 0;
    Gamma *gam;
    for (int i=0; i<nHits; ++i) {
      long long int hitt = hits[i].timestamp; 
      for (int j=0; j<nBGOs; ++j) {
        long long int bgot = bgos[j].timestamp;
        double tdiff = (hitt-bgot)/3276.8; //ns
        if (abs(tdiff) < conf.BGOVetoTime && bgos[j].RawEnergy > 10) {
          hits[i].Suppress = true;
        }
      }   

      //probably increment some counter here
      if (hits[i].Suppress == true) { continue; }
      if (hits[i].PileUp == 1 ) { continue; }
      if (hits[i].OutOfRange == 1) { continue; }
      if (hits[i].CFDForce == 1) { continue; }

      if (hits[i].Energy > conf.EnThresh) {
        if (mult == 0) { //if the first hit to get this far, make gamma based off it
          gammas[mult].Set(hits[i], i);
          ++mult;
          ++nGammas;
          gam = &(gammas[mult-1]);
        }
        else {
          double tdiff = (double)abs((long long int)gam->timestamp - (long long int)hits[i].timestamp)/3276.8;
          if (gam->timestamp == 0 || tdiff < conf.AddBackTDiff) {  // if inside the time window, 
            gam->Energy += hits[i].Energy;                         // add to existing gamma
            gam->HitInds[gam->nHits] = i;
            ++gam->nHits;
            if (hits[i].Energy > gam->MaxEn) {
              gam->MaxEn = hits[i].Energy;
              gam->MaxEnInd = i;
              gam->timestamp = hits[i].timestamp;
              gam->ctimestamp = hits[i].timestamp;
            }
          }
          else {
            if (mult >= MAX_GAMMAS) { 
              std::cout << "Warning! More than " << MAX_GAMMAS << " gammas" << std::endl;
              continue;
            }
            //new gamma
            gammas[mult].Set(hits[i], i);
            ++mult;
            ++nGammas;
            gam = &(gammas[mult-1]);
          }
        }
      }
    }    
  }
}
