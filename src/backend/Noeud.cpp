#include "Noeud.h"
#include "Arete.h"
#include <vector>

void Noeud::addArete(Arete* a) {
    aretesSuiv.push_back(a);
}


std::vector<Arete*> Noeud::getAretesSuivantes() const {
    return aretesSuiv;
}

