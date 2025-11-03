#include "GrapheRoutier.h"
#include <QFile>
#include <QXmlStreamReader>
#include <iostream>
#include <fstream>
#include <cmath>
#include <regex>
#include <algorithm>
#include <unordered_set>



using namespace std;

// Limites Mulhouse
const double MIN_LAT = 47.66;
const double MAX_LAT = 47.84;
const double MIN_LON = 7.207;
const double MAX_LON = 7.473;


GrapheRoutier::GrapheRoutier(bool oriente) : estOriente(oriente) {}

void GrapheRoutier::ajouterNoeud(const Noeud& n) {
    noeuds[n.getId()] = n;
}

void GrapheRoutier::ajouterArete(const Arete& a) {
    aretes.push_back(a);
}

Noeud* GrapheRoutier::getNoeudParId(long id) const {
    auto it = noeuds.find(id);
    if (it != noeuds.end())
        return const_cast<Noeud*>(&it->second); // pointeur non-const si tu veux modifier le noeud
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


bool GrapheRoutier::chargerDepuisOSM(const std::string& cheminFichier)
{
    QFile file(QString::fromStdString(cheminFichier));
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Erreur: impossible d’ouvrir " << cheminFichier << "\n";
        return false;
    }

    QXmlStreamReader xml(&file);

    std::unordered_map<long, Noeud> noeudsTmp; // nœuds valides dans la zone
    std::unordered_set<long> noeudsUtilises;   // nœuds effectivement reliés à une route

    // 1️⃣ Lire TOUS les nœuds, mais ne pas les ajouter encore
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if (xml.isStartElement() && xml.name() == "node") {
            long id = xml.attributes().value("id").toLongLong();
            double lat = xml.attributes().value("lat").toDouble();
            double lon = xml.attributes().value("lon").toDouble();

            if (lat >= MIN_LAT && lat <= MAX_LAT && lon >= MIN_LON && lon <= MAX_LON) {
                noeudsTmp[id] = Noeud(id, lat, lon);
            }
        }
    }

    // Revenir au début pour lire les ways
    file.seek(0);
    xml.clear();
    xml.setDevice(&file);

    // 2️⃣ Lire les ways et construire les arêtes uniquement pour les routes
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();

        if (xml.isStartElement() && xml.name() == "way") {
            std::vector<long> refs;
            bool isHighway = false;

            while (!(xml.isEndElement() && xml.name() == "way")) {
                xml.readNext();
                if (xml.isStartElement()) {
                    if (xml.name() == "nd") {
                        long ref = xml.attributes().value("ref").toLongLong();
                        refs.push_back(ref);
                    } else if (xml.name() == "tag") {
                        QString key = xml.attributes().value("k").toString();
                        if (key == "highway") {
                            isHighway = true;
                        }
                    }
                }
            }

            if (!isHighway) continue;

            long prev = -1;
            for (long ref : refs) {
                if (noeudsTmp.find(ref) == noeudsTmp.end()) continue; // hors zone

                if (prev != -1 && noeudsTmp.count(prev) && noeudsTmp.count(ref)) {
                    const Noeud& a = noeudsTmp[prev];
                    const Noeud& b = noeudsTmp[ref];

                    double d = calculerDistance(a, b);
                    ajouterArete(Arete(prev, ref, d));
                    if (!estOriente)
                        ajouterArete(Arete(ref, prev, d));

                    noeudsUtilises.insert(prev);
                    noeudsUtilises.insert(ref);
                }
                prev = ref;
            }
        }
    }

    // 3️⃣ Maintenant, ajouter uniquement les nœuds utilisés
    for (long id : noeudsUtilises) {
        ajouterNoeud(noeudsTmp.at(id));
    }

    if (xml.hasError()) {
        std::cerr << "Erreur parsing XML OSM : " << xml.errorString().toStdString() << std::endl;
        return false;
    }

    file.close();

    afficherResume();
    std::cout << "✅ " << noeudsUtilises.size() << " nœuds routiers chargés (isolés exclus)." << std::endl;
    return true;
}


const std::unordered_map<long, Noeud>& GrapheRoutier::getNoeuds() const {
    return noeuds;
}

const std::vector<Arete>& GrapheRoutier::getAretes() const {
    return aretes;
}

bool GrapheRoutier::get_estOriente() const {
    return estOriente;
}

std::vector<long> GrapheRoutier::getVoisins(long id) const {
    std::vector<long> voisins;
    for (const auto& a : aretes) {
        if (a.getIdSource() == id) {
            voisins.push_back(a.getIdDestination());
        } else if (a.getIdDestination() == id) {
            voisins.push_back(a.getIdSource());
        }
    }
    // éliminer doublons éventuels
    std::sort(voisins.begin(), voisins.end());
    voisins.erase(std::unique(voisins.begin(), voisins.end()), voisins.end());
    return voisins;
}

int GrapheRoutier::degreNoeud(long id) const {
    int c = 0;
    for (const auto& a : aretes) {
        if (a.getIdSource() == id || a.getIdDestination() == id) ++c;
    }
    return c;
}

