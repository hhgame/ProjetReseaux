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
    noeudDestinationId = -1;
    positionSurArete = 0.0;
    dernierNoeudId = -1; // départ sans précédent
}


void Vehicule::avancerSurGraphe(double dt, const GrapheRoutier& graphe) {
    const double EPS = 1e-6; // tolérance numérique

    while (dt > 0.0) {

        // --- 1. Choisir une destination si nécessaire ---
        if (noeudDestinationId == -1) {
            const Noeud* src = graphe.getNoeudParId(noeudActuelId);
            if (!src) return;

            std::vector<long> voisins;
            for (const auto& a : graphe.getAretes()) {
                // Considérer toutes les arêtes comme bidirectionnelles
                if (a.getIdSource() == noeudActuelId)
                    voisins.push_back(a.getIdDestination());
                else if (a.getIdDestination() == noeudActuelId)
                    voisins.push_back(a.getIdSource());
            }

            // --- 2. Éviter de faire demi-tour ---
            if (dernierNoeudId != -1 && voisins.size() > 1) {
                voisins.erase(std::remove(voisins.begin(), voisins.end(), dernierNoeudId), voisins.end());
            }

            // --- 3. Cas spécial : cul-de-sac ---
            if (voisins.empty()) {
                if (dernierNoeudId != -1) {
                    noeudDestinationId = dernierNoeudId; // demi-tour forcé
                } else {
                    return; // pas de voisins du tout
                }
            } else {
                noeudDestinationId = voisins[std::rand() % voisins.size()];
            }

            // 🔍 (optionnel) Debug
            // std::cout << "Veh " << id << " " << noeudActuelId << " -> " << noeudDestinationId << " (prev=" << dernierNoeudId << ")\n";
        }

        // --- 4. Récupérer les nœuds source et destination ---
        const Noeud* src = graphe.getNoeudParId(noeudActuelId);
        const Noeud* dst = graphe.getNoeudParId(noeudDestinationId);
        if (!src || !dst) return;

        double dArete = graphe.calculerDistance(*src, *dst);
        if (dArete <= EPS) return;

        // --- 5. Calculer la distance parcourue ---
        double distanceRestante = (1.0 - positionSurArete) * dArete;
        double distancePossible = vitesse * dt;

        if (distancePossible + EPS < distanceRestante) {
            // 🚗 Toujours sur la même arête
            positionSurArete += distancePossible / dArete;
            dt = 0.0;
        } else {
            // 🚗 On atteint le nœud destination
            dt -= distanceRestante / vitesse;
            dernierNoeudId = noeudActuelId;  // <-- AJOUT IMPORTANT
            noeudActuelId = noeudDestinationId;
            noeudDestinationId = -1;
            positionSurArete = 0.0;
            continue; // choisira un nouveau voisin
        }

        // --- 6. Mise à jour position ---
        x = src->getLatitude()  + (dst->getLatitude()  - src->getLatitude())  * positionSurArete;
        y = src->getLongitude() + (dst->getLongitude() - src->getLongitude()) * positionSurArete;

        // --- 7. Mise à jour direction ---
        double dx = dst->getLatitude()  - src->getLatitude();
        double dy = dst->getLongitude() - src->getLongitude();
        direction = std::atan2(dy, dx) * 180.0 / M_PI;
        if (direction < 0) direction += 360.0;

        // Si on est bloqué sur un nœud sans destination, choisir un autre au hasard dans tout le graphe
        if (noeudDestinationId == -1 && dt > 0.0) {
            const auto& noeuds = graphe.getNoeuds();
            if (!noeuds.empty()) {
                auto it = noeuds.begin();
                std::advance(it, std::rand() % noeuds.size());
                noeudDestinationId = it->first;
            }
        }


        break; // fin du pas de temps
    }
}


