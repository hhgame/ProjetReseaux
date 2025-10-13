#include "vehicule.h"
#include <cmath>

Vehicule::Vehicule(int id, double x, double y, double vitesse, double direction)
    : id{id}, x{x}, y{y}, vitesse{vitesse}, direction{direction}
{}

int Vehicule::getId() const
{
    return id;
}

double Vehicule::getX() const
{
    return x;
}

double Vehicule::getY() const
{
    return y;
}

double Vehicule::getVitesse() const
{
    return vitesse;
}

double Vehicule::getDirection() const
{
    return direction;
}

void Vehicule::setVitesse(double v)
{
    vitesse = v;
}

void Vehicule::setDirection(double dir)
{
    direction = dir;
}

void Vehicule::avancer(double dt) {
    double rad = direction * M_PI / 180.0;
    x += vitesse * std::cos(rad) * dt;
    y += vitesse * std::sin(rad) * dt;

    // Corriger les petites erreurs flottantes
    const double epsilon = 1e-10; // seuil
    if (std::abs(x) < epsilon) x = 0.0;
    if (std::abs(y) < epsilon) y = 0.0;
}

void Vehicule::afficherEtat() const {
    std::cout << "Vehicule " << id
              << " | Position: (" << x << ", " << y << ")"
              << " | Vitesse: " << vitesse
              << " | Direction: " << direction << " degres"
              << std::endl;
}
