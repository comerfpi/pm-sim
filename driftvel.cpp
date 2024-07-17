#include <iostream>
#include <NEST.hh>
#include <DetectorExample_XENON10.hh>

int main() {
    // Initialize NEST
    NEST::NESTcalc nestCalc;
    
    // Set up the example detector
    NEST::DetectorExample_Xenon detector;

    double Ecg = 800; // Field between cathode and grid 1
    double Egg = 1600; // Field between two grids
    double Ega = 2400; // Field between grid 2 and anode
    
    // Define an array of electric field values (in V/cm)
    double electricFields[3] = {Ecg, Egg, Ega};
    
    // Loop over each electric field value
    for (int i = 0; i < 3; ++i) {
        // Set the electric field for the current iteration
        detector.SetElectricField(electricFields[i]);
        
        // Get the density of liquid xenon at desired temperature and pressure
        double density = detector.LiquidDensity();
        
        // Calculate the drift velocity
        double driftVelocity = nestCalc.GetDriftVelocity(detector, density);
        
        // Output the result
        std::cout << "Electric Field: " << electricFields[i] << " V/cm" << std::endl;
        std::cout << "Drift Velocity: " << driftVelocity << " cm/µs" << std::endl;
        std::cout << std::endl;
    }
    
    return 0;
}
