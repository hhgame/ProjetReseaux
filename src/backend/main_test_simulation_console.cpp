#include <iostream>
#include "simulateur.h"
#include "vehicule.h"

int main() {
    Simulateur simu;
    simu.setPasDeTemps(1.0);

    simu.ajouterVehicule(Vehicule(1, 0, 0, 10, 0));
    simu.ajouterVehicule(Vehicule(2, 0, 0, 8, 45));
    simu.ajouterVehicule(Vehicule(3, 0, 0, 2, 90));

    for (int i = 0; i < 5; ++i) {
        std::cout << "=== Temps: " << (i+1) << "s ===\n";
        simu.update();
        simu.afficherEtat();
    }

    return 0;
}
