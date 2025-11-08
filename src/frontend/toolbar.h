#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>

class ToolBar : public QWidget
{
    Q_OBJECT

public:
    explicit ToolBar(QWidget *parent = nullptr);

signals:
    void speedChanged(double newSpeed);
    void openParamWindow();

private slots:
    void onSpeedSliderChanged(int value);
    void onParamButtonClicked();

private:
    QSlider *speedSlider;
    QLabel *speedLabel;
    QPushButton *paramButton;
};

#endif // TOOLBAR_H
