#include "GrapheInterference.h"
#include <cmath>
#include <iostream>

GrapheInterference::GrapheInterference(double rayonDefaut)
    : rayonTransmissionDefaut(rayonDefaut)
{
    liens.clear();
}

static double deg2rad(double deg) {
    return deg * M_PI / 180.0;
}

static double distanceEnMetres(double lat1, double lon1, double lat2, double lon2) {
    constexpr double R = 6371000.0; // rayon de la Terre en mètres
    double dLat = deg2rad(lat2 - lat1);
    double dLon = deg2rad(lon2 - lon1);
    double a = sin(dLat/2) * sin(dLat/2) +
               cos(deg2rad(lat1)) * cos(deg2rad(lat2)) *
                   sin(dLon/2) * sin(dLon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1.0 - a));
    return R * c;
}

long long GrapheInterference::hashCell(int cx, int cy) const
{
    return ((long long)cx << 32) ^ (long long)cy;
}

void GrapheInterference::majGraphe(const std::vector<Vehicule>& vehicules)
{
    clear();
    grille.clear();

    if (vehicules.empty()) return;

    // Calcul du rayon maximum parmi tous les véhicules pour dimensionner la grille
    double cellSize = rayonTransmissionDefaut;
    for (const auto& v : vehicules)
    {
        double r = v.getRayonTransmission();
        if (r > cellSize) cellSize = r;
    }

    // Indexation spatiale des véhicules
    for (size_t i = 0; i < vehicules.size(); ++i)
    {
        double lat = vehicules[i].getX();
        double lon = vehicules[i].getY();
        double mLat = lat * 111320.0;
        double mLon = lon * 111320.0 * cos(deg2rad(lat));

        int cx = static_cast<int>(std::floor(mLon / cellSize));
        int cy = static_cast<int>(std::floor(mLat / cellSize));

        grille[hashCell(cx, cy)].vehicules.push_back(i);
    }

    // Recherche des voisins proches (9 cellules voisines)
    for (const auto& entry : grille)
    {
        long long cellHash = entry.first;
        int cx = cellHash >> 32;
        int cy = (int)(cellHash & 0xFFFFFFFF);

        const auto& cellVeh = entry.second.vehicules;

        for (int ox = -1; ox <= 1; ++ox)
        {
            for (int oy = -1; oy <= 1; ++oy)
            {
                auto it = grille.find(hashCell(cx + ox, cy + oy));
                if (it == grille.end()) continue;

                const auto& voisins = it->second.vehicules;

                for (size_t i : cellVeh)
                {
                    for (size_t j : voisins)
                    {
                        if (i >= j) continue; // éviter doublons

                        double latA = vehicules[i].getX();
                        double lonA = vehicules[i].getY();
                        double latB = vehicules[j].getX();
                        double lonB = vehicules[j].getY();

                        double distance = distanceEnMetres(latA, lonA, latB, lonB);

                        double rayonA = vehicules[i].getRayonTransmission();
                        double rayonB = vehicules[j].getRayonTransmission();

                        // Si distance < somme des rayons, créer un lien
                        if (distance <= (rayonA + rayonB))
                        {
                            liens.push_back(new LienCommunication{
                                {latA, lonA},
                                {latB, lonB},
                                distance
                            });
                        }
                    }
                }
            }
        }
    }
}


/**
 * Retourne les liens du graphe
 */
std::vector<LienCommunication*> GrapheInterference::getLiens() const
{
    return liens;
}

/**
 * Affiche le graphe dans la console
 */
void GrapheInterference::afficherGraphe() const
{
    if (liens.empty())
    {
        std::cout << "Aucune connexion V2V active." << std::endl;
        return;
    }

    std::cout << "==== Connexions V2V actives ====" << std::endl;
    for (const auto& lien : liens)
        lien->afficherInfos();
    std::cout << "================================" << std::endl;
}

/**
 * Vide le graphe et libère la mémoire
 */
void GrapheInterference::clear()
{
    for (auto* lien : liens)
        delete lien;

    liens.clear();
}
