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
    float xtl_ens[4]; //local copy for cross-talk correction
    for (int xtl = 0; xtl<4; ++xtl) {
      xtl_ens[xtl] = 0.0;
    }

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
      if (conf.Suppress == true) {
        if (hits[i].Suppress == true) { continue; }
      }
      if (hits[i].PileUp == 1 ) {
        nPileUp += 1;
        continue;
      }
      if (hits[i].OutOfRange == 1) { continue; }
      if (hits[i].CFDForce == 1) { continue; }

      if (hits[i].Energy > conf.EnThresh && hits[i].RawEnergy < conf.RawUpThresh) {
        xtl_ens[hits[i].CrystalID] = hits[i].Energy;
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

            gam->Energy = 0;
            for (int hit_i=0; hit_i<gam->nHits; ++hit_i) {
              if (xtl_ens[hits[gam->HitInds[hit_i]].CrystalID] > 0.0) {
                for (int xtlj=0; xtlj<4; ++xtlj) {
                  int xtl = hits[gam->HitInds[hit_i]].CrystalID;
                  if (xtl == xtlj) { continue; }
                  hits[gam->HitInds[hit_i]].Energy += conf.ct_cal[CloverID][xtl][xtlj]*xtl_ens[xtlj];
                }
                gam->Energy += hits[gam->HitInds[hit_i]].Energy;
              }
            }
            
            //new gamma
            gammas[nGammas].Set(hits[i], i);
            for (int xtl = 0; xtl<4; ++xtl) {
              xtl_ens[xtl] = 0.0;
            }
            xtl_ens[hits[i].CrystalID] = hits[i].Energy;
            ++nGammas;
            gam = &(gammas[nGammas-1]);
          }
        }
      }
    }
    //final gamma finished,

    if (nGammas > 0) {
      gam->Energy = 0;
      for (int hit_i=0; hit_i<gam->nHits; ++hit_i) {
        if (xtl_ens[hits[gam->HitInds[hit_i]].CrystalID] > 0.0) {
          for (int xtlj=0; xtlj<4; ++xtlj) {
            int xtl = hits[gam->HitInds[hit_i]].CrystalID;
            if (xtl == xtlj) { continue; }
            hits[gam->HitInds[hit_i]].Energy += conf.ct_cal[CloverID][xtl][xtlj]*xtl_ens[xtlj];
          }
          gam->Energy += hits[gam->HitInds[hit_i]].Energy;
        }                
      }
    }
  }
}
