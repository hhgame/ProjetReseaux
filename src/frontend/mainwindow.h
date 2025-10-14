#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMainWindow>
#include "mapwidgetosm.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    MapWidgetOSM* mapWidget;
};

#endif // MAINWINDOW_H
