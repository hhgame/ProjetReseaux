#ifndef GRAPHEROUTIER_H
#define GRAPHEROUTIER_H
#include <vector>
#include <unordered_map>
#include "Noeud.h"
#include "Arete.h"


class GrapheRoutier {
private:
    std::unordered_map<long, Noeud> noeuds;
    std::vector<Arete> aretes;
    bool estOriente;

public:
    GrapheRoutier(bool oriente = false);
    void ajouterNoeud(const Noeud& n);
    void ajouterArete(const Arete& a);
    Noeud* getNoeudParId(long id) const;
    void afficherResume() const;
    const std::unordered_map<long, Noeud>& getNoeuds() const;
    const std::vector<Arete>& getAretes() const;
    bool get_estOriente() const;
    std::vector<long> getVoisins(long id) const;
    int degreNoeud(long id) const;



    bool chargerDepuisOSM(const std::string& cheminFichier);
    double calculerDistance(const Noeud& n1, const Noeud& n2) const;
};
#endif // GRAPHEROUTIER_H
