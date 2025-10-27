#include "GrapheRoutier.h"
#include <QFile>
#include <QXmlStreamReader>
#include <iostream>
#include <fstream>
#include <cmath>
#include <regex>



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

/*bool GrapheRoutier::chargerDepuisOSM(const std::string& cheminFichier) {
    std::ifstream ifs(cheminFichier);
    if (!ifs) {
        std::cerr << "Erreur: impossible d’ouvrir " << cheminFichier << "\n";
        return false;
    }

    std::stringstream ss;
    ss << ifs.rdbuf();
    const std::string xml = ss.str();

    // 1️⃣ Charger les noeuds <node>
    std::regex nodeRe(
        "<node[^>]*\\bid\\s*=\\s*\"(-?\\d+)\"[^>]*\\blat\\s*=\\s*\"([-0-9.]+)\"[^>]*\\blon\\s*=\\s*\"([-0-9.]+)\"[^>]*//*>",
        std::regex::icase
        );

    for (std::sregex_iterator it(xml.begin(), xml.end(), nodeRe), end; it != end; ++it) {
        const long   id  = std::stol((*it)[1].str());
        const double lat = std::stod((*it)[2].str());
        const double lon = std::stod((*it)[3].str());

        // Filtrer par limites Mulhouse
        if (lat < MIN_LAT || lat > MAX_LAT || lon < MIN_LON || lon > MAX_LON)
            continue;

        ajouterNoeud(Noeud(id, lat, lon));
    }

    // 2️⃣ Charger les ways <way>
    std::regex wayRe(
        "<way\\b[^>]*>([\\s\\S]*?)</way>",
        std::regex::icase
        );

    std::regex ndRe(
        "<nd[^>]*\\bref\\s*=\\s*\"(-?\\d+)\"[^>]*//*?>",
        std::regex::icase
        );

    std::regex highwayTagRe(
        "<tag[^>]*\\bk\\s*=\\s*\"highway\"",
        std::regex::icase
        );*/

    /*for (std::sregex_iterator wit(xml.begin(), xml.end(), wayRe), wend; wit != wend; ++wit) {
        const std::string body = (*wit)[1].str();

        // Filtrer uniquement les routes
        if (!std::regex_search(body, highwayTagRe))
            continue;

        long prev = -1;
        for (std::sregex_iterator nit(body.begin(), body.end(), ndRe), nend; nit != nend; ++nit) {
            const long ref = std::stol((*nit)[1].str());
            const Noeud* a = getNoeudParId(prev);
            const Noeud* b = getNoeudParId(ref);

            if (prev != -1 && a && b) {
                const double d = calculerDistance(*a, *b);
                ajouterArete(Arete(prev, ref, d));
                if (!estOriente) ajouterArete(Arete(ref, prev, d));
            }

            prev = ref;
        }
    }

    afficherResume();
    return true;
}*/

bool GrapheRoutier::chargerDepuisOSM(const std::string& cheminFichier)
{
    QFile file(QString::fromStdString(cheminFichier));
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Erreur: impossible d’ouvrir " << cheminFichier << "\n";
        return false;
    }

    QXmlStreamReader xml(&file);
    std::unordered_map<long, Noeud> noeudsTmp; // nœuds temporaires pour garder seulement Mulhouse

    // 1️⃣ Lire tous les nœuds
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if (xml.isStartElement() && xml.name() == "node") {
            long id = xml.attributes().value("id").toLongLong();
            double lat = xml.attributes().value("lat").toDouble();
            double lon = xml.attributes().value("lon").toDouble();

            if (lat >= MIN_LAT && lat <= MAX_LAT && lon >= MIN_LON && lon <= MAX_LON) {
                noeudsTmp[id] = Noeud(id, lat, lon);
                ajouterNoeud(Noeud(id, lat, lon));
            }
        }
    }

    // Revenir au début pour lire les ways
    file.seek(0);
    xml.clear();
    xml.setDevice(&file);

    // 2️⃣ Lire les ways
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

            if (!isHighway) continue; // on ne garde que les routes

            long prev = -1;
            for (long ref : refs) {
                const Noeud* a = getNoeudParId(prev);
                const Noeud* b = getNoeudParId(ref);

                if (prev != -1 && a && b) {
                    double d = calculerDistance(*a, *b);
                    ajouterArete(Arete(prev, ref, d));
                    if (!estOriente) ajouterArete(Arete(ref, prev, d));
                }

                prev = ref;
            }
        }
    }

    if (xml.hasError()) {
        std::cerr << "Erreur parsing XML OSM : " << xml.errorString().toStdString() << std::endl;
        return false;
    }

    file.close();
    afficherResume();
    return true;
}



