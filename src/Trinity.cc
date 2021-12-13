#include <fstream>

#include "Trinity.hh"

namespace ClarionTrinity {
  TrinityConf::TrinityConf(std::string conffile) : 
    energySimilarity(1e6), 
    peakSimilarity(1e6), 
    tailSimilarity(1e6),
    backConsistency(1e6),
    rejectPU(true) {
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
  }

  void TrinityConf::ReadAngleMap(std::string mapfile) {
    std::cout << mapfile << std::endl;
    std::ifstream file(mapfile.c_str());
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
    }
  }

  
  void TrinityConf::Print() {
    for (int i=0; i<nCrystals; ++i) {
      printf("%i    %i.%i.%i     %i.%i.%i   %5.4f    %5.4f\n", GaggID[i], Cr1[i],Sl1[i],Ch1[i], Cr2[i],Sl2[i],Ch2[i], theta[GaggID[i]], phi[GaggID[i]]);
    }
  }
}
