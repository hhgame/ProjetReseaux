#include "simulateur.h"
#include <iostream>
<<<<<<< HEAD
#include <string>

Simulateur::Simulateur()
    : tempsEcoule{0.0}, pasDeTemps{1.0}
{
    // Chemin du fichier map
    const std::string pathMap = "../../map"; // ton fichier s'appelle "map"
    routes.chargerDepuisOSM(pathMap);
    routes.afficherResume();
}
=======

Simulateur::Simulateur()
    : tempsEcoule{0.0}, pasDeTemps{1.0}
{}
>>>>>>> 5467289d52eb9a8094f8d9e318f4196ff04ef2c3

void Simulateur::ajouterVehicule(const Vehicule& v) {
    vehicules.push_back(v);
    graphe.majGraphe(vehicules);
}

void Simulateur::update() {
    for (auto& v : vehicules) {
        v.avancer(pasDeTemps);
    }
    tempsEcoule += pasDeTemps;
    graphe.majGraphe(vehicules);
}

void Simulateur::afficherEtat() const {
    std::cout << "=== Temps: " << tempsEcoule << "s ===" << std::endl;
    for (const auto& v : vehicules) {
        v.afficherEtat();
    }
}

int Simulateur::getNombreVehicules() const {
    return vehicules.size();
}

void Simulateur::reinitialiser() {
    vehicules.clear();
    graphe.majGraphe(vehicules);
    tempsEcoule = 0.0;
}

void Simulateur::setPasDeTemps(double dt) {
    pasDeTemps = dt;
}

void Simulateur::lierAuGraphe() {
}
