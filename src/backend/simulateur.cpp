#include "simulateur.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

#define RAYON_MIN 50
#define RAYON_MAX 200

Simulateur::Simulateur() {
    const std::string pathMap = "../../map";
    routes.chargerDepuisOSM(pathMap);
    routes.afficherResume();
    ajouterVehiculesAleatoires(2.0, 10.0);
}

void Simulateur::ajouterVehiculesAleatoires(double vitesseMin, double vitesseMax) {
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
    for (const auto& [id, n] : noeudsMap) idsNoeuds.push_back(id);

    vehicules.clear();
    for (int i = 0; i < nbVehicules; i++) {
        long idNoeud = idsNoeuds[std::rand() % idsNoeuds.size()];
        const Noeud& noeud = noeudsMap.at(idNoeud);
        int rayon = rayonTransmissionAleatoire
                        ? (std::rand() % (RAYON_MAX - RAYON_MIN + 1)) + RAYON_MIN
                        : rayonTransmission;
        double vitesse = vitesseMin + (std::rand() / (double)RAND_MAX) * (vitesseMax - vitesseMin);
        double direction = std::rand() % 360;

        Vehicule v(i + 1, rayon, noeud.getLatitude(), noeud.getLongitude(), vitesse, direction);
        v.setNoeudDepart(idNoeud);

        vehicules.push_back(v);
    }

    graphe.majGraphe(vehicules);
    std::cout << nbVehicules << " véhicules ajoutés aléatoirement au graphe." << std::endl;
}

void Simulateur::update() {
    if (facteurVitesse <= 0.0) return;

    double dt = pasDeTemps * facteurVitesse;
    tempsEcoule += dt;

    for (auto& v : vehicules) v.avancerSurGraphe(dt, routes);

    graphe.majGraphe(vehicules);

    static int cpt = 0;
    if (++cpt % 10 == 0) {
        std::cout << "== UPDATE ==\n";
        vehicules[0].afficherEtat();
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

void Simulateur::lierAuGraphe() {
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

