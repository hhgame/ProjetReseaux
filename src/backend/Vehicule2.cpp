#include "Vehicule.h"
#include <algorithm>
#include <cmath>
#include <limits>

Vehicule::Vehicule(int id, GrapheRoutier* g, std::unique_ptr<ComportementVehicule> comp)
    : id_(id), graphe_(g), comportement_(std::move(comp)) {}

void Vehicule::setTrajectoire(const Trajectoire& t) {
    route_ = t;
    idx_ = 0;
    s_ = 0.0;
    v_ = 0.0;
    chargerSegmentCourant();
}

bool Vehicule::chargerSegmentCourant() {
    if (!graphe_ || route_.size() < 2 || idx_+1 >= route_.size()) {
        seg_ = {-1,-1};
        segLongueur_ = 0.0;
        return false;
    }
    seg_ = { route_[idx_], route_[idx_+1] };
    const Noeud* a = graphe_->getNoeudParId(seg_.src);
    const Noeud* b = graphe_->getNoeudParId(seg_.dst);
    if (!a || !b) { seg_ = {-1,-1}; segLongueur_ = 0.0; return false; }

    A_ = { a->getLatitude(), a->getLongitude() };
    B_ = { b->getLatitude(), b->getLongitude() };
    segLongueur_ = graphe_->calculerDistance(*a, *b);
    s_ = std::clamp(s_, 0.0, segLongueur_);
    return true;
}

PositionGeo Vehicule::position() const {
    if (segLongueur_ <= 0.0) return A_;
    double t = std::clamp(s_ / segLongueur_, 0.0, 1.0);
    double lat = A_.lat + t * (B_.lat - A_.lat);
    double lon = A_.lon + t * (B_.lon - A_.lon);
    return {lat, lon};
}

bool Vehicule::update(double dt, double distanceAvant) {
    if (seg_.src == -1) return true; // plus de segment actif

    // 1) vitesse cible via stratégie
    InputsComportement in;
    in.vitesseActuelle = v_;
    in.vitesseLimite   = vLimite_;
    in.distanceAvant   = distanceAvant;
    const double vTarget = comportement_->vitesseDesiree(in);

    // 2) accélération bornée vers la cible
    const double dv = vTarget - v_;
    double a = 0.0;
    if (dv > 0.0) a = std::min(comportement_->accelMax(),  dv / std::max(1e-3, dt));
    else          a = -std::min(comportement_->freinMax(), (-dv) / std::max(1e-3, dt));
    v_ = std::max(0.0, v_ + a * dt);

    // 3) avancement
    s_ += v_ * dt;

    // 4) passage au segment suivant si fin atteinte
    if (s_ >= segLongueur_ - 1e-6) {
        idx_++;
        if (idx_ + 1 >= route_.size()) {
            seg_ = {-1,-1};
            segLongueur_ = 0.0;
            v_ = 0.0;
            return true; // terminé
        }
        s_ = 0.0;
        chargerSegmentCourant();
    }
    return false;
}
