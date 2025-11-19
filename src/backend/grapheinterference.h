#ifndef GRAPHEINTERFERENCE_H
#define GRAPHEINTERFERENCE_H

#include <vector>
#include <unordered_map>
#include "Vehicule.h"
#include "LienCommunication.h"

/**
 * Classe qui gère le graphe d'interférences entre véhicules.
 * Optimisée pour éviter O(n²) grâce à une grille spatiale.
 */
class GrapheInterference
{
private:
    /** Liste des liens de communication actifs */
    std::vector<LienCommunication*> liens;

    /** Rayon de transmission par défaut si non défini dans un véhicule */
    double rayonTransmissionDefaut;

    /** Cellule d'indexation spatiale */
    struct Cell {
        std::vector<size_t> vehicules; // indices des véhicules dans cette cellule
    };

    /** Grille spatiale : hash → cellule */
    std::unordered_map<long long, Cell> grille;

    /**
     * Génère un hash unique pour une cellule de la grille
     * @param cx Coordonnée x de la cellule
     * @param cy Coordonnée y de la cellule
     * @return hash unique
     */
    long long hashCell(int cx, int cy) const;

public:
    /**
     * Constructeur
     * @param rayonDefaut Rayon par défaut pour les véhicules
     */
    GrapheInterference(double rayonDefaut = 150.0);

    /**
     * Met à jour le graphe d'interférence en fonction des positions des véhicules.
     * @param vehicules Vecteur de véhicules à prendre en compte
     */
    void majGraphe(const std::vector<Vehicule>& vehicules);

    /**
     * Retourne tous les liens actifs
     * @return vecteur de pointeurs vers les liens
     */
    std::vector<LienCommunication*> getLiens() const;

    /**
     * Affiche le graphe dans la console
     */
    void afficherGraphe() const;

    /**
     * Vide le graphe et libère la mémoire des liens
     */
    void clear();
};

#endif
