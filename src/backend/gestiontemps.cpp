#include "gestiontemps.h"
#include <iostream>

GestionTemps::GestionTemps(double pasInitial)
    : tempsCourant(0.0), pasDeTemps(pasInitial), enMarche(false)
{
}

void GestionTemps::demarrer()
{
    enMarche = true;
}

void GestionTemps::arreter()
{
    enMarche = false;
}

void GestionTemps::reinitialiser()
{
    tempsCourant = 0.0;
    enMarche = false;
}

void GestionTemps::avancer()
{
    if (enMarche)
        tempsCourant += pasDeTemps;
}

double GestionTemps::getTempsCourant() const
{
    return tempsCourant;
}

void GestionTemps::setPasDeTemps(double nouveauPas)
{
    pasDeTemps = nouveauPas;
}

double GestionTemps::getPasDeTemps() const
{
    return pasDeTemps;
}

bool GestionTemps::estActif() const
{
    return enMarche;
}
