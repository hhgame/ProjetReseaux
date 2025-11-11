#include "mainwindow.h"

MainWindow::MainWindow(Simulateur* simulateur, QWidget *parent)
    : QMainWindow(parent), s(simulateur)
{
    setupUI();
}

MainWindow::~MainWindow()
{
    delete s;
}

// Slots
void MainWindow::ouvrirParametres()
{
    if (paramWindow) {
        paramWindow->show();
        paramWindow->raise();
        paramWindow->activateWindow();
    }
}

void MainWindow::miseAJourVitesse(double facteur)
{
    if (s) s->setFacteurVitesse(facteur);
}

// Initialisation UI
void MainWindow::setupUI()
{
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* layout = new QVBoxLayout(central);

    toolbar = new ToolBar(this);
    layout->addWidget(toolbar);

    connect(toolbar, &ToolBar::speedChanged, this, &MainWindow::miseAJourVitesse);
    connect(toolbar, &ToolBar::openParamWindow, this, &MainWindow::ouvrirParametres);

    mapWidget = new MapWidgetOSM(this, s);
    mapWidget->setCentre(47.75, 7.34);
    mapWidget->setZoom(13);
    layout->addWidget(mapWidget);

    paramWindow = new ParamWindow(s, nullptr);

    resize(800, 600);
}
