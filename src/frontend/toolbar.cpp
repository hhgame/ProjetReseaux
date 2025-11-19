#include "toolbar.h"

ToolBar::ToolBar(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    // Slider de vitesse
    speedSlider = new QSlider(Qt::Horizontal, this);
    speedSlider->setRange(1, 8);   // 0.25 -> 2.0 par pas de 0.25
    speedSlider->setValue(4);      // valeur par défaut 1.0 (4 * 0.25)
    connect(speedSlider, &QSlider::valueChanged, this, &ToolBar::onSpeedSliderChanged);

    // Label affichant la valeur
    speedLabel = new QLabel("Vitesse: 0.25x", this);

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
    double vitesse = value * 0.25; // conversion vers réel
    speedLabel->setText(QString("Vitesse: %1x").arg(vitesse));
    emit speedChanged(vitesse);
}

// Slot pour le bouton paramètres
void ToolBar::onParamButtonClicked()
{
    emit openParamWindow();
}
