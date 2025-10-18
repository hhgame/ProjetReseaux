#include "GrapheRoutier.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <regex>



using namespace std;


GrapheRoutier::GrapheRoutier(bool oriente) : estOriente(oriente) {}

void GrapheRoutier::ajouterNoeud(const Noeud& n) {
    noeuds[n.getId()] = n;
}

void GrapheRoutier::ajouterArete(const Arete& a) {
    aretes.push_back(a);
}

Noeud* GrapheRoutier::getNoeudParId(long id) {
    if (noeuds.find(id) != noeuds.end()) return &noeuds[id];
    return nullptr;
}

void GrapheRoutier::afficherResume() const {
    cout << "Resume du graphe routier :" << endl;
    cout << "  Noeuds : " << noeuds.size() << endl;
    cout << "  Aretes : " << aretes.size() << endl;
    cout << "  Orientation : " << (estOriente ? "Oui" : "Non") << endl;
}

// Distance géographique (formule de Haversine)
double GrapheRoutier::calculerDistance(const Noeud& n1, const Noeud& n2) const {
    const double R = 6371000.0;
    double lat1 = n1.getLatitude() * M_PI / 180.0;
    double lat2 = n2.getLatitude() * M_PI / 180.0;
    double dLat = (lat2 - lat1);
    double dLon = (n2.getLongitude() - n1.getLongitude()) * M_PI / 180.0;

    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(lat1) * cos(lat2) *
                   sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return R * c;
}

bool GrapheRoutier::chargerDepuisOSM(const std::string& cheminFichier) {
    std::ifstream ifs(cheminFichier);
    if (!ifs) {
        std::cerr << "Erreur: impossible d’ouvrir " << cheminFichier << "\n";
        return false;
    }

    std::stringstream ss;
    ss << ifs.rdbuf();
    const std::string xml = ss.str();

    // 1) Noeuds : <node id="..." lat="..." lon="..."/>
    // Note: toutes les séquences \ sont échappées (\\b, \\s, \\" etc.)
    std::regex nodeRe(
        "<node[^>]*\\bid\\s*=\\s*\"(-?\\d+)\"[^>]*\\blat\\s*=\\s*\"([-0-9.]+)\"[^>]*\\blon\\s*=\\s*\"([-0-9.]+)\"[^>]*/?>",
        std::regex::icase
        );

    for (std::sregex_iterator it(xml.begin(), xml.end(), nodeRe), end; it != end; ++it) {
        const long   id  = std::stol((*it)[1].str());
        const double lat = std::stod((*it)[2].str());
        const double lon = std::stod((*it)[3].str());
        ajouterNoeud(Noeud(id, lat, lon));
    }

    // 2) Extraire toutes les <way>...</way> (match multi-lignes via [\\s\\S])
    std::regex wayRe(
        "<way\\b[^>]*>([\\s\\S]*?)</way>",
        std::regex::icase
        );

    // 3) Dans chaque way, relier les nd successifs : <nd ref="..."/>
    std::regex ndRe(
        "<nd[^>]*\\bref\\s*=\\s*\"(-?\\d+)\"[^>]*/?>",
        std::regex::icase
        );

    for (std::sregex_iterator wit(xml.begin(), xml.end(), wayRe), wend; wit != wend; ++wit) {
        const std::string body = (*wit)[1].str();

        // (optionnel) : filtrer uniquement les vraies routes OSM
        // if (!std::regex_search(body, std::regex("<tag[^>]*\\bk\\s*=\\s*\"highway\"", std::regex::icase)))
        //     continue;

        long prev = -1;
        for (std::sregex_iterator nit(body.begin(), body.end(), ndRe), nend; nit != nend; ++nit) {
            const long ref = std::stol((*nit)[1].str());
            if (prev != -1) {
                const Noeud* a = getNoeudParId(prev);
                const Noeud* b = getNoeudParId(ref);
                if (a && b) {
                    const double d = calculerDistance(*a, *b);
                    ajouterArete(Arete(prev, ref, d));
                    if (!estOriente) ajouterArete(Arete(ref, prev, d));
                }
            }
            prev = ref;
        }
    }

    afficherResume();
    return true;
}


