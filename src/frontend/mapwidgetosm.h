#ifndef MAPWIDGETOSM_H
#define MAPWIDGETOSM_H
#pragma once

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMap>
#include <QPixmap>
#include <QWheelEvent>
#include <QMouseEvent>

/**
 * Définit une tuile
 */
class Tuile {
public:
    Tuile();
    Tuile(int x, int y, int z);
    Tuile(int x, int y, int z, QPixmap pix);

    /**
     * Setters
     */
    void setX(int nx);
    void setY(int ny);
    void setZ(int nz);
    void setPixmap(const QPixmap& pix);

    /**
     * Getters
     */
    int getX() const;
    int getY() const;
    int getZ() const;
    QPixmap getPixmap() const;

private:
    int x;
    int y;
    int z;
    QPixmap pixmap;
};

class Overlay {
public:
    Overlay(const QPixmap& pix, double lat, double lon);

    /**
     * Setters
     */
    void setLat(double nlat);
    void setLon(double nlon);
    void setPixmap(const QPixmap& pix);

    /**
     * Getters
     */
    double getLat() const;
    double getLon() const;
    QPixmap getPixmap() const;

private:
    QPixmap pixmap;
    double lat;
    double lon;
};


/**
 * Widget dédié à l'affichage des tuiles OpenStreetMap.
 */
class MapWidgetOSM : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructeur du widget OSM
     * @param parent Widget parent éventuel
     */
    explicit MapWidgetOSM(QWidget* parent = nullptr);

    /**
     * Définit le centre de la carte en latitude/longitude
     * @param lat Latitude du centre
     * @param lon Longitude du centre
     */
    void setCentre(double lat, double lon);

    /**
     * Définit le niveau de zoom
     * @param zoom Niveau de zoom (borne automatiquement)
     */
    void setZoom(int zoom);

    /**
     * Ajoute un overlay par-dessus la carte
     * @param pix Pixmap à afficher
     * @param lat Latitude du point
     * @param lon Longitude du point
     */
    void addOverlay(const QPixmap& pix, double lat, double lon);

protected:
    /**
     * Dessine la carte et les overlays
     */
    void paintEvent(QPaintEvent* event) override;

    /**
     * Gestion du zoom avec la molette
     */
    void wheelEvent(QWheelEvent* event) override;

    /**
     * Gestion du début du pan avec la souris
     */
    void mousePressEvent(QMouseEvent* event) override;

    /**
     * Gestion du déplacement de la carte (pan)
     */
    void mouseMoveEvent(QMouseEvent* event) override;

private slots:
    /**
     * Slot appelé lorsque le téléchargement d'une tuile est terminé
     */
    void tileDownloaded(QNetworkReply* reply);


private:

    /** Gestionnaire de téléchargement */
    QNetworkAccessManager* networkManager;

    /** Cache des tuiles */
    QMap<QString, Tuile> tiles;

    /** Liste des overlays */
    QList<Overlay> overlays;

    /** Latitude du centre */
    double centerLat;

    /** Longitude du centre */
    double centerLon;

    /** Niveau de zoom actuel */
    int zoomLevel;

    /** Taille d'une tuile en pixels */
    int tileSize;

    /** Position de la souris sur le pan */
    QPoint lastMousePos;

    /**
     * Charge les tuiles autour du centre
     */
    void loadTiles();

    /**
     * Convertit une coordonnée lat/lon en coordonnées de tuile
     * @param lat Latitude
     * @param lon Longitude
     * @param z Niveau de zoom
     * @return Coordonnées de la tuile (x,y)
     */
    QPointF latLonToTileXY(double lat, double lon, int z);

    /**
     * Convertit une coordonnée lat/lon en position pixel sur le widget
     * @param lat Latitude
     * @param lon Longitude
     * @return Position pixel (x,y)
     */
    QPointF latLonToPixel(double lat, double lon);

    /**
     * Génère une clé unique pour une tuile
     * @param x Coordonnée x de la tuile
     * @param y Coordonnée y de la tuile
     * @param z Niveau de zoom
     * @return Clé sous forme "z/x/y"
     */
    QString tileKey(int x, int y, int z) const;

    /**
     * Contrainte le centre pour rester sur le bassin mulhousien
     */
    void constrainCenter();
};


#endif // MAPWIDGETOSM_H
