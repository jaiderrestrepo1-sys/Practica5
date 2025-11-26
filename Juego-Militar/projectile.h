#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <QGraphicsEllipseItem>
#include <QTimer>
#include <QObject>
#include <cmath>

class Projectile : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT

public:
    double vx, vy;
    double masa;
    double g = 0.4;

    Projectile(double vx, double vy, double masa) : vx(vx), vy(vy), masa(masa) {
        setRect(-5, -5, 10, 10);
        setBrush(Qt::red);

        QTimer *t = new QTimer(this);
        connect(t, &QTimer::timeout, this, &Projectile::mover);
        t->start(20);
    }

signals:
    void impacto(int dano);

public slots:
    void mover() {
        vy += g;

        setPos(x() + vx, y() + vy);


        if (x() < 0 || x() > 800) vx *= -1;
        if (y() < 0 || y() > 600) vy *= -1;

        QList<QGraphicsItem*> items = collidingItems();
        for (auto *it : items) {
            if (it->data(0) == 1) { // obstáculo marcado
                double vmag = std::sqrt(vx*vx + vy*vy);
                int dano = masa * vmag * 0.3;

                emit impacto(dano);

                scene()->removeItem(this);
                delete this;
                return;
            }
        }
    }
};

#endif // PROJECTILE_H
