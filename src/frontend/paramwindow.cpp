#include "../frontend/ParamWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

ParamWindow::ParamWindow(Simulateur *sim, QWidget *parent)
    : QWidget(parent), simulation(sim)
{
    setWindowTitle("Paramètres de la simulation");

    // Widgets
    vehiculesBox = new QSpinBox();
    vehiculesBox->setRange(1, 5000);
    vehiculesBox->setValue(simulation->getNbVehicules());

    rayonBox = new QSpinBox();
    rayonBox->setRange(10, 1000);
    rayonBox->setValue(simulation->getRayonTransmission());

    rayonAleaBox = new QCheckBox("Rayon aléatoire");
    rayonAleaBox->setChecked(simulation->getRayonAleatoire());
    rayonBox->setEnabled(!simulation->getRayonAleatoire());

    connect(rayonAleaBox, &QCheckBox::toggled, this, [=](bool checked){
        rayonBox->setEnabled(!checked);
    });


    afficheRayonBox = new QCheckBox("Afficher rayon de transmission");
    afficheRayonBox->setChecked(simulation->getAfficheRayonTransmission());

    afficheGrapheBox = new QCheckBox("Afficher graphe d'interférence");
    afficheGrapheBox->setChecked(simulation->getAfficheGrapheInterference());

    auto *form = new QFormLayout();
    form->addRow("Nombre de véhicules :", vehiculesBox);
    form->addRow("Rayon transmission (m) :", rayonBox);
    form->addRow("", rayonAleaBox);
    form->addRow("", afficheRayonBox);
    form->addRow("", afficheGrapheBox);

    validateButton = new QPushButton("Valider");
    cancelButton = new QPushButton("Annuler");

    auto *buttons = new QHBoxLayout();
    buttons->addStretch();
    buttons->addWidget(validateButton);
    buttons->addWidget(cancelButton);

    auto *main = new QVBoxLayout(this);
    main->addLayout(form);
    main->addLayout(buttons);
    setLayout(main);

    connect(validateButton, &QPushButton::clicked, this, &ParamWindow::onValidate);
    connect(cancelButton, &QPushButton::clicked, this, &ParamWindow::onCancel);
}

void ParamWindow::onValidate()
{
    // Mettre à jour les paramètres dans le simulateur
    simulation->setRayonAleatoire(rayonAleaBox->isChecked());
    simulation->setRayonTransmission(rayonBox->value());
    simulation->mettreAJourNbVehicules(vehiculesBox->value());


    simulation->setAfficheRayonTransmission(afficheRayonBox->isChecked());
    simulation->setAfficheGrapheInterference(afficheGrapheBox->isChecked());

    // Recalculer le graphe des interférences immédiatement si nécessaire
    if (simulation->getAfficheGrapheInterference()) {
        simulation->recalculerGrapheInterference();
    } else {
        simulation->clearGrapheInterference();
    }

    emit closed();
    close();
}

void ParamWindow::onCancel()
{
    emit closed();
    close();
}

