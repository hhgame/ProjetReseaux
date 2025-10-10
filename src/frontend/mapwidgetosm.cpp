#include "mapwidgetosm.h"
#include <QPainter>
#include <QtMath>
#include <QUrl>
#include <QDebug>

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
    zoomLevel(12),
    tileSize(256)
{
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &MapWidgetOSM::tileDownloaded);
    loadTiles();
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
    zoomLevel = qBound(12, zoom, 16);
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
    centerLat = qBound(47.6, centerLat, 48.0);
    centerLon = qBound(7.1, centerLon, 7.6);
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
                req.setRawHeader("User-Agent", "MapWidgetOSMQt/1.0 (hugoh@example.com)");

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

    // Split en ignorant les segments vides (évite le premier "")
    QStringList parts = reply->url().path().split("/", Qt::SkipEmptyParts);
    // On attend les 3 derniers éléments : prefix (osmfr), z, x, y.png  -> on prend les derniers 3
    if (parts.size() >= 3) {
        // indexation : ... , z, x, y.png  -> z = parts[parts.size()-3]
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
    // L'offset est la partie fractionnelle de la tuile visible (startTile - widgetTopLeft) * tileSize
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


void MapWidgetOSM::wheelEvent(QWheelEvent* event)
{
    int delta = event->angleDelta().y();
    setZoom(zoomLevel + (delta>0 ? 1 : -1));
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
        centerLat -= delta.y() * factor * 360.0 / M_PI;
        centerLon -= delta.x() * factor * 360.0 / M_PI;
        constrainCenter();
        lastMousePos = event->pos();
        loadTiles();
        update();
    }
}
