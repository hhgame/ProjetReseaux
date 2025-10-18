#ifndef NOEUD_H
#define NOEUD_H
#pragma once
#include <iostream>

class Noeud {
    long   id_{0};
    double lat_{0.0};
    double lon_{0.0};

public:
    // ✅ constructeur par défaut (évite les soucis avec std::map::operator[])
    Noeud() = default;

    Noeud(long id, double latitude, double longitude)
        : id_(id), lat_(latitude), lon_(longitude) {}

    long   getId()        const { return id_;  }
    double getLatitude()  const { return lat_; }
    double getLongitude() const { return lon_; }

    void afficherInfos() const {
        std::cout << "Noeud " << id_ << " (" << lat_ << ", " << lon_ << ")\n";
    }
};

#endif // NOEUD_H
