#include "toolbar.h"

ToolBar::ToolBar(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    // Slider de vitesse
    speedSlider = new QSlider(Qt::Horizontal, this);
    speedSlider->setRange(0, 4);   // 0.25 -> 2.0 par pas de 0.25
    speedSlider->setValue(1);      // valeur par défaut 1.0 (4 * 0.25)
    connect(speedSlider, &QSlider::valueChanged, this, &ToolBar::onSpeedSliderChanged);

    // Label affichant la valeur
    speedLabel = new QLabel("Vitesse: 1x", this);

    // Bouton paramètres
    paramButton = new QPushButton("Paramètres", this);
    connect(paramButton, &QPushButton::clicked, this, &ToolBar::onParamButtonClicked);

    layout->addWidget(speedLabel);
    layout->addWidget(speedSlider);
    layout->addWidget(paramButton);

    setLayout(layout);
    setFixedHeight(50);
}


// Slot pour le slider
void ToolBar::onSpeedSliderChanged(int value)
{
    double facteurBrut = value * 0.25; // ce que le simulateur utilise réellement

    if (value == 0) {
        speedLabel->setText("Vitesse: Pause");
        emit speedChanged(0.0);
        return;
    }

    double facteurAffiche = facteurBrut / 0.25; // normalisation : 0.25 devient x1

    speedLabel->setText(QString("Vitesse: %1x").arg(facteurAffiche, 0, 'f', 0));
    emit speedChanged(facteurBrut);
}

// Slot pour le bouton paramètres
void ToolBar::onParamButtonClicked()
{
    emit openParamWindow();
}
