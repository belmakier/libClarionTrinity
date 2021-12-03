#include "Event.hh"

#include <libpixie/event.hh>
#include <libpixie/reader.hh>

namespace ClarionTrinity {
  const PIXIE::Event& Event::Set(const PIXIE::Reader &reader, int eventInd) {
    const PIXIE::Event &e = reader.events[eventInd];
    trinity.nParts = 0;
    clarion.nClovers = 0;
    
    for (int i=0; i<e.nMeas; ++i) {
      const PIXIE::Measurement &meas = reader.measurements[e.fMeasurements[i]];
      ////////////////////////
      // TRINITY PROCESSING //
      ////////////////////////

      //for each measurement, check if it's part of Trinity
      int GaggID = trinity.conf.GaggIDMap[meas.crateID][meas.slotID][meas.channelNumber];
      int Sensor = trinity.conf.GaggSensorMap[meas.crateID][meas.slotID][meas.channelNumber];
      
      if (GaggID != -1) {
        int exists = 0;
        for (int j=0; j<trinity.nParts; ++j) { //iterate through existing GAGG objects
          if (trinity.parts[j].GaggID == GaggID) {
            if (Sensor == 1) {
              trinity.parts[j].Set1(meas);
	      trinity.parts[j].iMeas1 = i;
              exists = 1;
              break; //found it
            }
            else if (Sensor == 2) {
              trinity.parts[j].Set2(meas);
	      trinity.parts[j].iMeas2 = i;
              exists = 1;
              break; //found it
            }
          }
        }
        if (exists == 0) { //make new
          if (trinity.nParts == MAX_GAGG_CRYSTALS) { std::cout << "WARNING, too many GaGG crystals in a single event, increase MAX_GAGG_CRYSTALS?" << std::endl;
            continue;
          }
          trinity.parts[trinity.nParts].Reset(GaggID);
          ++trinity.nParts;
          if (Sensor == 1) {
            trinity.parts[trinity.nParts-1].Set1(meas);
	    trinity.parts[trinity.nParts-1].iMeas1 = i;
          }
          else if (Sensor == 2) {
            trinity.parts[trinity.nParts-1].Set2(meas);
	    trinity.parts[trinity.nParts-1].iMeas2 = i;
          }
        }
      }

      ////////////////////////
      // CLARION PROCESSING //
      ////////////////////////
      //for each measurement, check if it's part of Clover
      int cloverID = clarion.conf.CloverIDMap[meas.crateID][meas.slotID][meas.channelNumber];
      int crystal = clarion.conf.CrystalMap[meas.crateID][meas.slotID][meas.channelNumber];
      if (cloverID != -1) {            
        int exists = 0;
        for (int k=0; k<clarion.nClovers; ++k) { //iterate through existing Clovers
          if (clarion.clovers[k].CloverID == cloverID) {
            clarion.clovers[k].AddHit(meas, crystal, clarion.conf);
            exists = 1;
            break; //found it
          }
        }
        if (exists == 0) { //make new
          if (clarion.nClovers == MAX_CLOVERS) { std::cout << "WARNING, too many Clovers in a single event, increase MAX_CLOVERS?" << std::endl;
            continue;
          }
          clarion.clovers[clarion.nClovers].Reset(cloverID);
          ++clarion.nClovers;
          clarion.clovers[clarion.nClovers-1].AddHit(meas, crystal, clarion.conf);
        }
      }
    }
    //measurements finished processing

    //validate GAGGs with both SiPM fired
    for (int j=0; j<trinity.nParts; ++j) {
      trinity.parts[j].Set(trinity.conf);
      // if (trinity.parts[j].valid) {
      //   trinity.valid_parts[trinity.nValidParts].push_back(trinity.parts[j]);
      // }
    }

    //Process Hits -> Gammas (addback) here
    for (int j=0; j<clarion.nClovers; ++j) {
      clarion.clovers[j].MakeGammas(clarion.conf);
    }

    return e;
  }  
}
