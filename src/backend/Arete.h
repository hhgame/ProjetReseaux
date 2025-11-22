#ifndef ARETE_H
#define ARETE_H
#pragma once
#include <iostream>

/**
 * Classe représentant une arete du graphe routier
 */
class Arete {

private:
    /** id du noeud source */
    long   src_{0};
    /** id du noeud destination */
    long   dst_{0};
    /** distance entre les deux noeuds en mètre */
    double dist_{0.0};

public:
    /**
     * Constructeur par défaut
     */
    Arete() = default;

    /**
     * Constructeur à partir de toutes les données
     * @param idSource id du noeud source
     * @param idDestination id du noeud de destination
     * @param distance la distance entre les deux noeuds
     */
    Arete(long idSource, long idDestination, double distance)
        : src_(idSource), dst_(idDestination), dist_(distance) {}

    /**
     * Récupérer l'id du noeud source
     * @return l'id du noeud source de l'arête
     */
    long   getIdSource()      const { return src_;  }

    /**
     * Récupérer l'id du noeud de destination
     * @return l'id du noeud destination de l'arête
     */
    long   getIdDestination() const { return dst_;  }

    /**
     * Récupérer la distance entre les deux noeuds
     * @return distance entre les deux noeuds en mètres
     */
    double getDistance()      const { return dist_; }

    /**
     * Méthode d'affichage du status du noeud sur la console
     */
    void afficherInfos() const {
        std::cout << "Arete " << src_ << " -> " << dst_
                  << "  (" << dist_ << " m)\n";
    }
};

#endif // ARETE_H
