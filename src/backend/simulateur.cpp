#include "simulateur.h"
#include <iostream>

Simulateur::Simulateur()
    : gestionTemps(1.0)
{}

void Simulateur::ajouterVehicule(const Vehicule& v) {
    vehicules.push_back(v);
    graphe.majGraphe(vehicules);
}

void Simulateur::update() {
    if (!gestionTemps.estActif())
        return;

    for (auto& v : vehicules) {
        v.avancer(gestionTemps.getPasDeTemps());
    }

    gestionTemps.avancer();
    graphe.majGraphe(vehicules);
}

void Simulateur::afficherEtat() const {
    std::cout << "=== Temps: " << gestionTemps.getTempsCourant() << "s ===" << std::endl;
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
    gestionTemps.reinitialiser();
}

void Simulateur::setPasDeTemps(double dt) {
    gestionTemps.setPasDeTemps(dt);
}

void Simulateur::lierAuGraphe() {
}
