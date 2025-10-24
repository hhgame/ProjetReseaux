#include "Collision.h"
#include <algorithm>
#include <cmath>

// Haversine local (ici pour ne pas dépendre du graphe)
static double haversine(double lat1, double lon1, double lat2, double lon2) {
    constexpr double R = 6371000.0;
    auto rad = [](double d){ return d * M_PI / 180.0; };
    double p1 = rad(lat1), p2 = rad(lat2);
    double dlat = p2 - p1;
    double dlon = rad(lon2 - lon1);
    double a = sin(dlat/2)*sin(dlat/2) + cos(p1)*cos(p2)*sin(dlon/2)*sin(dlon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    return R * c;
}

double distanceEntre(const Vehicule& a, const Vehicule& b, const GrapheRoutier&) {
    PositionGeo pa = a.position();
    PositionGeo pb = b.position();
    return haversine(pa.lat, pa.lon, pb.lat, pb.lon);
}

void resoudreCollisionsMemeSegment(std::vector<Vehicule*>& vehicules,
                                   double minGap,
                                   double margeCroisement)
{
    // Implémentation volontairement légère (O(n^2)) : OK pour un TP / 2000 véhicules avec
    // une optimisation légère côté simulateur (bucket par segment).
    for (size_t i = 0; i < vehicules.size(); ++i) {
        Vehicule* vi = vehicules[i];
        auto si = vi->segment();
        if (si.src < 0) continue;

        for (size_t j = i+1; j < vehicules.size(); ++j) {
            Vehicule* vj = vehicules[j];
            auto sj = vj->segment();
            if (sj.src < 0) continue;

            // Cas 1 : même segment, même sens
            if (si == sj) {
                double di = vi->progression();
                double dj = vj->progression();

                // On identifie le leader (plus avancé) et le suiveur
                Vehicule* leader  = (di >= dj) ? vi : vj;
                Vehicule* suiveur = (di >= dj) ? vj : vi;
                double dLead   = std::max(di, dj);
                double dFollow = std::min(di, dj);

                // Si l’écart < minGap, on *devrait* freiner le suiveur :
                // Dans ce design, c’est le Simulateur qui passe "distanceAvant"
                // faible au suiveur pour que son Comportement réduise v.
                (void)leader; (void)suiveur; (void)dLead; (void)dFollow; (void)minGap;
            }

            // Cas 2 : même segment mais sens inverses (croisement frontal)
            if (si.inverseDe(sj)) {
                double sA = vi->progression();
                double sB = vj->progression();
                double L  = vi->longueurSegment(); // supposé identique pour les deux

                // Si sA + sB > L - margeCroisement -> risque de croisement
                // Là aussi, on laissera le Simulateur réduire "distanceAvant"
                // des deux véhicules pour provoquer un freinage.
                (void)sA; (void)sB; (void)L; (void)margeCroisement;
            }
        }
    }
}
