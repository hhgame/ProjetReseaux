#include "simulateur.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>

Simulateur::Simulateur()
    : tempsEcoule{0.0}, pasDeTemps{0.5}, facteurVitesse{1.0}
{
    const std::string pathMap = "../ProjetReseaux/map";
    routes.chargerDepuisOSM(pathMap);
    routes.afficherResume();
    ajouterVehiculesAleatoires(2.0, 10.0);

}

void Simulateur::ajouterVehiculesAleatoires(
    double vitesseMin,
    double vitesseMax
    ) {
    // Initialiser RNG une seule fois
    static bool rngInit = false;
    if (!rngInit) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        rngInit = true;
    }

    const auto& noeudsMap = routes.getNoeuds();
    if (noeudsMap.empty()) {
        std::cerr << "Erreur : aucun noeud chargé dans le graphe routier." << std::endl;
        return;
    }

    std::vector<long> idsNoeuds;
    idsNoeuds.reserve(noeudsMap.size());
    for (const auto& [id, n] : noeudsMap) {
        idsNoeuds.push_back(id);
    }

    for (int i = 0; i < nb_vehicule; i++) {
        long idNoeud = idsNoeuds[std::rand() % idsNoeuds.size()];
        const Noeud& noeud = noeudsMap.at(idNoeud);
        int rayon = rayonTransmissionAleatoire
                        ? (std::rand() % (MAX - MIN + 1)) + MIN
                        : rayonTransmission;
        double vitesse = vitesseMin + (std::rand() / (double)RAND_MAX) * (vitesseMax - vitesseMin);
        double direction = std::rand() % 360;

        Vehicule v(i + 1, rayon, noeud.getLatitude(), noeud.getLongitude(), vitesse, direction);

        // 🟢 Correction : il faut lui dire sur quel nœud il démarre
        v.setNoeudDepart(idNoeud);

        vehicules.push_back(v);
    }

    graphe.majGraphe(vehicules);

    std::cout << nb_vehicule << " véhicules ajoutés aléatoirement au graphe." << std::endl;
}


void Simulateur::update() {
    if (facteurVitesse <= 0.0) return;

    double dt = pasDeTemps * facteurVitesse;
    tempsEcoule += dt;

    for (auto& v : vehicules) {
        v.avancerSurGraphe(dt, routes);
    }

    graphe.majGraphe(vehicules);

    // 🧪 test temporaire
    static int cpt = 0;
    if (++cpt % 10 == 0) {
        std::cout << "== UPDATE ==\n";
        vehicules[0].afficherEtat();
    }
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

const std::vector<Vehicule>& Simulateur::getVehicules() const {
    return vehicules;
}

void Simulateur::setFacteurVitesse(double facteur) {
    facteurVitesse = facteur;
}
double Simulateur::getFacteurVitesse() const {
    return facteurVitesse;
}
