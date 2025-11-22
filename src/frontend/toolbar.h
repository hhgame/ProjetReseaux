#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>

/**
 * Classe représentant la barre des outils pour la simulation
 */
class ToolBar : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructeur du widget de toolbar
     * @param parent le QWidget parent (nécessaire à Qt) de base à nullptr
     */
    explicit ToolBar(QWidget *parent = nullptr);

signals:
    /**
     * Signal sur changement de vitesse
     * @param newSpeed nouvelle vitesse
     */
    void speedChanged(double newSpeed);
    /**
     * Signal sur ouverture de la fenêtre de paramétrage
     */
    void openParamWindow();

private slots:
    /**
     * Sur changement de vitesse sur la slider
     * @param value la nouvelle vitesse d'execution
     */
    void onSpeedSliderChanged(int value);
    /**
     * Sur clique sur le bouton de paramétrage
     */
    void onParamButtonClicked();

private:
    /** La barre de choix de vitesse d'exécution */
    QSlider *speedSlider;
    /** Label qui indique la vitesse actuelle d'exécution */
    QLabel *speedLabel;
    /** Bouton de paramétrage */
    QPushButton *paramButton;
};

#endif // TOOLBAR_H
