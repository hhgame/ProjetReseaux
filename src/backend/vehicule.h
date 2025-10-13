#ifndef VEHICULE_H
#define VEHICULE_H

#include <iostream>

class Vehicule {
    private:
        int id;
        double x;
        double y;
        double vitesse;
        double direction; // en degrés

    public:
        Vehicule(int id, double x = 0.0, double y = 0.0, double vitesse = 0.0, double direction = 0.0);

        // Getters
        int getId() const;
        double getX() const;
        double getY() const;
        double getVitesse() const;
        double getDirection() const;

        // Setters
        void setVitesse(double v);
        void setDirection(double dir);

        // Méthodes
        void avancer(double dt); // déplace le véhicule selon sa vitesse et direction
        void afficherEtat() const;
};

#endif // VEHICULE_H
