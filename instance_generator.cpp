#include <iostream>
#include <random>
#include <map>
#include <vector>

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
    SIZE size;
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

void printInstances(std::vector<Instance> &instances);

int main (int argc, char *argv[]) {
    // Metodo de uso: ./instance_generator <# instancias> <tamaño de problema>
    if (argc != 3) {
        std::cout << "Metodo de uso: ./instance_generator <# instancias> <tamaño de problema (s, m, b)>" << std::endl;
        return 1;
    }
    int numInstances = *argv[1] - '0';
    SIZE size = static_cast<SIZE>(*argv[2]);
    std::vector<Instance> instances = instance_generator(numInstances, size);
    printInstances(instances);
    return 0;
}

std::vector<Instance> instance_generator(int numInstances, SIZE size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> plantDistr;
    std::uniform_int_distribution<> centerDistr;
    std::uniform_int_distribution<> zoneDistr;
    std::uniform_int_distribution<> generalDistr(100, 1000);
    switch (size) {
        case SMALL:
            plantDistr = std::uniform_int_distribution<> (3, 10);
            centerDistr = std::uniform_int_distribution<> (6, 12);
            zoneDistr = std::uniform_int_distribution<> (8, 15);
            break;
        case MEDIUM:
            plantDistr = std::uniform_int_distribution<> (11, 20);
            centerDistr = std::uniform_int_distribution<> (12, 24);
            zoneDistr = std::uniform_int_distribution<> (16, 30);
            break;
        case BIG:
            plantDistr = std::uniform_int_distribution<> (21, 35);
            centerDistr = std::uniform_int_distribution<> (25, 40);
            zoneDistr = std::uniform_int_distribution<> (31, 45);
            break;
    }
    std::vector<Instance> output(numInstances);
    for (int i = 0; i < output.size(); i++) {
        Instance instance;
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

void printInstances(std::vector<Instance> &instances) {
    int numIns = 1;
    for (const Instance &ins : instances) {
        std::cout << "Instance " << numIns << ":" << std::endl;
        std::cout << "Plants:" << std::endl;
        for (const Plant &plant : ins.plants) {
            std::cout << "-Max distribution: " << plant.maxDistribution << std::endl;
            std::cout << "-Send cost:\n";
            for (const auto key : plant.costs) {
                std::cout << "      -Center " << key.first << ": " << key.second << std::endl;
            }
        }
        std::cout << "Centers:" << std::endl;
        for (const Center &center : ins.centers) {
            std::cout << "-Max manage: " << center.maxManage << " | " << "Operation cost: " << center.operationCost << std::endl;
            std::cout << "-Send cost:\n";
            for (const auto key : center.costs) {
                std::cout << "      -Zone " << key.first << ": " << key.second << std::endl;
            }
        }
        std::cout << "Demand zones:" << std::endl;
        for (const demandZone &zone : ins.demandZones) {
            std::cout << "-Demand amount: " << zone.demandAmount << std::endl;
        }
        numIns++;
        std::cout << "############################################################################" << std::endl;
    }
}
