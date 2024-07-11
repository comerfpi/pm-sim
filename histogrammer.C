// Generate histogram from COMSOL electric field components

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>

void histogrammer() {
    std::ifstream inFile("data/fieldcomp.txt");
    if (!inFile.is_open()) {
        std::cerr << "Unable to open file: do you have fieldcomp.txt in ./data?" << std::endl;
        return;
    }

    // Define histograms
    // 1D
    TH1D *histX = new TH1D("histX", "Distribution of Ex", 100, 0, 80000);
    TH1D *histY = new TH1D("histY", "Distribution of Ey", 100, 0, 80000);
    TH1D *histZ = new TH1D("histZ", "Distribution of Ez", 100, 0, 80000);
    TH1D *histNorm = new TH1D("histNorm", "Distribution of |E|", 100, 0, 240000);
    // 2D
    TH2D *histXY = new TH2D("histXY", "Distribution of Ex and Ey", 100, -8000, 8000, 100, -8000, 8000);


    // Read data, discarding %comments
    double x, y, z, Ex, Ey, Ez, normE;
    std::string line;

    while (std::getline(inFile, line)) {
      if (line[0] == '%') continue;

      std::istringstream ss(line);
      if (ss >> x >> y >> z >> Ex >> Ey >> Ez >> normE) {
	// only take points inside 1mm radius (where there should be uniformity)
	// and only between the cathode and the anode
	//if ((x*x + y*y) >= 1.0 && z <= 2.0 && z >= 45.155) {
	if ((x*x + y*y) <= 1.0) {
	  histX->Fill(Ex);
	  histY->Fill(Ey);
	  histZ->Fill(Ez);
	  histNorm->Fill(normE);
	  histXY->Fill(Ex, Ey);
	  }
      }

    }

    inFile.close();

    // Create canvas, draw histograms
    TCanvas *canvas1 = new TCanvas("canvas1", "Electric Field Distributions", 800, 600);
    canvas1->Divide(3,1);

    canvas1->cd(1);
    histX->Draw();

    canvas1->cd(2);
    histY->Draw();

    canvas1->cd(3);
    histZ->Draw();

    canvas1->SaveAs("histogram.pdf");

    TCanvas *canvas2 = new TCanvas("canvas2", "Electric Field Distributions", 800, 600);

    histXY->Draw("LEGO1");

    // Save the canvas as an SVG file
    canvas2->SaveAs("histogram2D.pdf");

    TCanvas *canvas3 = new TCanvas("canvas3", "Electric Field Norm Distribution", 800, 600);

    histNorm->Draw();

    canvas3->SaveAs("histogramnorm.pdf");

    delete histX;
    delete histY;
    delete histZ;
    delete canvas1;

    delete histXY;
    delete canvas2;

    delete histNorm;
    delete canvas3;
}

// For compiling
int main() {
    histogrammer();
    return 0;
}
