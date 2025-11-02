#include "mainwindow.h"
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Créer un widget central
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    // Créer le simulateur
    s = new Simulateur{};

    // Layout vertical
    QVBoxLayout* layout = new QVBoxLayout(central);

    // Créer la toolbar et connecter ses signaux
    toolbar = new ToolBar(this);
    layout->addWidget(toolbar);

    connect(toolbar, &ToolBar::speedChanged, this, &MainWindow::miseAJourVitesse);
    connect(toolbar, &ToolBar::openParamWindow, this, &MainWindow::ouvrirParametres);

    // Créer le widget OSM
    mapWidget = new MapWidgetOSM(this, s);
    mapWidget->setCentre(47.75, 7.34); // Mulhouse
    mapWidget->setZoom(13);
    layout->addWidget(mapWidget);

    paramWindow = new ParamWindow(s, nullptr);

    resize(800, 600);
    show();
}

MainWindow::~MainWindow()
{
    delete s;
}

// ===== Slots =====
void MainWindow::ouvrirParametres()
{
    // Affiche la fenêtre ParamWindow
    if (paramWindow) {
        paramWindow->show();
        paramWindow->raise();
        paramWindow->activateWindow();
    }
}

void MainWindow::miseAJourVitesse(double facteur)
{
    if (s) {
        s->setFacteurVitesse(facteur);
    }
}
