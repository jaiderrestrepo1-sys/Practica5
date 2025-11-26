#include "projectile.h"
#include <QGraphicsScene>

Projectile::Projectile(double vx_, double vy_, double masa_, QGraphicsItem *parent)
    : QObject(), QGraphicsEllipseItem(parent),
    vx(vx_), vy(vy_), masa(masa_)
{
    setRect(-5, -5, 10, 10);
    setBrush(QBrush(Qt::red));

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Projectile::mover);
    timer->start(20);
}

void Projectile::mover()
{
    vy += g;

    setPos(x() + vx, y() + vy);

    if (!scene()) return;

    QRectF box = scene()->sceneRect();

    if (x() < box.left() || x() > box.right())
        vx = -vx;

    if (y() < box.top() || y() > box.bottom())
        vy = -vy;

    QList<QGraphicsItem*> items = collidingItems();
    for (auto *it : items) {
        if (it->data(0).toInt() == 1) {

            double vmag = std::sqrt(vx*vx + vy*vy);
            int dano = static_cast<int>(masa * vmag * 0.4);

            emit impacto(dano);

            scene()->removeItem(this);
            timer->stop();
            deleteLater();
            return;
        }
    }
}
