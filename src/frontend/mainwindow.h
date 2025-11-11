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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Simulateur* simulateur, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void ouvrirParametres();
    void miseAJourVitesse(double facteur);

private:
    void setupUI();

    Simulateur* s;
    MapWidgetOSM* mapWidget;
    ToolBar* toolbar;
    ParamWindow* paramWindow;
};

#endif // MAINWINDOW_H
