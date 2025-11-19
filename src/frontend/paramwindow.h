#ifndef PARAMWINDOW_H
#define PARAMWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QFormLayout>
#include "../backend/simulateur.h"

class ParamWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ParamWindow(Simulateur *sim, QWidget *parent = nullptr);

signals:
    void closed();

private slots:
    void onValidate();
    void onCancel();

private:
    Simulateur *simulation;

    QSpinBox *vehiculesBox;
    QSpinBox *rayonBox;
    QCheckBox *rayonAleaBox;
    QCheckBox *afficheRayonBox;
    QCheckBox *afficheGrapheBox;

    QPushButton *validateButton;
    QPushButton *cancelButton;
};

#endif // PARAMWINDOW_H

