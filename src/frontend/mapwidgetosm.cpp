#include "mapwidgetosm.h"
#include <QPainter>
#include <QtMath>
#include <QUrl>

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
    : QWidget{parent},
    networkManager(new QNetworkAccessManager(this)),
    // Centre Mulhouse approx
    centerLat(47.75),
    centerLon(7.34),
    zoomLevel(12),
    tileSize(256)
{
    // Connexion du slot de téléchargement des tuiles
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &MapWidgetOSM::tileDownloaded);
}

void MapWidgetOSM::setCentre(double lat, double lon)
{
    centerLat = lat;
    centerLon = lon;
    // Limiter la zone
    constrainCenter();

    // Recharger les tuiles
    loadTiles();

    // Rafraîchir l'affichage
    update();
}

void MapWidgetOSM::setZoom(int zoom)
{
    // Limite de zoom
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
    // Limiter la carte au bassin mulhousien
    double minLat = 47.6, maxLat = 48.0;
    double minLon = 7.1, maxLon = 7.6;
    centerLat = qBound(minLat, centerLat, maxLat);
    centerLon = qBound(minLon, centerLon, maxLon);
}

void MapWidgetOSM::loadTiles()
{
    tiles.clear();
    QPointF centerTile = latLonToTileXY(centerLat, centerLon, zoomLevel);
    int cx = static_cast<int>(centerTile.x());
    int cy = static_cast<int>(centerTile.y());

    /** Charger une grille 3x3 autour du centre */
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            int x = cx + dx;
            int y = cy + dy;
            QString key = tileKey(x, y, zoomLevel);
            if (!tiles.contains(key)) {
                QUrl url(QString("https://tile.openstreetmap.org/%1/%2/%3.png")
                             .arg(zoomLevel).arg(x).arg(y));
                networkManager->get(QNetworkRequest(url));
                tiles.insert(key, {x, y, zoomLevel, QPixmap()});
            }
        }
    }
}

void MapWidgetOSM::tileDownloaded()
{
    /** Récupération de la tuile téléchargée */
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    QByteArray data = reply->readAll();
    QPixmap pix;
    pix.loadFromData(data);

    /** Extraire x/y/z depuis l'URL de la tuile */
    QUrl url = reply->url();
    QStringList parts = url.path().split("/");
    if (parts.size() >= 4) {
        int z = parts[1].toInt();
        int x = parts[2].toInt();
        int y = parts[3].split(".").first().toInt();
        QString key = tileKey(x, y, z);
        if (tiles.contains(key)) tiles[key].getPixmap() = pix;
    }

    reply->deleteLater();
    update();
}

void MapWidgetOSM::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);

    QPointF centerTile = latLonToTileXY(centerLat, centerLon, zoomLevel);
    int cx = static_cast<int>(centerTile.x());
    int cy = static_cast<int>(centerTile.y());
    double fx = centerTile.x() - cx;
    double fy = centerTile.y() - cy;
    int offsetX = width()/2 - tileSize*fx;
    int offsetY = height()/2 - tileSize*fy;

    /** Dessiner les tuiles */
    for (auto tile : tiles) {
        if (!tile.getPixmap().isNull()) {
            int dx = (tile.getX() - cx) * tileSize + offsetX;
            int dy = (tile.getY() - cy) * tileSize + offsetY;
            painter.drawPixmap(dx, dy, tileSize, tileSize, tile.getPixmap());
        }
    }

    /** Dessiner les overlays par-dessus */
    for (auto ov : overlays) {
        QPointF pos = latLonToPixel(ov.getLat(), ov.getLon());
        painter.drawPixmap(pos.x() - ov.getPixmap().width()/2,
                           pos.y() - ov.getPixmap().height()/2,
                           ov.getPixmap());
    }
}

void MapWidgetOSM::wheelEvent(QWheelEvent* event)
{
    /** Zoomer ou dézoomer avec la molette */
    int delta = event->angleDelta().y();
    setZoom(zoomLevel + (delta>0 ? 1 : -1));
}

void MapWidgetOSM::mousePressEvent(QMouseEvent* event)
{
    /** Début du pan */
    lastMousePos = event->pos();
}

void MapWidgetOSM::mouseMoveEvent(QMouseEvent* event)
{
    /** Déplacement de la carte si clic gauche maintenu */
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

