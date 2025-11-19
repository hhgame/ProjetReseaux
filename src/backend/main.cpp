#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QPainter>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <QThread>
#include "../frontend/mainwindow.h"
#include "../backend/simulateur.h"

// Loader circulaire
class CircularLoader : public QWidget {
public:
    explicit CircularLoader(QWidget* parent = nullptr)
        : QWidget(parent), angle(0)
    {
        setFixedSize(150, 150); // taille du loader
        timer = new QTimer(this);
        QObject::connect(timer, &QTimer::timeout, [this]() {
            angle = (angle + 10) % 360;
            update();
        });
        timer->start(50);
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        int radius = width()/2 - 10;
        QPoint center(width()/2, height()/2);

        int numPoints = 8;
        int pointRadius = 10;

        for (int i = 0; i < numPoints; ++i) {
            double theta = 2 * M_PI * i / numPoints + qDegreesToRadians((double)angle);
            int x = center.x() + radius * std::cos(theta) - pointRadius/2;
            int y = center.y() + radius * std::sin(theta) - pointRadius/2;

            QColor color = QColor::fromHsvF((double)i/numPoints, 0.8, 0.9);
            painter.setBrush(color);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(QPoint(x + pointRadius/2, y + pointRadius/2), pointRadius, pointRadius);
        }
    }

private:
    int angle;
    QTimer* timer;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget splash;
    splash.setWindowFlags(Qt::FramelessWindowHint | Qt::SplashScreen);
    splash.setStyleSheet(
        "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #4c566a, stop:1 #2e3440); color: white;"
        );
    splash.showMaximized();

    // Layout centré
    QVBoxLayout* layout = new QVBoxLayout(&splash);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    QLabel* label = new QLabel("Chargement de la carte...", &splash);
    label->setStyleSheet("font-size: 48px; font-weight: bold; color: #eceff4;");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    QLabel* subLabel = new QLabel("Veuillez patienter", &splash);
    subLabel->setStyleSheet("font-size: 24px; color: #d8dee9;");
    subLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(subLabel);

    // Loader circulaire centré
    CircularLoader* loader = new CircularLoader(&splash);
    layout->addWidget(loader, 0, Qt::AlignCenter);

    qApp->processEvents(); // afficher le splash immédiatement

    // Charger simulateur en thread séparé
    QFuture<Simulateur*> future = QtConcurrent::run([]() -> Simulateur* {
        QThread::sleep(2);
        return new Simulateur();
    });

    QFutureWatcher<Simulateur*> watcher;
    watcher.setFuture(future);

    QObject::connect(&watcher, &QFutureWatcher<Simulateur*>::finished, [&]() {
        Simulateur* simulateur = future.result();
        splash.close();

        MainWindow* w = new MainWindow(simulateur);
        w->showMaximized();
    });

    return a.exec();
}
