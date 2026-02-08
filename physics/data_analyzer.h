#pragma once
#include <vector>
#include <fstream>
#include <string>

class DataAnalyzer {
public:
    struct Spectrum {
        std::vector<double> wavelengths;
        std::vector<double> intensities;
    };
    
    static Spectrum computeSpectrum(const std::vector<double>& temperatures, 
                                     const std::vector<double>& densities) {
        Spectrum spec;
        for(double lambda = 1e-10; lambda < 1e-6; lambda *= 1.1) {
            spec.wavelengths.push_back(lambda);
            double intensity = 0;
            for(size_t i=0; i<temperatures.size(); i++) {
                intensity += planckFunction(lambda, temperatures[i]) * densities[i];
            }
            spec.intensities.push_back(intensity);
        }
        return spec;
    }
    
    static double planckFunction(double lambda, double T) {
        const double h = 6.626e-34;
        const double c = 299792458.0;
        const double k = 1.38e-23;
        double x = h * c / (lambda * k * T);
        return 2.0 * h * c * c / (lambda*lambda*lambda*lambda*lambda) / (exp(x) - 1.0);
    }
    
    static void exportCSV(const std::string& filename, const Spectrum& spec) {
        std::ofstream file(filename);
        file << "Wavelength,Intensity\n";
        for(size_t i=0; i<spec.wavelengths.size(); i++) {
            file << spec.wavelengths[i] << "," << spec.intensities[i] << "\n";
        }
        file.close();
    }
    
    static double totalLuminosity(const Spectrum& spec) {
        double L = 0;
        for(size_t i=1; i<spec.intensities.size(); i++) {
            double dLambda = spec.wavelengths[i] - spec.wavelengths[i-1];
            L += spec.intensities[i] * dLambda;
        }
        return L;
    }
};
