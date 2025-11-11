#ifndef SIMULATEUR_H
#define SIMULATEUR_H

#include <vector>
#include "vehicule.h"
#include "grapheinterference.h"
#include "GrapheRoutier.h"

class Simulateur {
private:
    std::vector<Vehicule> vehicules;

    bool rayonTransmissionAleatoire = true;
    int rayonTransmission = 100;
    int nbVehicules = 100;

    GrapheInterference graphe;
    GrapheRoutier routes;

    // Pré-calcul des IDs de nœuds pour tirage aléatoire
    std::vector<long> vecteurNoeuds;

    double tempsEcoule = 0.0;
    double pasDeTemps = 0.5;       // temps entre updates
    double facteurVitesse = 1.0;   // 1.0 = normal, 2.0 = x2, 0 = pause

public:
    Simulateur();

    // Simulation
    void ajouterVehiculesAleatoires(double vitesseMin = 5.0, double vitesseMax = 20.0);
    void update();
    void afficherEtat() const;
    void reinitialiser();
    void lierAuGraphe();

    // Accesseurs
    int getNombreVehicules() const;
    const std::vector<Vehicule>& getVehicules() const;

    double getFacteurVitesse() const;
    void setFacteurVitesse(double facteur);
    double getPasDeTemps() const;
    void setPasDeTemps(double dt);

    int getNbVehicules() const;
    void setNbVehicules(int n);
    int getRayonTransmission() const;
    void setRayonTransmission(int r);

    void mettreAJourNbVehicules(int nouveauNb);

    // Gestion véhicules
    void placerVehiculeSurNoeud(int idVehicule, long idNoeud);
};

#endif // SIMULATEUR_H
