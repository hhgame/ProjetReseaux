#ifndef SIMULATEUR_H
#define SIMULATEUR_H

#include <vector>
#include "gestiontemps.h"
#include "vehicule.h"
#include "grapheinterference.h"

class Simulateur {
    private:
        std::vector<Vehicule> vehicules;
        // Graphe des interference entre voiture
        GrapheInterference graphe;
        GestionTemps gestionTemps;

    public:
        Simulateur();

        void ajouterVehicule(const Vehicule& v);
        void update();
        void afficherEtat() const;
        int getNombreVehicules() const;
        void reinitialiser();
        void setPasDeTemps(double dt);

        //Méthode a prévoir
        void lierAuGraphe();
};

#endif // SIMULATEUR_H
