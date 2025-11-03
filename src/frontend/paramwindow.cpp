#include "../frontend/ParamWindow.h"
#include <QHBoxLayout>

ParamWindow::ParamWindow(Simulateur *sim, QWidget *parent)
    : QWidget(parent), simulation(sim)
{
    setWindowTitle("Paramètres de la simulation");

    vehiculesBox = new QSpinBox(this);
    vehiculesBox->setRange(1, 1000);
    vehiculesBox->setValue(simulation->getNbVehicules());

    auto *form = new QFormLayout();
    form->addRow("Nombre de véhicules :", vehiculesBox);

    validateButton = new QPushButton("Valider", this);
    cancelButton = new QPushButton("Annuler", this);

    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(validateButton);
    buttonLayout->addWidget(cancelButton);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(form);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    connect(validateButton, &QPushButton::clicked, this, &ParamWindow::onValidate);
    connect(cancelButton, &QPushButton::clicked, this, &ParamWindow::onCancel);
}

void ParamWindow::onValidate()
{
    simulation->mettreAJourNbVehicules(vehiculesBox->value());
    vehiculesBox->setValue(simulation->getNbVehicules());
    emit closed();
    close();
}

void ParamWindow::onCancel()
{
    vehiculesBox->setValue(simulation->getNbVehicules());
    emit closed();
    close();
}
