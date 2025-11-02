#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mapwidgetosm.h"
#include "toolbar.h"
#include "paramwindow.h"
#include "../backend/simulateur.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void ouvrirParametres();
    void miseAJourVitesse(double facteur);

private:
    Simulateur* s;
    MapWidgetOSM* mapWidget;
    ToolBar* toolbar;
    ParamWindow* paramWindow;
};

#endif // MAINWINDOW_H
