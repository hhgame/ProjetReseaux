#ifndef VEHICULE_H
#define VEHICULE_H

const int MIN = 100;
const int MAX = 500;

class Noeud;
class GrapheRoutier;

/**
 * Classe représentant un véhicule
 */
class Vehicule {
    private:
        /** Identifiant du véhicule */
        int id;
        /** La composante x de la position du véhicule */
        double x;
        /** La composante y de la position du véhicule */
        double y;
        /** Vitesse du véhicule */
        double vitesse;
        /** Direction du véhicule (en degré) */
        double direction;
        /** Rayon de transmission du véhicule */
        int rayonTransmission;

        /** L'identifiant du noeud sur lequel la voiture se trouve */
        long noeudActuelId = -1;
        /** L'identifiant du noeud de destination sur lequel la voiture se rend */
        long noeudDestinationId = -1;
        /** Position de la voiture sur l'arete */
        double positionSurArete = 0.0;
        /** Identifiant du dernier noeud */
        long dernierNoeudId = -1;


    public:
        /**
         * Constructeur sans rayon de transmission du véhicule
         * @param id l'identifiant du véhicule
         * @param x la composante x de la position du véhicule
         * @param y la composante y de la position du véhicule
         * @param vitesse la vitesse du véhicule
         * @param direction la direction du véhicule
         */
        Vehicule(int id, double x = 0.0, double y = 0.0, double vitesse = 0.0, double direction = 0.0);
        /**
         * Constructeur avc toutes les données du véhicule
         * @param id l'identifiant du véhicule
         * @param rayonTrans le rayon de transmission
         * @param x la composante x de la position du véhicule
         * @param y la composante y de la position du véhicule
         * @param vitesse la vitesse du véhicule
         * @param direction la direction du véhicule
         */
        Vehicule(int id, int rayonTrans, double x = 0.0, double y = 0.0, double vitesse = 0.0, double direction = 0.0);

        /**
         * Récupération de l'identifiant du véhicule
         * @return l'identifiant du véhicule
         */
        int getId() const;
        /**
         * Récupération de la composante x de la position du véhicule
         * @return la composante x de la position du véhicule
         */
        double getX() const;
        /**
         * Récupération de la composante y de la position du véhicule
         * @return la composante y de la position du véhicule
         */
        double getY() const;
        /**
         * Récupération de la vitesse du véhicule
         * @return la vitesse du véhicule
         */
        double getVitesse() const;
        /**
         * Récupération de la direction du véhicule
         * @return la direction du véhicule en degré
         */
        double getDirection() const;
        /**
         * Récupération du rayon de transmission du véhicule
         * @return le rayon de transmission du véhicule
         */
        int getRayonTransmission() const;

        /**
         * Changement de la vitesse du véhicule
         * @param v la nouvelle vitesse
         */
        void setVitesse(double v);
        /**
         * Changement de la direction du véhicule
         * @param dir la nouvelle direction
         */
        void setDirection(double dir);

        /**
         * Faire avancer le véhicule sur la graphe routier
         * @param dt le pas de temps
         * @param graphe le graphe routier
         */
        void avancerSurGraphe(double dt, const GrapheRoutier& graphe);
        /**
         * Changement du noeud de départ
         * @param id l'identifian du nouveau noeud de départ
         */
        void setNoeudDepart(long id);
        /**
         * Changement du rayon de transmission
         * @param r le nouveau rayon de transmission
         */
        void setRayonTransmission(int r);


        /**
         * Afficher l'état actuel du véhicule sur la console
         */
        void afficherEtat() const;

        /**
         * Changement du dernier noeud
         * @param id l'identifiant du nouveau dernier noeud
         */
        void setDernierNoeudId(long id) { dernierNoeudId = id; }
        /**
         * Récupération du dernier noeud
         * @return l'identifiant du dernier noeud
         */
        long getDernierNoeudId() const { return dernierNoeudId; }


};

#endif // VEHICULE_H
