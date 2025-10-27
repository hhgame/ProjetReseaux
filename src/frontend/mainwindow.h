#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMainWindow>
#include "mapwidgetosm.h"
<<<<<<< HEAD
#include "../backend/simulateur.h"
=======
>>>>>>> 5467289d52eb9a8094f8d9e318f4196ff04ef2c3

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    MapWidgetOSM* mapWidget;
<<<<<<< HEAD
    Simulateur* s;
=======
>>>>>>> 5467289d52eb9a8094f8d9e318f4196ff04ef2c3
};

#endif // MAINWINDOW_H
