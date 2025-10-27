#include "mainwindow.h"
#include <QVBoxLayout>
#include <QPixmap>
<<<<<<< HEAD
#include "../backend/simulateur.h"
=======
>>>>>>> 5467289d52eb9a8094f8d9e318f4196ff04ef2c3


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Créer un widget central
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

<<<<<<< HEAD
    s = new Simulateur{};

=======
>>>>>>> 5467289d52eb9a8094f8d9e318f4196ff04ef2c3
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
