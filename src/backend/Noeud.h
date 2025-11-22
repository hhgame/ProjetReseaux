#ifndef NOEUD_H
#define NOEUD_H
#pragma once
#include <iostream>
#include <vector>

class Arete;

/**
 * Classe représentant un noeud du graphe routier
 */
class Noeud {
private:
    /** Identifiant du noeud */
    long   id_{0};
    /** Latitude de la position du noeud */
    double lat_{0.0};
    /** Longitude de la position du noeud */
    double lon_{0.0};
    /** Tableau des aretes suivantes d'un noeud */
    std::vector<Arete> aretesSuiv;

public:
    /**
     * Constructeur par défaut
     */
    Noeud() = default;

    /**
     * Constructeur d'un noeud à partir de toutes ses données
     * @param id l'identifiant du noeud
     * @param latitude la latitude de la position du noeud
     * @param longitude la longitude de la position du noeud
     */
    Noeud(long id, double latitude, double longitude)
        : id_(id), lat_(latitude), lon_(longitude) {}

    /**
     * Récupérer l'identifiant du noeud
     * @return l'id du noeud
     */
    long   getId()        const { return id_;  }
    /**
     * Récupération de la composante latitude de la position
     * de noeud
     * @return la latitude de la position du noeud
     */
    double getLatitude()  const { return lat_; }
    /**
     * Récupération de la composante longitude de la position
     * de noeud
     * @return la longitude de la position du noeud
     */
    double getLongitude() const { return lon_; }

    /**
     * Ajout d'un arete suivante au noeud
     * @param a l'arete à rajouter
     */
    void addArete(Arete a);
    /**
     * Récupération des aretes suivantes du noeud
     * @return le tableau des aretes suivante du noeud
     */
    std::vector<Arete> getAretesSuivantes() const;

    /**
     * Affichage du statuts actuel du noeud
     */
    void afficherInfos() const {
        std::cout << "Noeud " << id_ << " (" << lat_ << ", " << lon_ << ")\n";
    }
};

#endif // NOEUD_H
