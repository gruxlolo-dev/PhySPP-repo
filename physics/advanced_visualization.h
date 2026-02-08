#pragma once
#include "../core/vec3.h"
#include <vector>
#include <string>
#include <fstream>

class AdvancedVisualization {
public:
    struct DensityMap {
        std::vector<std::vector<std::vector<double>>> data;
        int nx, ny, nz;
        double xmin, xmax, ymin, ymax, zmin, zmax;
    };
    
    struct VelocityField {
        std::vector<Vec3> positions;
        std::vector<Vec3> velocities;
    };
    
    static DensityMap createDensityMap(const std::vector<Vec3>& positions, 
                                       const std::vector<double>& masses,
                                       int resolution) {
        DensityMap map;
        map.nx = map.ny = map.nz = resolution;
        map.data.resize(resolution, std::vector<std::vector<double>>(resolution, std::vector<double>(resolution, 0.0)));
        
        double maxDist = 0;
        for(const auto& p : positions) {
            double d = p.length();
            if(d > maxDist) maxDist = d;
        }
        map.xmin = map.ymin = map.zmin = -maxDist;
        map.xmax = map.ymax = map.zmax = maxDist;
        
        double dx = (map.xmax - map.xmin) / resolution;
        double dy = (map.ymax - map.ymin) / resolution;
        double dz = (map.zmax - map.zmin) / resolution;
        
        for(size_t i=0; i<positions.size(); i++) {
            int ix = (int)((positions[i].x - map.xmin) / dx);
            int iy = (int)((positions[i].y - map.ymin) / dy);
            int iz = (int)((positions[i].z - map.zmin) / dz);
            if(ix >= 0 && ix < resolution && iy >= 0 && iy < resolution && iz >= 0 && iz < resolution) {
                map.data[ix][iy][iz] += masses[i];
            }
        }
        
        return map;
    }
    
    static void exportDensityMapVTK(const std::string& filename, const DensityMap& map) {
        std::ofstream file(filename);
        file << "# vtk DataFile Version 3.0\n";
        file << "Density Map\n";
        file << "ASCII\n";
        file << "DATASET STRUCTURED_POINTS\n";
        file << "DIMENSIONS " << map.nx << " " << map.ny << " " << map.nz << "\n";
        file << "ORIGIN " << map.xmin << " " << map.ymin << " " << map.zmin << "\n";
        double dx = (map.xmax - map.xmin) / map.nx;
        double dy = (map.ymax - map.ymin) / map.ny;
        double dz = (map.zmax - map.zmin) / map.nz;
        file << "SPACING " << dx << " " << dy << " " << dz << "\n";
        file << "POINT_DATA " << map.nx * map.ny * map.nz << "\n";
        file << "SCALARS density double\n";
        file << "LOOKUP_TABLE default\n";
        for(int k=0; k<map.nz; k++) {
            for(int j=0; j<map.ny; j++) {
                for(int i=0; i<map.nx; i++) {
                    file << map.data[i][j][k] << "\n";
                }
            }
        }
        file.close();
    }
    
    static std::vector<double> computeSpectrum(const std::vector<double>& temperatures,
                                               const std::vector<double>& densities,
                                               int numBins) {
        std::vector<double> spectrum(numBins, 0.0);
        double minE = 1e-10;
        double maxE = 1e-6;
        double logMin = log10(minE);
        double logMax = log10(maxE);
        double dlogE = (logMax - logMin) / numBins;
        
        for(int i=0; i<numBins; i++) {
            double E = pow(10.0, logMin + i * dlogE);
            for(size_t j=0; j<temperatures.size(); j++) {
                spectrum[i] += densities[j] * exp(-E / (1.38e-23 * temperatures[j]));
            }
        }
        return spectrum;
    }
};
