// Generate histogram from COMSOL electric field components

#include <iostream>
#include <fstream>
#include <string>
#include <TH1F.h>
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
    TH1F *histX = new TH1F("histX", "Distribution of Ex", 100, -10, 10);
    TH1F *histY = new TH1F("histY", "Distribution of Ey", 100, -10, 10);
    TH1F *histZ = new TH1F("histZ", "Distribution of Ez", 100, -10, 10);

    // Read data, discarding %comments
    double x, y, z, Ex, Ey, Ez;
    std::string line;

    while (std::getline(inFile, line)) {
      if (line[0] == '%') continue;

      std::istringstream ss(line);
      if (ss >> x >> y >> z >> Ex >> Ey >> Ez) {
	histX->Fill(Ex);
	histY->Fill(Ey);
	histZ->Fill(Ez);
      }
    }

    inFile.close();

    // Create canvas, draw histograms
    TCanvas *canvas = new TCanvas("canvas", "Electric Field Distributions", 800, 600);
    canvas->Divide(3,1);

    canvas->cd(1);
    histX->Draw();

    canvas->cd(2);
    histY->Draw();

    canvas->cd(3);
    histZ->Draw();

    canvas->SaveAs("histogram.svg");

    delete histX;
    delete histY;
    delete histZ;
    delete canvas;
}

// For compiling
int main() {
    histogrammer();
    return 0;
}
