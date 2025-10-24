#include "ComportementVehicule.h"

// ===== ComportementConstant ===================================================

ComportementConstant::ComportementConstant(double vms) : v_(vms) {}

double ComportementConstant::vitesseDesiree(const InputsComportement& /*in*/) const {
    return v_;
}

double ComportementConstant::accelMax() const { return 2.0; }
double ComportementConstant::freinMax() const { return 4.0; }

// ===== ComportementPrudent ====================================================

double ComportementPrudent::vitesseDesiree(const InputsComportement& in) const {
    // Base : vitesse limite si fournie, sinon ~50 km/h
    double v = (in.vitesseLimite > 0.0) ? in.vitesseLimite : 13.9;
    // Réductions simples en fonction de la distance au véhicule de devant
    if (in.distanceAvant < 20.0)      v *= 0.30;  // très proche
    else if (in.distanceAvant < 40.0) v *= 0.60;  // assez proche
    return v;
}

double ComportementPrudent::accelMax() const { return 1.5; }
double ComportementPrudent::freinMax() const { return 5.0; }

// ===== ComportementVif ========================================================

double ComportementVif::vitesseDesiree(const InputsComportement& in) const {
    // Base : vitesse limite si fournie, sinon ~80 km/h
    double v = (in.vitesseLimite > 0.0) ? in.vitesseLimite : 22.2;
    // Si très proche, réduire un peu
    if (in.distanceAvant < 15.0) v *= 0.50;
    return v;
}

double ComportementVif::accelMax() const { return 3.0; }
double ComportementVif::freinMax() const { return 6.0; }
