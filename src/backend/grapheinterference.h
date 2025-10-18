#ifndef GRAPHEINTERFERENCE_H
#define GRAPHEINTERFERENCE_H

#include <vector>
#include "Vehicule.h"
#include "LienCommunication.h"

/**
 * La classe GrapheInterference gère le graphe dynamique des communications V2V.
 *
 * À chaque cycle de simulation, elle détermine quels véhicules peuvent communiquer
 * entre eux selon leur position et leur rayon de transmission.
 */
class GrapheInterference
{
private:
    // Liste des liens de communication actuel
    std::vector<LienCommunication*> liens;

    // Si un véhicule n'a pas de rayon de transmission valeur par défaut
    double rayonTransmissionDefaut;

public:
    /**
     * Constructeur par défaut
     * @param rayonDefaut Rayon de transmission par défaut (en mètres)
     */
    GrapheInterference(double rayonDefaut = 150.0);

    /**
     * Met à jour le graphe d'interférences selon la position des véhicules.
     * @param vehicules Liste actuelle des véhicules dans la simulation.
     */
    void majGraphe(const std::vector<Vehicule>& vehicules);

    /**
     * Retourne la liste actuelle des liens de communication.
     */
    std::vector<LienCommunication*> getLiens() const;

    /**
     * ATTENTION CECI EST UNE METHODE DE DEBUGAGE
     * Affiche la liste des connexions dans la console.
     */
    void afficherGraphe() const;

    /**
     * Supprime tous les liens actuels.
     */
    void clear();
};

#endif // GRAPHEINTERFERENCE_H
