#ifndef COMPORTEMENTVEHICULE_H
#define COMPORTEMENTVEHICULE_H
#include <memory>

/**
 * Données d'entrée fournies au comportement à chaque tick.
 * - vitesseActuelle : vitesse du véhicule (m/s)
 * - vitesseLimite   : limite (m/s), 0 si aucune
 * - distanceAvant   : distance au véhicule juste devant sur le même segment (m),
 *                     utiliser une valeur très grande s'il n'y en a pas.
 */
struct InputsComportement {
    double vitesseActuelle{0.0};
    double vitesseLimite{0.0};
    double distanceAvant{1e12};
};

/**
 * Interface (pattern Strategy) pour la "façon de conduire".
 * Le véhicule demandera une vitesse cible et les bornes d'accélération/freinage.
 */
class ComportementVehicule {
public:
    virtual ~ComportementVehicule() = default;

    // Vitesse désirée (m/s) en fonction du contexte courant.
    virtual double vitesseDesiree(const InputsComportement& in) const = 0;

    // Accélération et freinage maximaux (m/s²).
    virtual double accelMax() const = 0;
    virtual double freinMax() const = 0;
};

/** Conduite à vitesse constante (utile pour tests et benchmarks). */
class ComportementConstant : public ComportementVehicule {
public:
    explicit ComportementConstant(double vms);

    double vitesseDesiree(const InputsComportement& in) const override;
    double accelMax() const override;
    double freinMax() const override;

private:
    double v_; // m/s
};

/** Conduite prudente (réduit la vitesse si la distanceAvant est faible). */
class ComportementPrudent : public ComportementVehicule {
public:
    ComportementPrudent() = default;

    double vitesseDesiree(const InputsComportement& in) const override;
    double accelMax() const override;
    double freinMax() const override;
};

/** Conduite plus "vive" (vitesses plus élevées, accélère/freine plus fort). */
class ComportementVif : public ComportementVehicule {
public:
    ComportementVif() = default;

    double vitesseDesiree(const InputsComportement& in) const override;
    double accelMax() const override;
    double freinMax() const override;
};
#endif // COMPORTEMENTVEHICULE_H
