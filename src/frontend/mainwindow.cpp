#include "mainwindow.h"
#include <QVBoxLayout>
#include <QPixmap>
#include "../backend/simulateur.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Créer un widget central
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    s = new Simulateur{};

    // Layout vertical
    QVBoxLayout* layout = new QVBoxLayout(central);

    // Créer le widget OSM
    mapWidget = new MapWidgetOSM(this);

    // Définir le centre sur Mulhouse et zoom initial
    mapWidget->setCentre(47.75, 7.34);
    mapWidget->setZoom(13);

    // Ajouter le widget OSM au layout
    layout->addWidget(mapWidget);

    resize(800, 600);
    show();
}

MainWindow::~MainWindow()
{
}
