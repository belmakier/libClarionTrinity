#include "Clover.hh"
#include "Clarion.hh"

namespace ClarionTrinity {
  void Clover::MakeGammas(const ClarionConf &conf) {
    //gammas.clear();
    nGammas = 0;
    nPileUp = 0;
    nNonPrompt = 0;
    nSuppress = 0;
    Gamma *gam;
    for (int i=0; i<nHits; ++i) {
      long long int hitt = hits[i].timestamp; 
      for (int j=0; j<nBGOs; ++j) {
        long long int bgot = bgos[j].timestamp;
        double tdiff = (hitt-bgot)/3276.8; //ns
        if (abs(tdiff) < conf.BGOVetoTime) {
          if (bgos[j].PileUp == 1) {
            hits[i].Suppress = true;
            nSuppress += 1;
            continue;
          }
          if (bgos[j].RawEnergy > conf.RawBGOThresh && bgos[j].RawEnergy < conf.RawUpThresh) {          
            hits[i].Suppress = true;
            nSuppress += 1;
          }
        }
      }    

      //probably increment some counter here
      if (hits[i].Suppress == true) { continue; }
      if (hits[i].PileUp == 1 ) {
        nPileUp += 1;
        continue;
      }
      if (hits[i].OutOfRange == 1) { continue; }
      if (hits[i].CFDForce == 1) { continue; }

      if (hits[i].Energy > conf.EnThresh && hits[i].RawEnergy < conf.RawUpThresh) {
        if (nGammas == 0) { //if the first hit to get this far, make gamma based off it
          gammas[nGammas].Set(hits[i], i);
          ++nGammas;
          gam = &(gammas[nGammas-1]);
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
            nNonPrompt += 1;
            if (nGammas >= MAX_GAMMAS) { 
              std::cout << "Warning! More than " << MAX_GAMMAS << " gammas" << std::endl;
              continue;
            }
            //old gamma is finished, apply correction
            if (gam->nHits == 2) {
              int id0 = hits[gam->HitInds[0]].CrystalID;
              int id1 = hits[gam->HitInds[1]].CrystalID;
              double gain = conf.gain_ab[CloverID][id0][id1];
              double off = conf.offset_ab[CloverID][id0][id1]; 
              gam->Energy = off + gain*gam->Energy;
            }
            
            //new gamma
            gammas[nGammas].Set(hits[i], i);
            ++nGammas;
            gam = &(gammas[nGammas-1]);
          }
        }
      }
    }
    //final gamma finished,
    if (nGammas > 0) {
      if (gam->nHits == 2) {
        int id0 = hits[gam->HitInds[0]].CrystalID;
        int id1 = hits[gam->HitInds[1]].CrystalID;
        double gain = conf.gain_ab[CloverID][id0][id1];
        double off = conf.offset_ab[CloverID][id0][id1]; 
        gam->Energy = off + gain*gam->Energy;
      }
    }
  }
}
