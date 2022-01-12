#include <fstream>
#include <cmath>

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

    for (int i=0; i<MAX_CLOVERS; ++i) {
      pos[i] = 0; //default furthest in
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
      ss >> pos[CloverID[n]];
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
      for (int i=0; i<4; ++i) {
        for (int i2=0; i2<4; ++i2) {
          offset_ab[id][i][i2] = 0.0;
          gain_ab[id][i][i2] = 1.0;
        }
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

  void ClarionConf::ReadAbCal(std::string calfile) {
    std::cout << calfile << std::endl;
    std::ifstream file(calfile.c_str());
    file.precision(9);
    std::string line;

    int n = 0;
    while (std::getline(file, line)) {
      if (line.size() == 0) { continue; }
      if (line[0] == '#') { continue; }
      if (line[0] == ';') { continue; }

      std::stringstream ss(line);
      ss.precision(9);
      
      int id, i, i2;
      double off, gain;
      ss >> id >> i >> i2 >> off >> gain;
 
      offset_ab[id][i][i2] = off;
      gain_ab[id][i][i2] = gain;
      offset_ab[id][i2][i] = off;
      gain_ab[id][i2][i] = gain;

      printf("%i    %i    %i    %15.14f     %15.14f\n", id, i, i2, offset_ab[id][i][i2], gain_ab[id][i][i2]);
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
      double phin, phim, phif, thetan, thetam, thetaf;
      ss >> id >> i >> thetan >> phin >> thetam >> phim >> thetaf >> phif;

      if (pos[id] == 0) { //near
        theta[id][i] = thetan;
        phi[id][i] = phin;
      }
      else if (pos[id] == 1) { //mid
        theta[id][i] = thetam;
        phi[id][i] = phim;
      }
      else if (pos[id] == 2) { //far
        theta[id][i] = thetam;
        phi[id][i] = phim;
      }
      else { //wherever you are
        std::cout << "Error: Invalid Clover position " << pos[id] << " - should be: " << std::endl;
        std::cout << "    0: nearest" << std::endl;
        std::cout << "    1: middle" << std::endl;
        std::cout << "    2: furthest" << std::endl;
      }
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

  void ClarionConf::ReadEfficiencyCal(std::string fn, const int &ID) {
    std::ifstream file(fn.c_str());
    std::string line;
    getline(file, line);
    getline(file, line);
    std::stringstream ss(line);
    std::cout << line << std::endl;
    float abseff, a, aerr, b, berr, c, cerr, d, derr, e, eerr, f, ferr, g, gerr;
    ss >> abseff >> a >> aerr >> b >> berr >> c >> cerr >> d >> derr >> e >> eerr >> f >> ferr >> g >> gerr;
    AbsEff_i[ID] = abseff;
    EffCal_i[ID][0] = a;
    EffCal_i[ID][1] = b;
    EffCal_i[ID][2] = c;
    EffCal_i[ID][3] = d;
    EffCal_i[ID][4] = e;
    EffCal_i[ID][5] = f;
    EffCal_i[ID][6] = g;
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

  float ClarionConf::Efficiency(const double &e, const int &ID) const {
    double x = std::log(e/100.0);
    double y = std::log(e/1000.0);
    float eff = std::pow((EffCal_i[ID][0] + EffCal_i[ID][1]*x + EffCal_i[ID][2]*x*x), -EffCal_i[ID][6]) +
      std::pow((EffCal_i[ID][3] + EffCal_i[ID][4]*y + EffCal_i[ID][5]*y*y), -EffCal_i[ID][6]);
    eff = std::pow(eff, -1.0/EffCal_i[ID][6]);
    eff = AbsEff_i[ID]*std::exp(eff);
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
    std::cout << "Clarion Addback correction: " << std::endl;
    for (int i=0; i<nClovers; ++i) {
      for (int j=0; j<4; ++j) {
        for (int k=j+1; k<4; ++k) {
          printf("%i    %i    %i    %15.14f     %15.14f\n", CloverID[i], j, k, offset_ab[CloverID[i]][j][k], gain_ab[CloverID[i]][j][k]);
        }
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
