#include "simulateur.h"
#include <iostream>
#include <random>
#include <ctime>
#include "grapheinterference.h"

#define RAYON_MIN 50
#define RAYON_MAX 200

Simulateur::Simulateur() {
    const std::string pathMap = "../../map";
    routes.chargerDepuisOSM(pathMap);
    routes.afficherResume();

    // Préparer la liste des noeuds pour tirage aléatoire
    for (const auto& [id, n] : routes.getNoeuds())
        vecteurNoeuds.push_back(id);

    graphe = GrapheInterference();
    ajouterVehiculesAleatoires(20.0, 100.0);
}

void Simulateur::ajouterVehiculesAleatoires(double vitesseMin, double vitesseMax) {
    if (vecteurNoeuds.empty()) {
        std::cerr << "Erreur : aucun nœud chargé pour placer les véhicules." << std::endl;
        return;
    }

    static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_real_distribution<double> distVitesse(vitesseMin, vitesseMax);
    std::uniform_int_distribution<int> distDirection(0, 359);
    std::uniform_int_distribution<int> distRayon(RAYON_MIN, RAYON_MAX);

    vehicules.clear();
    vehicules.reserve(nbVehicules);

    const size_t nNoeuds = vecteurNoeuds.size();

    for (int i = 0; i < nbVehicules; ++i) {
        long idNoeud = vecteurNoeuds[rng() % nNoeuds];
        const Noeud* noeud = routes.getNoeudParId(idNoeud);
        if (!noeud) continue;

        double vitesse = distVitesse(rng);
        double direction = distDirection(rng);

        // Rayon selon aléatoire ou fixe
        int rayon = rayonTransmissionAleatoire ? distRayon(rng) : rayonTransmission;

        vehicules.emplace_back(i + 1, rayon, noeud->getLatitude(), noeud->getLongitude(), vitesse, direction);
        vehicules.back().setNoeudDepart(idNoeud);
    }

    // On recalcule les rayons si nécessaire pour tous les véhicules avant de mettre à jour le graphe
    if (rayonTransmissionAleatoire) {
        for (auto &veh : vehicules) {
            veh.setRayonTransmission(distRayon(rng));
        }
    } else {
        for (auto &veh : vehicules) {
            veh.setRayonTransmission(rayonTransmission);
        }
    }

    // Mise à jour du graphe d’interférence
    if (afficherGrapheInterference) {
        recalculerGrapheInterference();
    } else {
        clearGrapheInterference();
    }
}

void Simulateur::update() {
    if (facteurVitesse <= 0.0 || vehicules.empty()) return;

    double dt = pasDeTemps * facteurVitesse;
    tempsEcoule += dt;

    // Mise à jour des véhicules
    for (auto& v : vehicules)
        v.avancerSurGraphe(dt, routes);

    // Mise à jour du graphe (positions)
    if(afficherGrapheInterference) {
        graphe.majGraphe(vehicules);
    }
}

void Simulateur::afficherEtat() const {
    std::cout << "=== Temps: " << tempsEcoule << "s ===" << std::endl;
    for (const auto& v : vehicules) v.afficherEtat();
}

void Simulateur::reinitialiser() {
    vehicules.clear();
    graphe.majGraphe(vehicules);
    tempsEcoule = 0.0;
}

void Simulateur::mettreAJourNbVehicules(int nouveauNb) {
    nbVehicules = nouveauNb;
    ajouterVehiculesAleatoires(20.0, 100.0);
}

// getters/setters
bool Simulateur::getRayonAleatoire() const { return rayonTransmissionAleatoire; }
void Simulateur::setRayonAleatoire(bool v) { rayonTransmissionAleatoire = v; }

void Simulateur::setAfficheRayonTransmission(bool v) { afficherRayonTransmission = v; }
void Simulateur::setAfficheGrapheInterference(bool v) { afficherGrapheInterference = v; }

void Simulateur::setRayonTransmission(int r) {
    rayonTransmission = r;
    // Si on ne veut pas de rayon aléatoire, appliquer à tous les véhicules
    if (!rayonTransmissionAleatoire) {
        for (auto &veh : vehicules) {
            veh.setRayonTransmission(r);
        }
    }
}

void Simulateur::recalculerGrapheInterference() {
    graphe.clear();
    graphe.majGraphe(vehicules);
}

void Simulateur::clearGrapheInterference() {
    graphe.clear();
}


// Accesseurs
int Simulateur::getNombreVehicules() const { return vehicules.size(); }
const std::vector<Vehicule>& Simulateur::getVehicules() const { return vehicules; }

double Simulateur::getFacteurVitesse() const { return facteurVitesse; }
void Simulateur::setFacteurVitesse(double f) { facteurVitesse = f; }

double Simulateur::getPasDeTemps() const { return pasDeTemps; }
void Simulateur::setPasDeTemps(double dt) { pasDeTemps = dt; }

int Simulateur::getNbVehicules() const { return nbVehicules; }
void Simulateur::setNbVehicules(int n) { nbVehicules = n; }

int Simulateur::getRayonTransmission() const { return rayonTransmission; }

bool Simulateur::getAfficheRayonTransmission() const { return afficherRayonTransmission; }
bool Simulateur::getAfficheGrapheInterference() const { return afficherGrapheInterference; }
std::vector<LienCommunication*> Simulateur::getLiens() const { return graphe.getLiens(); }
