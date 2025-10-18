#include "LienCommunication.h"
#include <iostream>

LienCommunication::LienCommunication()
    : idVehiculeA(-1), idVehiculeB(-1), distance(0.0)
{}

LienCommunication::LienCommunication(int idA, int idB, double dist)
    : idVehiculeA(idA), idVehiculeB(idB), distance(dist)
{}

int LienCommunication::getIdVehiculeA() const
{
    return idVehiculeA;
}

int LienCommunication::getIdVehiculeB() const
{
    return idVehiculeB;
}

double LienCommunication::getDistance() const
{
    return distance;
}

bool LienCommunication::estEgale(const LienCommunication& autre) const
{
    return ((idVehiculeA == autre.idVehiculeA && idVehiculeB == autre.idVehiculeB) ||
            (idVehiculeA == autre.idVehiculeB && idVehiculeB == autre.idVehiculeA));
}

void LienCommunication::afficherInfos() const
{
    std::cout << "Lien : Vehicule " << idVehiculeA
              << " <-> Vehicule " << idVehiculeB
              << " (distance = " << distance << " m)" << std::endl;
}
