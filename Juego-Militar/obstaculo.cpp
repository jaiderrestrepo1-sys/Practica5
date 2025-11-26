#include "obstaculo.h"
#include <QGraphicsScene>
#include <QBrush>

Obstaculo::Obstaculo(int resistencia, QGraphicsItem *parent)
    : QGraphicsRectItem(parent), m_resistencia(resistencia)
{
    setRect(0, 0, 40, 40);
    setBrush(QBrush(Qt::gray));
    setData(0, 1);
}

void Obstaculo::recibirDano(int d)
{
    m_resistencia -= d;
    if (m_resistencia <= 0) {
        if (scene()) scene()->removeItem(this);
        delete this;
    } else {

        int r = qMax(50, 200 - (200 * (100 - m_resistencia) / 100));
        setBrush(QBrush(QColor(r, r, r)));
    }
}
