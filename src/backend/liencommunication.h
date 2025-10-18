#ifndef LIENCOMMUNICATION_H
#define LIENCOMMUNICATION_H

/**
 * Représente une connexion entre deux véhicules.
 */
class LienCommunication
{
private:
    // Id du premier véhicule
    int idVehiculeA;

    // Id du deuxième véhicule
    int idVehiculeB;

    // Distance entre les véhicules
    double distance;

public:
    /**
     * Constructeur par défaut
     * Initialise les IDs à -1 (Aucune voiture ne peux avoir un id null) et la distance à 0.0.
     */
    LienCommunication();

    /**
     * Constructeur paramétré
     * @param idA ID du premier véhicule
     * @param idB ID du second véhicule
     * @param dist Distance entre les deux véhicules (mètres)
     */
    LienCommunication(int idA, int idB, double dist);

    /**
     * Retourne l'ID du premier véhicule
     */
    int getIdVehiculeA() const;

    /**
     * Retourne l'ID du second véhicule
     */
    int getIdVehiculeB() const;

    /**
     * Retourne la distance entre les deux véhicules
     */
    double getDistance() const;

    /**
     * Vérifie si deux liens sont équivalents
     * Deux liens sont considérés égaux si les mêmes véhicules sont connectés
     * @param autre Lien à comparer
     * @return true si les deux liens sont équivalents, false sinon
     */
    bool estEgale(const LienCommunication& autre) const;

    /**
     * ATTENTION METHODE DE TEST ET DEBUGAGE
     * Affiche les informations du lien dans la console
     * Format : "Lien : Vehicule 1 <-> Vehicule 2 (distance = 120.5 m)"
     */
    void afficherInfos() const;
};

#endif // LIENCOMMUNICATION_H
