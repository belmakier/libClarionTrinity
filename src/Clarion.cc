 #include <fstream>

#include "Clarion.hh"

namespace ClarionTrinity {
  ClarionConf::ClarionConf(std::string conffile) {
    for (int i=0; i<CT_MAX_CRATES; ++i) {
      for (int j=0; j<CT_MAX_SLOTS_PER_CRATE; ++j) {
        for (int k=0; k<CT_MAX_CHANNELS_PER_BOARD; ++k) {
          CloverIDMap[i][j][k] = -1;
          CrystalMap[i][j][k] = -1;
        }
      }
    }
    
    std::cout << conffile << std::endl;
    std::ifstream file(conffile.c_str());
    std::string line; 

    int n = 0;
    while (std::getline(file, line)) {
      if (line.size() == 0) { continue; }
      if (line[0] == '#') { continue; }
      if (line[0] == ';') { continue; }

      std::stringstream ss(line);

      ss >> CloverID[n];
      for (int i=0; i<8; ++i) {
        ss >> Cr[n][i] >> Sl[n][i] >> Ch[n][i];
        CloverIDMap[Cr[n][i]][Sl[n][i]][Ch[n][i]] = CloverID[n];
        CrystalMap[Cr[n][i]][Sl[n][i]][Ch[n][i]] = i;
      } 

      n += 1;
    }
    nClovers = n;

    for (int j=0; j<nClovers; ++j) {
      int id = CloverID[j];
      for (int i=0; i<8; ++i) {
        gain[id][i] = 1.0;
        offset[id][i] = 0.0;        
      }
    }
  }

  void ClarionConf::ReadCal(std::string calfile) {
    std::cout << calfile << std::endl;
    std::ifstream file(calfile.c_str());
    std::string line;

    int n = 0;
    while (std::getline(file, line)) {
      if (line.size() == 0) { continue; }
      if (line[0] == '#') { continue; }
      if (line[0] == ';') { continue; }

      std::stringstream ss(line);

      int id, i;
      ss >> id >> i;
      ss >> offset[id][i] >> gain[id][i];
    }
  }

  void ClarionConf::ReadAngleMap(std::string mapfile) {
    std::cout << mapfile << std::endl;
    std::ifstream file(mapfile.c_str());
    std::string line;

    int n = 0;
    while (std::getline(file, line)) {
      if (line.size() == 0) { continue; }
      if (line[0] == '#') { continue; }
      if (line[0] == ';') { continue; }

      std::stringstream ss(line);

      int id, i;
      ss >> id >> i;
      ss >> theta[id][i] >> phi[id][i];
    }
  }

  void ClarionConf::ReadEfficiencyCal(std::string fn) {
    std::ifstream file(fn.c_str());
    std::string line;
    getline(file, line);
    getline(file, line);
    std::stringstream ss(line);
    std::cout << line << std::endl;
    float abseff, a, aerr, b, berr, c, cerr, d, derr, e, eerr, f, ferr, g, gerr;
    ss >> abseff >> a >> aerr >> b >> berr >> c >> cerr >> d >> derr >> e >> eerr >> f >> ferr >> g >> gerr;
    AbsEff = abseff;
    EfficiencyCal[0] = a;
    EfficiencyCal[1] = b;
    EfficiencyCal[2] = c;
    EfficiencyCal[3] = d;
    EfficiencyCal[4] = e;
    EfficiencyCal[5] = f;
    EfficiencyCal[6] = g;
  }

  float ClarionConf::Efficiency(const double &e) const {
    double x = std::log(e/100.0);
    double y = std::log(e/1000.0);
    float eff = std::pow((EfficiencyCal[0] + EfficiencyCal[1]*x + EfficiencyCal[2]*x*x), -EfficiencyCal[6]) +
      std::pow((EfficiencyCal[3] + EfficiencyCal[4]*y + EfficiencyCal[5]*y*y), -EfficiencyCal[6]);
    eff = std::pow(eff, -1.0/EfficiencyCal[6]);
    eff = AbsEff*std::exp(eff);
    return eff;
  }

  void ClarionConf::Print() {
    std::cout << "Clarion configuration: " << std::endl;
    for (int i=0; i<nClovers; ++i) {
      printf("%i   ", CloverID[i]);
      for (int j=0; j<8; ++j) {
        printf("%i.%i.%i   ", Cr[i][j],Sl[i][j],Ch[i][j]);
      }
      printf("\n");
    }
    std::cout << "Clarion calibration: " << std::endl;
    for (int i=0; i<nClovers; ++i) {
      for (int j=0; j<8; ++j) {
        printf("%i  %i     %5.4f    %5.4f\n", CloverID[i], j, offset[CloverID[i]][j], gain[CloverID[i]][j]);
      }
    }
    std::cout << "Clarion angles: " << std::endl;
    for (int i=0; i<nClovers; ++i) {
      for (int j=0; j<5; ++j) {
        printf("%i  %i     %5.4f    %5.4f\n", CloverID[i], j, theta[CloverID[i]][j], phi[CloverID[i]][j]);
      }
    }
    
  }

  
}
