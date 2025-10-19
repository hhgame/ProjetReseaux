#ifndef GESTIONTEMPS_H
#define GESTIONTEMPS_H

class GestionTemps
{
    private:
        double tempsCourant;  // Temps simulé écoulé
        double pasDeTemps;    // Pas de temps (en secondes)
        bool enMarche;        // Indique si la simulation tourne

    public:
        GestionTemps(double pasInitial = 1.0);

        void demarrer();
        void arreter();
        void reinitialiser();
        void avancer();
        double getTempsCourant() const;
        void setPasDeTemps(double nouveauPas);
        double getPasDeTemps() const;
        bool estActif() const;

};

#endif // GESTIONTEMPS_H
