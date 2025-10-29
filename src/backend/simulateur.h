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
        int rayonTransmission;
        int nb_vehicule = 100;
        // Graphe des interference entre voiture
        GrapheInterference graphe;
        GrapheRoutier routes;
        double tempsEcoule;
        double pasDeTemps; //Temps qui sépare chaque update
        double facteurVitesse; // facteur de vitesse de simulation (1.0 = normal, 2.0 = 2x plus rapide, 0 = pause)

    public:
        Simulateur();

        void ajouterVehiculesAleatoires(
            double vitesseMin = 5.0,
            double vitesseMax = 20.0
            );

        void update();
        void afficherEtat() const;

        int getNombreVehicules() const;

        void placerVehiculeSurNoeud(int idVehicule, long idNoeud);
        const std::vector<Vehicule>& getVehicules() const;

        void setFacteurVitesse(double facteur);
        double getFacteurVitesse() const;



        //Méthode a prévoir
        void reinitialiser();
        void setPasDeTemps(double dt);
        void lierAuGraphe();
};

#endif // SIMULATEUR_H
