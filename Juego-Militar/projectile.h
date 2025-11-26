#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <QBrush>
#include <cmath>

class Projectile : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT

public:
    explicit Projectile(double vx, double vy, double masa, QGraphicsItem *parent = nullptr);
    ~Projectile() override = default;

    double vx;
    double vy;
    double masa;
    double g = 0.4;

signals:
    void impacto(int dano);

public slots:
    void mover();

private:
    QTimer *m_timer;
};

#endif // PROJECTILE_H
