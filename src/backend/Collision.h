#ifndef COLLISION_H
#define COLLISION_H
#pragma once
#include <vector>
#include <limits>
#include "Vehicule.h"

/*
 * distanceEntre(a,b) : distance géodésique (m) entre 2 véhicules via Haversine.
 * Sert pour l’affichage / debug / interférences (rayon de communication).
 */
double distanceEntre(const Vehicule& a, const Vehicule& b, const GrapheRoutier& g);

/*
 * resoudreCollisionsMemeSegment :
 *  - même segment, même sens : on maintient une "headway" (minGap)
 *  - même segment, sens opposé: on évite le croisement en s’arrêtant avant (margeCroisement)
 *
 * NOTE : Ici on ne modifie pas directement la vitesse s’il y a conflit ;
 *        la logique recommandée est que le Simulateur calcule "distanceAvant"
 *        (distance au leader) et la passe à Vehicule::update() pour que le
 *        comportement décide de ralentir. Cette fonction est un utilitaire
 *        si tu veux ajouter des règles supplémentaires ou détecter des cas.
 */
void resoudreCollisionsMemeSegment(std::vector<Vehicule*>& vehicules,
                                   double minGap /*m*/,
                                   double margeCroisement /*m*/);

#endif // COLLISION_H
