#ifndef PARAMWINDOW_H
#define PARAMWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QFormLayout>
#include "../backend/simulateur.h"

/**
 * Classe représentant la fenêtre des paramètres de simulation
 */
class ParamWindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructeur de la fenêtre de paramétrage
     * @param sim le pointeur sur le simulateur
     * @param parent le QWidget parent (nécessaire à Qt) de base à nullptr
     */
    explicit ParamWindow(Simulateur *sim, QWidget *parent = nullptr);

signals:
    /**
     * Signal de fermeture de la fenêtre de paramétrage
     */
    void closed();

private slots:
    /**
     * Validation des paramètres entrées
     */
    void onValidate();
    /**
     * Annulation de la saisie des paramètres
     */
    void onCancel();

private:
    /** Pointeur sur la simulateur */
    Simulateur *simulation;

    /** Composant de saisie du nombre de véhicules */
    QSpinBox *vehiculesBox;
    /** Composant de saisie du rayon de transmission */
    QSpinBox *rayonBox;
    /** Composant choix du rayon aléatoire ou non */
    QCheckBox *rayonAleaBox;
    /** Composant de choix d'affichage du rayon de transmission */
    QCheckBox *afficheRayonBox;
    /** Composant de choix d'affichage du graphe des interférences */
    QCheckBox *afficheGrapheBox;

    /** Bouton de validation */
    QPushButton *validateButton;
    /** Bouton d'annulation */
    QPushButton *cancelButton;
};

#endif // PARAMWINDOW_H

