#include "mapwidgetosm.h"
#include <QPainter>
#include <QtMath>
#include <QUrl>
#include <QDebug>
#include <QPushButton>

Tuile::Tuile() :
    x{0}, y{0}, z{0}
{}

Tuile::Tuile(int x, int y, int z)
    : x{x}, y{y}, z{z}
{}

Tuile::Tuile(int x, int y, int z, QPixmap pix)
    : x{x}, y{y}, z{z}, pixmap{pix}
{}

int Tuile::getX() const
{
    return x;
}

int Tuile::getY() const
{
    return y;
}

int Tuile::getZ() const
{
    return z;
}

QPixmap Tuile::getPixmap() const
{
    return pixmap;
}

void Tuile::setX(int nx)
{
    x = nx;
}

void Tuile::setY(int ny)
{
    y = ny;
}

void Tuile::setZ(int nz)
{
    z = nz;
}

void Tuile::setPixmap(const QPixmap& pix)
{
    pixmap = pix;
}


Overlay::Overlay(const QPixmap& pix, double lat, double lon)
    : pixmap{pix}, lat{lat}, lon{lon}
{}

void Overlay::setPixmap(const QPixmap& pix)
{
    pixmap = pix;
}

void Overlay::setLat(double v)
{
    lat = v;
}

void Overlay::setLon(double v)
{
    lon = v;
}

double Overlay::getLat() const
{
    return lat;
}

double Overlay::getLon() const
{
    return lon;
}

QPixmap Overlay::getPixmap() const
{
    return pixmap;
}


MapWidgetOSM::MapWidgetOSM(QWidget* parent)
    : QWidget(parent),
    networkManager(new QNetworkAccessManager(this)),
    centerLat(47.75),
    centerLon(7.34),
    zoomLevel(MIN_ZOOM),
    tileSize(256)
{
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &MapWidgetOSM::tileDownloaded);
    loadTiles();

    zoomInBtn = new QPushButton("+", this);
    zoomOutBtn = new QPushButton("-", this);

    int btnSize = 36;
    zoomInBtn->setFixedSize(btnSize, btnSize);
    zoomOutBtn->setFixedSize(btnSize, btnSize);

    zoomInBtn->setStyleSheet(
        "QPushButton { background: rgba(255,255,255,180); border: 1px solid gray; "
        "border-radius: 6px; font-size: 20px; font-weight: bold; }"
        "QPushButton:hover { background: white; }"
        );
    zoomOutBtn->setStyleSheet(zoomInBtn->styleSheet());

    connect(zoomInBtn, &QPushButton::clicked, [this]() {
        setZoom(zoomLevel + 1);
    });
    connect(zoomOutBtn, &QPushButton::clicked, [this]() {
        setZoom(zoomLevel - 1);
    });

}

void MapWidgetOSM::setCentre(double lat, double lon)
{
    centerLat = lat;
    centerLon = lon;
    constrainCenter();
    loadTiles();
    update();
}

void MapWidgetOSM::setZoom(int zoom)
{
    const int minZoom = MIN_ZOOM;
    const int maxZoom = MAX_ZOOM;
    int newZoom = qBound(minZoom, zoom, maxZoom);

    // Si le zoom n'a pas changé, on ne recharge pas
    if (newZoom == zoomLevel)
        return;

    zoomLevel = newZoom;
    loadTiles();
    update();
}


void MapWidgetOSM::addOverlay(const QPixmap& pix, double lat, double lon)
{
    overlays.append({pix, lat, lon});
    update();
}

QPointF MapWidgetOSM::latLonToTileXY(double lat, double lon, int z)
{
    double n = qPow(2.0, z);
    double x = n * ((lon + 180.0) / 360.0);
    double latRad = qDegreesToRadians(lat);
    double y = n * (1.0 - (qLn(qTan(latRad) + 1.0 / qCos(latRad)) / M_PI)) / 2.0;
    return QPointF(x, y);
}

