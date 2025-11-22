#ifndef GRAPHEROUTIER_H
#define GRAPHEROUTIER_H
#include <vector>
#include <unordered_map>
#include "Noeud.h"
#include "Arete.h"

/**
 * Classe représentant le graphe de toutes les routes présentantes
 * et disponible actuelle dans la zone définie (Haut-Rhin)
 */
class GrapheRoutier {
private:
    /** Tableau désordonné des noeuds (intersections) du graphe */
    std::unordered_map<long, Noeud> noeuds;
    /** Tableau des aretes (routes) du graphe */
    std::vector<Arete> aretes;
    /** Pour savoir si le graphe est orienté */
    bool estOriente;

public:
    /**
     * Constructeur de graphe routier à partir de son orientation
     * @param oriente si le graphe est orienté true sinon false
     */
    GrapheRoutier(bool oriente = false);
    /**
     * Ajout d'un noeud au graphe routier
     * @param n le noeud à ajouter
     */
    void ajouterNoeud(const Noeud& n);
    /**
     * Ajout d'une arete au graphe routier
     * @param a l'arete à rajouter
     */
    void ajouterArete(const Arete& a);
    /**
     * Récupération du noeud à partir de son id
     * @param id l'identifiant du noeud à récupérer
     * @return un pointeur sur le noeud d'identifiant id
     */
    Noeud* getNoeudParId(long id) const;
    /**
     * Affichage de l'états du graphe actuel
     */
    void afficherResume() const;
    /**
     * Récupération du tableau de tous les noeuds
     * @return le tableau désordonné de tous les noeuds possibles
     */
    const std::unordered_map<long, Noeud>& getNoeuds() const;
    /**
     * Récupération de toutes les aretes du graphe
     * @return le tableau des aretes du graphe
     */
    const std::vector<Arete>& getAretes() const;
    /**
     * Savoir si le graphe est orienté
     * @return true si le graphe est orienté sinon false
     */
    bool get_estOriente() const;
    /**
     * Récupération des voisins du noeud d'identifiant id
     * @param id l'identifiant du noeud
     * @return le tableau des voisin du noeud d'identifiant id
     */
    std::vector<long> getVoisins(long id) const;
    /**
     * Récupération du nombre de voisin du noeud
     * @param id l'identifiant du noeud
     * @return le nombre de voisins du noeud
     */
    int degreNoeud(long id) const;


    /**
     * Chargement du graphe routier depuis un fichier xml format OpenStreetMap
     * @param cheminFichier vers le fichiers xml
     * @return true si le chargement a marché, false sinon
     */
    bool chargerDepuisOSM(const std::string& cheminFichier);
    /**
     * Calcul de la distance entre deux noeuds
     * @param n1 le premier noeud
     * @param n2 le second noeud
     * @return la distance entre les deux noeuds n1 et n2
     */
    double calculerDistance(const Noeud& n1, const Noeud& n2) const;
};
#endif // GRAPHEROUTIER_H
