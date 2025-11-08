#include "toolbar.h"

ToolBar::ToolBar(QWidget *parent)
    : QWidget(parent)
{
    // Layout horizontal
    QHBoxLayout *layout = new QHBoxLayout(this);

    // Slider de vitesse
    speedSlider = new QSlider(Qt::Horizontal, this);
    speedSlider->setRange(0, 3); // 0 = pause, 1 = normal, 2 = 2x, 3 = 3x
    speedSlider->setValue(1);    // vitesse normale par défaut
    connect(speedSlider, &QSlider::valueChanged, this, &ToolBar::onSpeedSliderChanged);

    // Label affichant la valeur
    speedLabel = new QLabel("Vitesse: 1x", this);

    // Bouton paramètres
    paramButton = new QPushButton("Paramètres", this);
    connect(paramButton, &QPushButton::clicked, this, &ToolBar::onParamButtonClicked);

    // Ajouter au layout
    layout->addWidget(speedLabel);
    layout->addWidget(speedSlider);
    layout->addWidget(paramButton);

    setLayout(layout);

    setFixedHeight(50);
}

// Slot pour le slider
void ToolBar::onSpeedSliderChanged(int value)
{
    speedLabel->setText(QString("Vitesse: %1x").arg(value));
    emit speedChanged(static_cast<double>(value));
}

// Slot pour le bouton paramètres
void ToolBar::onParamButtonClicked()
{
    emit openParamWindow();
}