QPointF MapWidgetOSM::latLonToPixel(double lat, double lon)
{
    QPointF tileXY = latLonToTileXY(lat, lon, zoomLevel);
    QPointF centerTile = latLonToTileXY(centerLat, centerLon, zoomLevel);
    double dx = (tileXY.x() - centerTile.x()) * tileSize + width()/2;
    double dy = (tileXY.y() - centerTile.y()) * tileSize + height()/2;
    return QPointF(dx, dy);
}

QString MapWidgetOSM::tileKey(int x, int y, int z) const
{
    return QString("%1/%2/%3").arg(z).arg(x).arg(y);
}

void MapWidgetOSM::constrainCenter()
{
    centerLat = qBound(MIN_LAT, centerLat, MAX_LAT);
    centerLon = qBound(MIN_LON, centerLon, MAX_LON);
}

void MapWidgetOSM::loadTiles()
{
    QPointF centerTile = latLonToTileXY(centerLat, centerLon, zoomLevel);
    int cx = static_cast<int>(centerTile.x());
    int cy = static_cast<int>(centerTile.y());

    int range = 1; // 3x3 tuiles
    for (int dx = -range; dx <= range; ++dx) {
        for (int dy = -range; dy <= range; ++dy) {
            int x = cx + dx;
            int y = cy + dy;
            QString key = tileKey(x, y, zoomLevel);

            if (!tiles.contains(key)) {
                QUrl url(QString("https://a.tile.openstreetmap.fr/osmfr/%1/%2/%3.png")
                             .arg(zoomLevel).arg(x).arg(y));
                qDebug() << "Téléchargement tuile:" << url.toString();

                QNetworkRequest req(url);
                req.setRawHeader("User-Agent", "MapWidgetOSMQt/1.0 (projetReseaux)");

                networkManager->get(req);
                tiles.insert(key, {x, y, zoomLevel, QPixmap()});
            }
        }
    }
}

void MapWidgetOSM::tileDownloaded(QNetworkReply* reply)
{
    if (!reply) return;

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Erreur téléchargement:" << reply->errorString()
            << "URL:" << reply->url().toString();
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QPixmap pix;
    if (!pix.loadFromData(data)) {
        qDebug() << "⚠️ Échec chargement image depuis:" << reply->url().toString();
    }

    QStringList parts = reply->url().path().split("/", Qt::SkipEmptyParts);
    if (parts.size() >= 3) {
        int idx = parts.size();
        int z = parts[idx - 3].toInt();
        int x = parts[idx - 2].toInt();
        int y = parts[idx - 1].split(".").first().toInt();
        QString key = tileKey(x, y, z);
        qDebug() << "Tuile reçue:" << z << x << y << "-> key =" << key << "pix null?" << pix.isNull();
        if (tiles.contains(key)) {
            tiles[key].setPixmap(pix);
        } else {
            qDebug() << "Warning: clé de tuile introuvable pour" << key;
        }
    } else {
        qDebug() << "URL tile malformed:" << reply->url().toString();
    }

    reply->deleteLater();
    update();
}

