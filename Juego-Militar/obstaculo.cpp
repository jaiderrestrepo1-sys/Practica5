#include "obstaculo.h"
#include <QGraphicsScene>
#include <QBrush>

Obstaculo::Obstaculo(int resistencia, int jugador, QGraphicsItem *parent)
    : QGraphicsRectItem(parent),
    m_resistencia(resistencia),
    jugadorDueño(jugador)
{
    setRect(0, 0, 40, 40);

    if (jugador == 1)
        setBrush(QBrush(Qt::blue));
    else
        setBrush(QBrush(Qt::green));

    setData(0, 1);
}

void Obstaculo::recibirDano(int d)
{
    m_resistencia -= d;

    if (m_resistencia <= 0) {
        if (scene()) scene()->removeItem(this);
        delete this;
    }
}
