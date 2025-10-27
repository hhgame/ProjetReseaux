#include "vehicule.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>
#include "GrapheRoutier.h"
#include "Noeud.h"

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

void Vehicule::avancer(double dt) {
    double rad = direction * M_PI / 180.0;
    x += vitesse * std::cos(rad) * dt;
    y += vitesse * std::sin(rad) * dt;

    // Corriger les petites erreurs flottantes
    const double epsilon = 1e-10; // seuil
    if (std::abs(x) < epsilon) x = 0.0;
    if (std::abs(y) < epsilon) y = 0.0;
}

void Vehicule::afficherEtat() const {
    std::cout << "Vehicule " << id
              << " | Position: (" << x << ", " << y << ")"
              << " | Vitesse: " << vitesse
              << " | Direction: " << direction << " degres"
              << std::endl;
}


void Vehicule::placerAuNoeud(const Noeud& n) {
    x = n.getLatitude();
    y = n.getLongitude();
}

void Vehicule::setNoeudDepart(long id) {
    noeudActuelId = id;
    noeudDestinationId = -1; // à choisir au prochain update
    positionSurArete = 0.0;
}

void Vehicule::avancerSurGraphe(double dt, const GrapheRoutier& graphe) {
    // Choisir un nouveau nœud destination si nécessaire
    if (noeudDestinationId == -1) {
        // Choisir un voisin aléatoire
        const Noeud* n = graphe.getNoeudParId(noeudActuelId);
        if (!n) return;

        // Collecter les voisins
        std::vector<long> voisins;
        for (const auto& a : graphe.getAretes()) {
            if (a.getIdSource() == noeudActuelId) voisins.push_back(a.getIdDestination());
            if (!graphe.get_estOriente() && a.getIdDestination() == noeudActuelId)
                voisins.push_back(a.getIdSource());
        }
        if (voisins.empty()) return;

        noeudDestinationId = voisins[std::rand() % voisins.size()];
    }

    const Noeud* src = graphe.getNoeudParId(noeudActuelId);
    const Noeud* dst = graphe.getNoeudParId(noeudDestinationId);
    if (!src || !dst) return;

    // Calculer distance à parcourir sur l'arête
    double dArete = graphe.calculerDistance(*src, *dst);
    double dParcourue = vitesse * dt;

    positionSurArete += dParcourue / dArete;

    if (positionSurArete >= 1.0) {
        // Arrivé au nœud destination
        noeudActuelId = noeudDestinationId;
        positionSurArete = 0.0;
        noeudDestinationId = -1; // choisir nouveau voisin au prochain update
    }

    // Mettre à jour coordonnées x, y
    x = src->getLatitude()  + (dst->getLatitude()  - src->getLatitude())  * positionSurArete;
    y = src->getLongitude() + (dst->getLongitude() - src->getLongitude()) * positionSurArete;

    // Mettre à jour direction (en degrés)
    double dx = dst->getLatitude()  - src->getLatitude();
    double dy = dst->getLongitude() - src->getLongitude();
    direction = std::atan2(dy, dx) * 180.0 / M_PI;
    if (direction < 0) direction += 360.0;
}
