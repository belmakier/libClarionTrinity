#include <fstream>

#include "Trinity.hh"

namespace ClarionTrinity {
  TrinityConf::TrinityConf(std::string conffile) : 
    energySimilarity(1e6), 
    peakSimilarity(1e6), 
    tailSimilarity(1e6),
    backConsistency(1e6),
    rejectPU(true),
    rejectOOR(true) {
    for (int i=0; i<CT_MAX_CRATES; ++i) {
      for (int j=0; j<CT_MAX_SLOTS_PER_CRATE; ++j) {
        for (int k=0; k<CT_MAX_CHANNELS_PER_BOARD; ++k) {
          GaggIDMap[i][j][k] = -1;
          GaggSensorMap[i][j][k] = -1;
        }
      }
    }
    
    std::cout << conffile << std::endl;
    std::ifstream file(conffile.c_str());
    if (!file.is_open()) {
      std::cout << "Warning! " << conffile << " not open" << std::endl;
    }
    std::string line;

    nCrystals = 0;
    std::getline(file, line);
    std::cout << line << std::endl;
    while (std::getline(file, line)) {
      std::cout << line << std::endl; 
      if (line.size() == 0) { continue; }
      if (line[0] == '#') { continue; }
      if (line[0] == ';') { continue; }

      std::stringstream ss(line);

      ss >> GaggID[nCrystals] >> Cr1[nCrystals] >> Sl1[nCrystals] >> Ch1[nCrystals] >>
        Cr2[nCrystals] >> Sl2[nCrystals] >> Ch2[nCrystals] >> gain[GaggID[nCrystals]];
      GaggIDMap[Cr1[nCrystals]][Sl1[nCrystals]][Ch1[nCrystals]] = GaggID[nCrystals];
      GaggIDMap[Cr2[nCrystals]][Sl2[nCrystals]][Ch2[nCrystals]] = GaggID[nCrystals];
      GaggSensorMap[Cr1[nCrystals]][Sl1[nCrystals]][Ch1[nCrystals]] = 1;
      GaggSensorMap[Cr2[nCrystals]][Sl2[nCrystals]][Ch2[nCrystals]] = 2;
      nCrystals += 1;
    }

    for (int i=0; i<MAX_GAGG_ID; ++i) {
      pidType[i] = PIDType::kTrace; //trace by default
    }
  }

  int TrinityConf::ReadAngleMap(std::string mapfile) {
    std::cout << mapfile << std::endl;
    std::ifstream file(mapfile.c_str());
    if (!file.is_open()) {
      std::cout << "Warning! " << mapfile << " not open" << std::endl;
      return -1;
    }
    std::string line;

    int n = 0;
    while (std::getline(file, line)) {
      if (line.size() == 0) { continue; }
      if (line[0] == '#') { continue; }
      if (line[0] == ';') { continue; }

      std::stringstream ss(line);

      int ring, xtl;
      ss >> ring >> xtl;
      ss >> theta[ring*100 + xtl] >> phi[ring*100 + xtl];
      ++n;
    }
    return n;
  }

  int TrinityConf::ReadQDCParams(std::string paramfile) {
    std::cout << paramfile << std::endl;
    std::ifstream file(paramfile.c_str());
    if (!file.is_open()) {
      std::cout << "Warning! " << paramfile << " not open" << std::endl;
      return -1;
    }
    std::string line;

    int n = 0;
    while (std::getline(file, line)) {
      if (line.size() == 0) { continue; }
      if (line[0] == '#') { continue; }
      if (line[0] == ';') { continue; }

      std::stringstream ss(line);

      int i;
      float w;
      std::string t;
      ss >> i >> w >> t;
      QDCWidth[i] = w;
      if (t == "E") {
        QDCType[i] = Window::kEnergy;
        energyWidth += w;
        ++n;
      }
      else if (t == "P") {
        QDCType[i] = Window::kPeak;
        energyWidth += w;
        peakWidth += w;
        ++n;
      }
      else if (t == "T") {
        QDCType[i] = Window::kTail;
        energyWidth += w;
        tailWidth += w;
        ++n;
      }
      else if (t == "B") {
        QDCType[i] = Window::kBackground;
        backWidth += w;
        ++n;
      }
      else if (t == "PB") {
        QDCType[i] = Window::kPostBackground;
        ++n;
      }
      else if (t == "O") {
        QDCType[i] = Window::kOther;
        ++n;
      }
      else {
        std::cout << "Warning - invalid QDC type" << std::endl;
      }      
    }
    return n;
  }

  
  void TrinityConf::Print() {
    for (int i=0; i<nCrystals; ++i) {
      printf("%i    %i.%i.%i     %i.%i.%i   %5.4f    %5.4f\n", GaggID[i], Cr1[i],Sl1[i],Ch1[i], Cr2[i],Sl2[i],Ch2[i], theta[GaggID[i]], phi[GaggID[i]]);
    }
  }
}