void MapWidgetOSM::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);

    // 1. Coordonnées flottantes de la tuile correspondant au centre de la carte
    QPointF centerTileFloat = latLonToTileXY(centerLat, centerLon, zoomLevel);

    // 2. Coordonnées flottantes de la tuile correspondant au coin supérieur gauche du widget
    // C'est le centre de la carte (en tuile flottante) moins la moitié de la taille du widget (en tuile)
    double widgetTopLeftTileX = centerTileFloat.x() - (width() / 2.0) / tileSize;
    double widgetTopLeftTileY = centerTileFloat.y() - (height() / 2.0) / tileSize;

    // 3. Coordonnées entières de la première tuile visible (coin supérieur gauche)
    int startTileX = qFloor(widgetTopLeftTileX);
    int startTileY = qFloor(widgetTopLeftTileY);

    // 4. Position en pixels du coin supérieur gauche de la tuile (startTileX, startTileY) sur le widget.
    // L'offset est la partie fractionnelle de la tuile visible
    double startPixelX = (startTileX - widgetTopLeftTileX) * tileSize;
    double startPixelY = (startTileY - widgetTopLeftTileY) * tileSize;

    // Dessin des tuiles
    int currentX = startTileX;
    while (true) {
        // Calcul de la position X de la tuile courante (en ajoutant des décalages entiers de tileSize)
        double currentPixelX = startPixelX + (currentX - startTileX) * tileSize;
        if (currentPixelX >= width()) break;

        int currentY = startTileY;
        while (true) {
            double currentPixelY = startPixelY + (currentY - startTileY) * tileSize;
            if (currentPixelY >= height()) break;

            QString key = tileKey(currentX, currentY, zoomLevel);
            if (tiles.contains(key)) {
                const Tuile& tile = tiles.value(key);
                if (!tile.getPixmap().isNull()) {
                    // Dessiner la tuile
                    painter.drawPixmap(static_cast<int>(currentPixelX),
                                       static_cast<int>(currentPixelY),
                                       tileSize, tileSize, tile.getPixmap());
                } else {
                    // Optionnel : Dessiner une tuile vide/grise ou un indicateur de chargement
                    painter.fillRect(static_cast<int>(currentPixelX),
                                     static_cast<int>(currentPixelY),
                                     tileSize, tileSize, Qt::lightGray);
                }
            }
            currentY++;
        }
        currentX++;
    }

    // Dessin des Overlays
    for (auto& ov : overlays) {
        QPointF pos = latLonToPixel(ov.getLat(), ov.getLon());
        painter.drawPixmap(pos.x() - ov.getPixmap().width()/2,
                           pos.y() - ov.getPixmap().height()/2,
                           ov.getPixmap());
    }
}

QPointF MapWidgetOSM::latLonFromPixel(const QPointF& pixel)
{
    QPointF centerTile = latLonToTileXY(centerLat, centerLon, zoomLevel);
    double tileX = centerTile.x() + (pixel.x() - width()/2.0) / tileSize;
    double tileY = centerTile.y() + (pixel.y() - height()/2.0) / tileSize;

    double n = qPow(2.0, zoomLevel);
    double lon = tileX / n * 360.0 - 180.0;
    double latRad = qAtan(sinh(M_PI * (1 - 2 * tileY / n)));
    double lat = qRadiansToDegrees(latRad);

    return QPointF(lon, lat);
}



void MapWidgetOSM::wheelEvent(QWheelEvent* event)
{
    int step = (event->angleDelta().y() > 0) ? 1 : -1;

    int newZoom = zoomLevel + step;

    const int minZoom = 12;
    const int maxZoom = 18;

    newZoom = qBound(minZoom, newZoom, maxZoom);

    if (newZoom == zoomLevel)
        return;

    QPointF mousePos = event->position();
    QPointF before = latLonFromPixel(mousePos);

    zoomLevel = newZoom;
    loadTiles();

    QPointF after = latLonFromPixel(mousePos);

    centerLat += (before.y() - after.y());
    centerLon += (before.x() - after.x());
    constrainCenter();


    update();
}



void MapWidgetOSM::mousePressEvent(QMouseEvent* event)
{
    lastMousePos = event->pos();
}

void MapWidgetOSM::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton) {
        QPoint delta = event->pos() - lastMousePos;
        double factor = 1.0 / (tileSize * qPow(2, zoomLevel));

        centerLat += delta.y() * factor * 360.0 / M_PI;
        centerLon -= delta.x() * factor * 360.0 / M_PI;

        constrainCenter();
        lastMousePos = event->pos();
        loadTiles();
        update();
    }
}


void MapWidgetOSM::resizeEvent(QResizeEvent*)
{
    int margin = 10;
    int spacing = 5;
    int btnSize = zoomInBtn->height();

    zoomInBtn->move(width() - btnSize - margin, margin);
    zoomOutBtn->move(width() - btnSize - margin, margin + btnSize + spacing);
}
