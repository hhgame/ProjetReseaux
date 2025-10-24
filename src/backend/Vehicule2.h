#ifndef VEHICULE2_H
#define VEHICULE2_H
#pragma once
#include <vector>
#include <memory>
#include <utility>
#include "ComportementVehicule.h"
#include "GrapheRoutier.h"

/* Position géographique (interpolation entre les extrémités de l'arête). */
struct PositionGeo {
    double lat{0.0};
    double lon{0.0};
};

/* Identifie un segment (arête) du graphe : src -> dst. */
struct SegmentId {
    long src{-1};
    long dst{-1};
    bool operator==(const SegmentId& o) const { return src==o.src && dst==o.dst; }
    bool inverseDe(const SegmentId& o) const { return src==o.dst && dst==o.src; }
};

/*
 * Vehicule : suit une trajectoire (suite d'IDs de noeuds) sur le GrapheRoutier.
 * La vitesse cible vient d'un ComportementVehicule (Strategy).
 */
class Vehicule {
public:
    using Trajectoire = std::vector<long>; // ex. [n1, n2, n3, ...]

private:
    int id_;
    GrapheRoutier* graphe_;
    std::unique_ptr<ComportementVehicule> comportement_;

    Trajectoire route_;
    size_t idx_{0};          // on suit route_[idx_] -> route_[idx_+1]
    SegmentId seg_{-1,-1};

    double segLongueur_{0.0}; // m
    double s_{0.0};           // m parcourus sur le segment courant
    double v_{0.0};           // m/s
    double vLimite_{0.0};     // m/s (0 si aucune)

    PositionGeo A_{}, B_{};

public:
    // ❗ signature modifiée : le graphe est passé en pointeur non-const
    Vehicule(int id, GrapheRoutier* g, std::unique_ptr<ComportementVehicule> comp);

    // Accesseurs
    int       id() const { return id_; }
    SegmentId segment() const { return seg_; }
    double    progression() const { return s_; }
    double    longueurSegment() const { return segLongueur_; }
    double    vitesse() const { return v_; }
    void      setVitesseLimite(double v) { vLimite_ = v; }

    // Trajectoire : au moins 2 noeuds
    void setTrajectoire(const Trajectoire& t);

    // Position géographique (lat/lon) par interpolation
    PositionGeo position() const;

    // Mise à jour (tick) — distanceAvant en m (ou très grand si aucun véhicule devant)
    // Retourne true si la trajectoire est terminée.
    bool update(double dt, double distanceAvant);

private:
    // Recharge A_, B_, segLongueur_ depuis le graphe pour le segment courant
    bool chargerSegmentCourant();
};

#endif // VEHICULE_H
