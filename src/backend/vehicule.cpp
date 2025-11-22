#include "vehicule.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>
#include "GrapheRoutier.h"
#include "Noeud.h"
#include <algorithm>

Vehicule::Vehicule(int id, double x, double y, double vitesse, double direction)
    : id{id}, x{x}, y{y}, vitesse{vitesse}, direction{direction}
{
    std::srand(std::time(0));
    const int RANGE = MAX - MIN + 1;
    rayonTransmission = (std::rand() % RANGE) + MIN;
}

Vehicule::Vehicule(int id, int rayonTrans, double x, double y, double vitesse, double direction)
    : id{id}, x{x}, y{y}, vitesse{vitesse}, direction{direction}, rayonTransmission{rayonTrans}
{}

int Vehicule::getId() const
{
    return id;
}

double Vehicule::getX() const
{
    return x;
}

double Vehicule::getY() const
{
    return y;
}

double Vehicule::getVitesse() const
{
    return vitesse;
}

double Vehicule::getDirection() const
{
    return direction;
}

int Vehicule::getRayonTransmission() const {
    return rayonTransmission;
}

void Vehicule::setVitesse(double v)
{
    vitesse = v;
}

void Vehicule::setDirection(double dir)
{
    direction = dir;
}

void Vehicule::afficherEtat() const {
    std::cout << "Vehicule " << id
              << " | Position: (" << x << ", " << y << ")"
              << " | Vitesse: " << vitesse
              << " | Direction: " << direction << " degres"
              << std::endl;
}

void Vehicule::setNoeudDepart(long id) {
    noeudActuelId = id;
    noeudDestinationId = -1;
    positionSurArete = 0.0;
    dernierNoeudId = -1; // départ sans précédent
}

void Vehicule::setRayonTransmission(int r) {
    rayonTransmission = r;
}

void Vehicule::avancerSurGraphe(double dt, const GrapheRoutier& graphe) {
    const double EPS = 1e-6;

    // Noeud source
    const Noeud* src = graphe.getNoeudParId(noeudActuelId);
    if (!src) return;

    // Choisir une destination si nécessaire
    if (noeudDestinationId == -1) {
        const auto& voisins = graphe.getVoisins(noeudActuelId);
        if (voisins.empty()) return;

        std::vector<long> candidats;

        // Exclure le dernier noeud (empêche de faire demi-tour constant)
        for (long v : voisins) {
            if (v != dernierNoeudId) candidats.push_back(v);
        }

        // S'il n'y a aucun autre choix: obligé de retourner en arrière
        if (candidats.empty()) candidats = voisins;

        // Choisir un voisin aléatoire
        noeudDestinationId = candidats[std::rand() % candidats.size()];
    }


    // Noeud destination
    const Noeud* dst = graphe.getNoeudParId(noeudDestinationId);
    if (!dst) return;

    // Distance sur l'arête
    double dArete = graphe.calculerDistance(*src, *dst);
    if (dArete <= EPS) return;

    double distanceRestante = (1.0 - positionSurArete) * dArete;
    double distancePossible = vitesse * dt;

    if (distancePossible + EPS < distanceRestante) {
        positionSurArete += distancePossible / dArete;
        // position et direction
        x = src->getLatitude()  + (dst->getLatitude()  - src->getLatitude())  * positionSurArete;
        y = src->getLongitude() + (dst->getLongitude() - src->getLongitude()) * positionSurArete;
    } else {
        // arrivée au noeud
        positionSurArete = 1.0;
        x = dst->getLatitude();
        y = dst->getLongitude();

        dt -= distanceRestante / vitesse;
        dernierNoeudId = noeudActuelId;
        noeudActuelId = noeudDestinationId;
        noeudDestinationId = -1;
        positionSurArete = 0.0;
    }

    // Calcul direction seulement si on bouge
    double dx = dst->getLatitude()  - src->getLatitude();
    double dy = dst->getLongitude() - src->getLongitude();
    direction = std::atan2(dy, dx) * 180.0 / M_PI;
    if (direction < 0) direction += 360.0;
}




