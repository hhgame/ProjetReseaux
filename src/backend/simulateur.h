#ifndef SIMULATEUR_H
#define SIMULATEUR_H

#include <vector>
#include "vehicule.h"
#include "grapheinterference.h"

class Simulateur {
    private:
        std::vector<Vehicule> vehicules;
        // Graphe des interference entre voiture
        GrapheInterference graphe;
        double tempsEcoule;
        double pasDeTemps; //Temps qui sépare chaque update

    public:
        Simulateur();

        void ajouterVehicule(const Vehicule& v);
        void update();
        void afficherEtat() const;

        int getNombreVehicules() const;

        //Méthode a prévoir
        void reinitialiser();
        void setPasDeTemps(double dt);
        void lierAuGraphe();
};

#endif // SIMULATEUR_H
