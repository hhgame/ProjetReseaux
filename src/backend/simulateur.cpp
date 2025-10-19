#include "simulateur.h"
#include <iostream>

Simulateur::Simulateur()
    : tempsEcoule{0.0}, pasDeTemps{1.0}
{}

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
