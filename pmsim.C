// derivative of claire/efield_modelling
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <random>

#include <TApplication.h>
#include <TCanvas.h>
#include <TH1F.h>

#include "Garfield/ComponentComsol.hh"
#include "Garfield/ViewCell.hh"
#include "Garfield/ViewSignal.hh"
#include "Garfield/ComponentAnalyticField.hh"
#include "Garfield/ViewField.hh"
#include "Garfield/MediumMagboltz.hh"
#include "Garfield/Sensor.hh"
#include "Garfield/AvalancheMicroscopic.hh"
#include "Garfield/AvalancheMC.hh"
#include "Garfield/ViewDrift.hh"
#include "Garfield/DriftLineRKF.hh"
#include "Garfield/Random.hh"
#include "Garfield/ComponentGrid.hh"
#include "Garfield/ViewField.hh"
#include "Garfield/ViewFEMesh.hh"

using namespace Garfield;

int main(int argc, char * argv[]) {
  TApplication app("app", &argc, argv);

  // Create a canvas
  TCanvas* canvas = new TCanvas("canvas", "Drift Lines", 800, 800);

  //number of electrons produced in initial avalanche
  int initElectronTotal = 0;
  //number of electrons that end up at the anode
  int finalElectronTotal = 0;
  const int numRuns = 10;
  //number of electrons to eject
  const int nElectron = 100;
 
  //parameters to change based on comsol model [cm]
  const int zCathode = 4.5155; //yCathode in model
  const int vAnode = 129.6;
  const int vCathode = -1584.4;

  ComponentComsol fm;
  fm.Initialise("data/pumamesh.mphtxt", "data/dielectric.dat", "data/pumafield.txt", "mm");
  fm.PrintRange();

  MediumMagboltz gas;

  //set the temperature [K] and pressure [Torr].
  //100% xenon gas
  gas.SetComposition("xe", 100.);
  //temperature [K]
  gas.SetTemperature(293.);
  //pressure [Torr]
  gas.SetPressure(1350.);
  //initialize table of scattering rates (called internally when a collision rate is requested and the gas mixture or other parameters have changed)
  gas.Initialise(false);  
  
  //load the ion mobilities.
  const std::string path = std::getenv("GARFIELD_INSTALL");
  gas.LoadIonMobility(path + "/share/Garfield/Data/IonMobility_Xe+_P12_Xe.txt");
  gas.LoadGasFile("data/GasTable_Xe_120K.gas");

  fm.SetGas(&gas);
  fm.PrintMaterials();
  fm.EnableConvergenceWarnings(false);

  Sensor sensor;
  sensor.AddComponent(&fm);
  sensor.SetArea(-4.5, -4.5, -15.3, 4.5, 4.5, 6.7);

  //drift line rkf to drift electrons -> calculates path of electron or ion by numerical integration of drift velocity vector, well adapted to smooth fields (e.g. analytic potentials)
  DriftLineRKF driftline;
  driftline.SetSensor(&sensor);
  driftline.EnableAvalanche(false);
  //driftline.EnableDebugging(true);
  
  // int nSteps = 50000;
  // sensor.SetTimeWindow(0., 1, nSteps);
  
  std::ofstream outfile;
  outfile.open("FullTransparency.csv", std::ios::out);
  ViewDrift driftView;
  driftline.EnablePlotting(&driftView);

   // //drift electrons using RKF method
  int totalElectron = 0;
  double avgTransparency=0;
  double gap1 = (4.51-4.35)/11;
  for (int k = 0; k<10; k++) {
  double ejectFrom = 4.35+(k*gap1);
  double measureFrom = 0.162-(k*gap1);
  for (int j = 0; j<numRuns; j++) {
    std::stringstream buffer;
    std::streambuf* oldCerr = std::cerr.rdbuf(buffer.rdbuf());
    int plane = 0;
    for (int i = 0; i<nElectron; i++) {
      std::random_device rd;
      std::mt19937 gen(rd());
      double min = -0.2;
      double max = 0.2;
      std::uniform_real_distribution<double> dist(min, max);
      double random_numberx = dist(gen);
      double random_numbery = dist(gen);
      
      const double startX = random_numberx;
      const double startY = random_numbery;
      const double startZ = ejectFrom;
      driftline.DriftElectron(startX, startY, startZ, 0);
      double x1, y1, z1, t1;
      int status = 0;
      driftline.GetEndPoint(x1, y1, z1, t1, status);
      
      std::vector<std::array<float, 3>> driftLine;
      bool electron;
      size_t driftLineIndex = i;
      driftView.GetDriftLine(driftLineIndex, driftLine, electron);
      for (const auto& point : driftLine) {
		std::cout<<"Electron: "<<i<<", x: "<<point[0]<<", y: "<<point[1]<<", z: "<<point[2]<<std::endl;
	//	outfile<<"Electron: "<<i<<", x: "<<point[0]<<", y: "<<point[1]<<", z: "<<point[2]<<"\n";
      }
      
      // std::cout<<"("<<x1<<", "<<y1<<", "<<z1<<")"<<" status: "<<status<<std::endl;
      if (z1<measureFrom) {
	++plane;
      }
    }
    //outfile.close();

    std::cerr.rdbuf(oldCerr);
    std::string output = buffer.str();
    std::string targetPrefix = "DriftLineRKF::Avalanche";
    std::regex pattern(targetPrefix + ".");

    size_t messageCount = 0;
    std::sregex_iterator it(output.begin(), output.end(), pattern);
    std::sregex_iterator end;

    while (it!=end) {
      messageCount++;
      ++it;
    }

    float gridTransparency = static_cast<float>(plane)/nElectron;
    std::cout<<"Grid Transparency: "<<gridTransparency<<std::endl;
    std::cout<<"Number of Electrons: "<<plane<<"/"<<nElectron<<" + "<<messageCount<<" electrons"<<std::endl;
    outfile<<ejectFrom<<", "<<measureFrom<<", "<<plane<<", "<<messageCount<<"\n";
     totalElectron+=plane;
     avgTransparency+=gridTransparency;
    }
  }
outfile.close();

  // Draw the drift view
  driftView.SetCanvas(canvas);
  driftView.Plot();

  // Run the application
  app.Run(true);

  std::cout << "done" << std::endl;
  return 0;
}
