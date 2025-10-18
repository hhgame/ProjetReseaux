#include "GrapheInterference.h"
#include <cmath>
#include <iostream>


GrapheInterference::GrapheInterference(double rayonDefaut)
    : rayonTransmissionDefaut(rayonDefaut)
{
    liens.clear();
}

void GrapheInterference::majGraphe(const std::vector<Vehicule>& vehicules)
{
    clear();

    for (size_t i = 0; i < vehicules.size(); ++i)
    {
        for (size_t j = i + 1; j < vehicules.size(); ++j)
        {
            double xA = vehicules[i].getX();
            double yA = vehicules[i].getY();
            double xB = vehicules[j].getX();
            double yB = vehicules[j].getY();

            double dx = xA - xB;
            double dy = yA - yB;
            double distance = std::sqrt(dx * dx + dy * dy);

            double rayonA = vehicules[i].getRayonTransmission();
            double rayonB = vehicules[j].getRayonTransmission();

            if (rayonA <= 0) rayonA = rayonTransmissionDefaut;
            if (rayonB <= 0) rayonB = rayonTransmissionDefaut;

            if (distance <= (rayonA + rayonB))
            {
                LienCommunication* lien = new LienCommunication{
                    vehicules[i].getId(),
                    vehicules[j].getId(),
                    distance
                };

                liens.push_back(lien);
            }
        }
    }
}

std::vector<LienCommunication*> GrapheInterference::getLiens() const
{
    return liens;
}

void GrapheInterference::afficherGraphe() const
{
    if (liens.empty())
    {
        std::cout << "Aucune connexion V2V active." << std::endl;
        return;
    }

    std::cout << "==== Connexions V2V actives ====" << std::endl;
    for (const auto& lien : liens)
    {
        lien->afficherInfos();
    }
    std::cout << "================================" << std::endl;
}

void GrapheInterference::clear()
{
    for(size_t i = 0; i<liens.size(); i++) {
        delete liens[i];
    }
    liens.clear();
}
