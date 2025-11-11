#include <QApplication>
#include <QLabel>
#include <QVBoxLayout>
#include <QtConcurrent>
#include <QFutureWatcher>
#include "../frontend/mainwindow.h"
#include "../backend/simulateur.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Splash screen full-screen
    QWidget splash;
    splash.setWindowFlags(Qt::FramelessWindowHint | Qt::SplashScreen);
    splash.setStyleSheet("background-color: #2e3440;");
    splash.showFullScreen();

    QLabel* label = new QLabel("Chargement de la carte, veuillez patienter...", &splash);
    label->setStyleSheet("color: white; font-size: 36px;");
    label->setAlignment(Qt::AlignCenter);

    QVBoxLayout* layout = new QVBoxLayout(&splash);
    layout->addWidget(label);

    qApp->processEvents(); // afficher le splash immédiatement

    // Charger le simulateur dans un thread séparé
    QFuture<Simulateur*> future = QtConcurrent::run([]() -> Simulateur* {
        return new Simulateur(); // méthode longue
    });

    QFutureWatcher<Simulateur*> watcher;
    watcher.setFuture(future);

    QObject::connect(&watcher, &QFutureWatcher<Simulateur*>::finished, [&]() {
        // récupérer simulateur
        Simulateur* simulateur = future.result();

        // fermer splash
        splash.close();

        // ouvrir fenêtre principale
        MainWindow* w = new MainWindow(simulateur);
        w->show();
    });

    return a.exec();
}
