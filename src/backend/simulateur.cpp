#include "simulateur.h"
#include <iostream>
#include <random>
#include <ctime>

#define RAYON_MIN 50
#define RAYON_MAX 200

Simulateur::Simulateur() {
    const std::string pathMap = "../../map";
    routes.chargerDepuisOSM(pathMap);
    routes.afficherResume();

    // Préparer la liste des nœuds pour tirage aléatoire
    for (const auto& [id, n] : routes.getNoeuds())
        vecteurNoeuds.push_back(id);

    ajouterVehiculesAleatoires(2.0, 10.0);
}

void Simulateur::ajouterVehiculesAleatoires(double vitesseMin, double vitesseMax) {
    if (vecteurNoeuds.empty()) {
        std::cerr << "Erreur : aucun nœud chargé pour placer les véhicules." << std::endl;
        return;
    }

    // Générateur aléatoire
    static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_real_distribution<double> distVitesse(vitesseMin, vitesseMax);
    std::uniform_int_distribution<int> distDirection(0, 359);
    std::uniform_int_distribution<int> distRayon(RAYON_MIN, RAYON_MAX);

    vehicules.clear();
    vehicules.reserve(nbVehicules); // Pré-allocation

    const size_t nNoeuds = vecteurNoeuds.size();

    for (int i = 0; i < nbVehicules; ++i) {
        long idNoeud = vecteurNoeuds[rng() % nNoeuds];
        const Noeud* noeud = routes.getNoeudParId(idNoeud);
        if (!noeud) continue;

        int rayon = rayonTransmissionAleatoire ? distRayon(rng) : rayonTransmission;
        double vitesse = distVitesse(rng);
        double direction = distDirection(rng);

        vehicules.emplace_back(i + 1, rayon, noeud->getLatitude(), noeud->getLongitude(), vitesse, direction);
        vehicules.back().setNoeudDepart(idNoeud);
    }

    graphe.majGraphe(vehicules);
    std::cout << nbVehicules << " véhicules ajoutés aléatoirement au graphe." << std::endl;
}

void Simulateur::update() {
    if (facteurVitesse <= 0.0 || vehicules.empty()) return;

    double dt = pasDeTemps * facteurVitesse;
    tempsEcoule += dt;

    // Mise à jour des véhicules
    for (auto& v : vehicules)
        v.avancerSurGraphe(dt, routes);

    // Mise à jour du graphe (positions)
    graphe.majGraphe(vehicules);
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

void Simulateur::lierAuGraphe() {
    // Non utilisé actuellement
}

void Simulateur::placerVehiculeSurNoeud(int idVehicule, long idNoeud) {
    if (idVehicule <= 0 || idVehicule > (int)vehicules.size()) return;
    vehicules[idVehicule-1].setNoeudDepart(idNoeud);
}

void Simulateur::mettreAJourNbVehicules(int nouveauNb) {
    nbVehicules = nouveauNb;
    ajouterVehiculesAleatoires();
    graphe.majGraphe(vehicules);
    std::cout << "Nombre de véhicules mis à jour : " << nbVehicules << std::endl;
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
void Simulateur::setRayonTransmission(int r) { rayonTransmission = r; }
