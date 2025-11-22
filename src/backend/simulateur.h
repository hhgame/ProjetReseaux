#ifndef SIMULATEUR_H
#define SIMULATEUR_H

#include <vector>
#include "vehicule.h"
#include "grapheinterference.h"
#include "GrapheRoutier.h"
#include "liencommunication.h"

/**
 * Classe représentant le simulateur et les calculs du simulateur
 */
class Simulateur {
private:
    /** Tableau des véhicules */
    std::vector<Vehicule> vehicules;

    /** Données de simulations */
    bool rayonTransmissionAleatoire = true;
    int rayonTransmission = 100;
    int nbVehicules = 100;
    bool afficherRayonTransmission = true;
    bool afficherGrapheInterference = true;

    /** Graphe des interferences */
    GrapheInterference graphe;
    /** Graphe routier */
    GrapheRoutier routes;

    /** Pré-calcul des IDs de nœuds pour tirage aléatoire */
    std::vector<long> vecteurNoeuds;

    /** Réglage du temps et de la vitesse de simulation */
    double tempsEcoule = 0.0;
    double pasDeTemps = 0.5;
    double facteurVitesse = 0.25;


public:
    /**
     * Constructeur par défaut
     */
    Simulateur();

    /**
     * Ajout des nbVehicules véhicules aléatoirement sur le graphe routier
     * @param vitesseMin la vitesse minimale possible des voitures
     * @param vitesseMax la vitesse maximale possible des voitures
     */
    void ajouterVehiculesAleatoires(double vitesseMin = 5.0, double vitesseMax = 20.0);
    /**
     * Mise-à-jour des données de simulation à chaque tour
     */
    void update();
    /**
     * Affichage de l'état de la simulation actuelle
     */
    void afficherEtat() const;
    /**
     * Réinitialisation de la simulation
     */
    void reinitialiser();


    /**
     * Récupération du nombre de véhicules
     * @return le nombre de véhicules actuel
     */
    int getNombreVehicules() const;
    /**
     * Récupération de tous les véhicule actuel
     * @return le tableau des véhicules actuels
     */
    const std::vector<Vehicule>& getVehicules() const;

    /**
     * Est ce que le rayon de transmission est afficher
     * @return true si il est affiché ou false sinon
     */
    bool getAfficheRayonTransmission() const;
    /**
     * Est ce que le graphe des interférences est affiché
     * @return true si il est affiché ou false sinon
     */
    bool getAfficheGrapheInterference() const;
    /**
     * Récupération de tous les liens de communication entre
     * les différents véhicules
     * @return le tableau des liens de communication entre les véhicules
     */
    std::vector<LienCommunication*> getLiens() const;

    /**
     * Récupération du facteur vitesse
     * @return le facteur vitesse
     */
    double getFacteurVitesse() const;
    /**
     * Changement du facteur vitesse
     * @param facteur le nouveau facteur vitesse
     */
    void setFacteurVitesse(double facteur);
    /**
     * Récupération du pas de temps
     * @return le pas de temps
     */
    double getPasDeTemps() const;
    /**
     * Changement du pas de temps
     * @param dt le nouveau pas de temps
     */
    void setPasDeTemps(double dt);

    /**
     * Récupération du rayon aleatoire
     * @return le rayon aleatoire
     */
    bool getRayonAleatoire() const;
    /**
     * Changement du rayon aleatoire
     * @param v le nouveau rayon aleatoire
     */
    void setRayonAleatoire(bool v);

    /**
     * Afficher le rayon de transmission du graphe
     * @param v true si on affiche le rayon de transmission, false sinon
     */
    void setAfficheRayonTransmission(bool v);
    /**
     * Afficher le graphe des interférences
     * @param v true si on affiche le graphe des interférences, false sinon
     */
    void setAfficheGrapheInterference(bool v);

    /**
     * Recalculer le graphe des interférences
     */
    void recalculerGrapheInterference();
    /**
     * Vider le graphe des interférences
     */
    void clearGrapheInterference();

    /**
     * Récupération du nombre de véhicules
     * @return le nombre de véhicules
     */
    int getNbVehicules() const;
    /**
     * Changement du nombre de véhicules
     * @param n le nouveau nombre de véhicules
     */
    void setNbVehicules(int n);
    /**
     * Récupération du rayon de transmission
     * @return le rayon de transmission
     */
    int getRayonTransmission() const;
    /**
     * Changement du rayon de transmission
     * @param r le nouveau rayon de transmission
     */
    void setRayonTransmission(int r);

    /**
     * Misa à jour du nombre de véhicules
     * @param nouveauNb le nouveau nombre de véhicule
     */
    void mettreAJourNbVehicules(int nouveauNb);
};

#endif // SIMULATEUR_H
