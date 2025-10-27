#include "simulateur.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>

Simulateur::Simulateur()
    : tempsEcoule{0.0}, pasDeTemps{1.0}
{
    // Chemin du fichier map
    const std::string pathMap = "D:/ProjetReseaux/ProjetReseaux/map";
    routes.chargerDepuisOSM(pathMap);
    routes.afficherResume();
    ajouterVehiculesAleatoires();
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
        // --- Choisir un noeud aléatoire ---
        long idNoeud = idsNoeuds[std::rand() % idsNoeuds.size()];
        const Noeud& noeud = noeudsMap.at(idNoeud);

        // --- Rayon de transmission ---
        int rayon = rayonTransmissionAleatoire
                        ? (std::rand() % (MAX - MIN + 1)) + MIN
                        : rayonTransmission;

        // --- Vitesse aléatoire ---
        double vitesse = vitesseMin + (std::rand() / (double)RAND_MAX) * (vitesseMax - vitesseMin);

        // --- Direction aléatoire ---
        double direction = std::rand() % 360;

        // --- Création du véhicule ---
        Vehicule v(i + 1, rayon, noeud.getLatitude(), noeud.getLongitude(), vitesse, direction);

        // --- Ajout au simulateur ---
        vehicules.push_back(v);
    }

    graphe.majGraphe(vehicules);

    std::cout << nb_vehicule << " véhicules ajoutés aléatoirement au graphe." << std::endl;
}


void Simulateur::update() {
    tempsEcoule += pasDeTemps;

    // Seuls les véhicules sont mis à jour tous les 5 secondes
    static double compteur = 0.0;
    compteur += pasDeTemps;

    if (compteur >= 5.0) {
        for (auto& v : vehicules) {
            v.avancerSurGraphe(compteur, routes); // avancer avec le temps cumulé
        }
        graphe.majGraphe(vehicules);
        compteur = 0.0;
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
