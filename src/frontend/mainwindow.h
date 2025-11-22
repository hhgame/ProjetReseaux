#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "../backend/simulateur.h"
#include "mapwidgetosm.h"
#include "toolbar.h"
#include "paramwindow.h"

/**
 * Classe représentant la fenêtre principale
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * Constructeur de la fenêtre principale
     * @param simulateur le pointeur sur le simulateur
     * @param parent le QWidget parent (nécessaire à Qt) de base à nullptr
     */
    MainWindow(Simulateur* simulateur, QWidget *parent = nullptr);
    /**
     * Destructeur de la fenêtre principale
     */
    ~MainWindow();

private slots:
    /**
     * Ouverture de la fenêtre des paramètres
     */
    void ouvrirParametres();
    /**
     * Mise-à-jour du facteur vitesse
     * @param facteur le nouveau facteur vitesse
     */
    void miseAJourVitesse(double facteur);

private:
    /**
     * Mise en place des composants dans la fenêtre principale
     */
    void setupUI();

    /** Pointeur sur le simulateur en cours d'execution */
    Simulateur* s;
    /** Widget de la carte avec les véhicules */
    MapWidgetOSM* mapWidget;
    /** Barre des paramètres en accès direct */
    ToolBar* toolbar;
    /** Ecran de paramétrage */
    ParamWindow* paramWindow;
};

#endif // MAINWINDOW_H
