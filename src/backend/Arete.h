#ifndef ARETE_H
#define ARETE_H
#pragma once
#include <iostream>

class Arete {
    long   src_{0};
    long   dst_{0};
    double dist_{0.0}; // en mètres

public:
    Arete() = default; // par sécurité
    Arete(long idSource, long idDestination, double distance)
        : src_(idSource), dst_(idDestination), dist_(distance) {}

    long   getIdSource()      const { return src_;  }
    long   getIdDestination() const { return dst_;  }
    double getDistance()      const { return dist_; }

    void afficherInfos() const {
        std::cout << "Arete " << src_ << " -> " << dst_
                  << "  (" << dist_ << " m)\n";
    }
};

#endif // ARETE_H
