#ifndef SIMULATEUR_H
#define SIMULATEUR_H

#include <vector>
#include "vehicule.h"
#include "grapheinterference.h"
<<<<<<< HEAD
#include "GrapheRoutier.h"
=======
>>>>>>> 5467289d52eb9a8094f8d9e318f4196ff04ef2c3

class Simulateur {
    private:
        std::vector<Vehicule> vehicules;
        // Graphe des interference entre voiture
        GrapheInterference graphe;
<<<<<<< HEAD
        GrapheRoutier routes;
=======
>>>>>>> 5467289d52eb9a8094f8d9e318f4196ff04ef2c3
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
