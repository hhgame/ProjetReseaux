#include "LienCommunication.h"
#include <iostream>


LienCommunication::LienCommunication()
    : vehiculeA({-1,-1}), vehiculeB({-1,-1}), distance(0.0)
{}

LienCommunication::LienCommunication(vehiculeOpti idA, vehiculeOpti idB, double dist)
    : vehiculeA(idA), vehiculeB(idB), distance(dist)
{}

vehiculeOpti LienCommunication::getVehiculeA() const
{
    return vehiculeA;
}

vehiculeOpti LienCommunication::getVehiculeB() const
{
    return vehiculeB;
}

double LienCommunication::getDistance() const
{
    return distance;
}

bool LienCommunication::estEgale(const LienCommunication& autre) const
{
    return (((vehiculeA.lat == autre.vehiculeA.lat && vehiculeA.lon == autre.vehiculeA.lon)
                 && (vehiculeB.lat == autre.vehiculeB.lat && vehiculeB.lon == autre.vehiculeB.lon) ||
            ((vehiculeA.lat == autre.vehiculeB.lat && vehiculeA.lon == autre.vehiculeB.lon)
                 && (vehiculeB.lat == autre.vehiculeA.lat && vehiculeB.lon == autre.vehiculeA.lon))));
}

void LienCommunication::afficherInfos() const
{
    std::cout << "Lien : Vehicule " << vehiculeA.lat <<" , "<<vehiculeA.lon
              << " <-> Vehicule " << vehiculeB.lat << " , "<<vehiculeB.lon
              << " (distance = " << distance << " m)" << std::endl;
}
