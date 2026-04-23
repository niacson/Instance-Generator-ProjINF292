#include <cstddef>
#include <iostream>
#include <random>
#include <map>
#include <vector>
#include <fstream>
#include <filesystem>

const int baseOperationCost = 500;

enum SIZE : char{
    SMALL = 's',
    MEDIUM = 'm',
    BIG = 'b'
};

struct Plant {
    int maxDistribution;
    std::map<int, int> costs;
};

struct Center {
    int maxManage; // Hace referencia a entrada y salida
    int operationCost;
    std::map<int, int> costs;
};

struct demandZone {
    int demandAmount;
};

struct Instance {
    std::string size;
    int systemTotalDemand;
    int maxOperatingCenters;
    std::vector<Plant> plants;
    std::vector<Center> centers;
    std::vector<demandZone> demandZones;
};

// Methods
std::vector<Instance> instance_generator(int numInstances, SIZE size);

Plant plant_generator(int maxDistribution, int numCenters);

Center center_generator(int maxManage, int numZones, int operationCost);

int sumDemand(const std::vector<demandZone> &zones);

void writeInstaces(std::vector<Instance> &instances, SIZE size);

int main (int argc, char *argv[]) {
    // Metodo de uso: ./instance_generator <# instancias> <tamaño de problema>
    if (argc != 3) {
        std::cout << "Metodo de uso: ./instance_generator <# instancias(1-9)> <tamaño de problema (s, m, b)>" << std::endl;
        return 1;
    }
    // TODO: Ver como manejar el caso en que se ingrese en argv[1] un numero de 2 digitos
    int numInstances = *argv[1] - '0';
    SIZE size = static_cast<SIZE>(*argv[2]);
    std::vector<Instance> instances = instance_generator(numInstances, size);
    writeInstaces(instances, size);
    return 0;
}

std::vector<Instance> instance_generator(int numInstances, SIZE size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> plantDistr;
    std::uniform_int_distribution<> centerDistr;
    std::uniform_int_distribution<> zoneDistr;
    std::uniform_int_distribution<> generalDistr(100, 1000);
    std::string insSize;
    switch (size) {
        case SMALL:
            plantDistr = std::uniform_int_distribution<> (3, 10);
            centerDistr = std::uniform_int_distribution<> (6, 12);
            zoneDistr = std::uniform_int_distribution<> (8, 15);
            insSize = "Small";
            break;
        case MEDIUM:
            plantDistr = std::uniform_int_distribution<> (11, 20);
            centerDistr = std::uniform_int_distribution<> (12, 24);
            zoneDistr = std::uniform_int_distribution<> (16, 30);
            insSize = "Medium";
            break;
        case BIG:
            plantDistr = std::uniform_int_distribution<> (21, 35);
            centerDistr = std::uniform_int_distribution<> (25, 40);
            zoneDistr = std::uniform_int_distribution<> (31, 45);
            insSize = "Big";
            break;
    }
    std::vector<Instance> output(numInstances);
    for (size_t i = 0; i < output.size(); i++) {
        Instance instance;
        instance.size = insSize;
        int numPlants = plantDistr(gen);
        int numCenters = centerDistr(gen);
        int numZones = zoneDistr(gen);
        // Creation of demand zones
        for (int i = 0; i < numZones; i++) {
            instance.demandZones.push_back(demandZone{generalDistr(gen)});
        }
        // Calculate sum of demand
        int baseDemandPerPlant = sumDemand(instance.demandZones) / numPlants;
        // Creation of plants
        for (int i = 0; i < numPlants; i++) {
            // Check if for the last plant the total demand has been covered
            int maxDistribution = generalDistr(gen);
            instance.plants.push_back(plant_generator(maxDistribution + baseDemandPerPlant, numCenters));
        }
        // Creation of centers
        for (int i = 0; i < numCenters; i++) {
            instance.centers.push_back(center_generator(generalDistr(gen), numZones, generalDistr(gen) + baseOperationCost));
        }
        output[i] = instance;
    }
    return output;
}

Plant plant_generator(int maxDistribution, int numCenters) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> costDistr(100, 1000);
    std::map<int, int> costs;
    Plant plant;
    plant.maxDistribution = maxDistribution;
    for (int i = 1; i <= numCenters; i++) {
        costs[i] = costDistr(gen);
    }
    plant.costs = costs;
    return plant;
}

Center center_generator(int maxManage, int numZones, int operationCost) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> costDistr(100, 1000);
    std::map<int, int> costs;
    Center center;
    center.maxManage = maxManage;
    center.operationCost = operationCost;
    for (int i = 1; i <= numZones; i++) {
        costs[i] = costDistr(gen);
    }
    center.costs = costs;
    return center;
}

int sumDemand(const std::vector<demandZone> &zones) {
    int acum = 0;
    for (const demandZone z : zones) {
        acum += z.demandAmount;
    }
    return acum;
}

void writeInstaces(std::vector<Instance> &instances, SIZE size) {
    if (!std::filesystem::exists("instances")) {
        std::filesystem::create_directory("instances");
    }
    std::string outPath;
    switch (size) {
        case SMALL:
            outPath = "instances/small_instances.txt";
            break;
        case MEDIUM:
            outPath = "instances/medium_instances.txt";
            break;
        case BIG:
            outPath = "instances/big_instances.txt";
            break;
    }
    std::ofstream output(outPath);
    int numIns = 1;
    for (const Instance &ins : instances) {
        output << "Instance " << numIns << " | " << ins.size << ":\n";
        output << "Plants:\n";
        for (size_t i = 0; i < ins.plants.size(); i++) {
            output << i + 1 << ".- Max distribution: " << ins.plants.at(i).maxDistribution << "\n";
            output << "-Send cost:\n";
            for (const auto key : ins.plants.at(i).costs) {
                output << "      -Center " << key.first << ": " << key.second << "\n";
            }
        }
        output << "Centers:" << "\n";
        for (size_t i = 0; i < ins.centers.size(); i++) {
            output << i + 1 << ".- Max manage: " << ins.centers.at(i).maxManage << " | " << "Operation cost: " << ins.centers.at(i).operationCost << "\n";
            output << "-Send cost:\n";
            for (const auto key : ins.centers.at(i).costs) {
                output << "      -Zone " << key.first << ": " << key.second << "\n";
            }
        }
        output << "Demand zones:" << "\n";
        for (size_t i = 0; i < ins.demandZones.size(); i++) {
            output << i + 1 << ".- Demand amount: " << ins.demandZones.at(i).demandAmount << "\n";
        }
        numIns++;
        output << "\n" << "\n";
    }
    output.close();
}
