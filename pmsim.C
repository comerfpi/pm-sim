#include <iostream>
#include <fstream>
#include <cstdlib>

#include <TCanvas.h>
#include <TROOT.h>
#include <TApplication.h>

#include "Garfield/ViewDrift.hh"

#include "Garfield/ComponentAnalyticField.hh"
#include "Garfield/MediumMagboltz.hh"
#include "Garfield/Sensor.hh"
#include "Garfield/DriftLineRKF.hh"
#include "Garfield/TrackHeed.hh"


using namespace Garfield;

double sagFunc(double x, double y, double z){
    // A parabola is basically a catenary, right?

    }

void pmsim(){

    // Make LXe medium using NEST-generated gas table
    MediumMagboltz LXe;
    LXe.LoadGasFile("GasTable_Xe_165K.gas");

    }

int main(){
    pmsim();
    }
