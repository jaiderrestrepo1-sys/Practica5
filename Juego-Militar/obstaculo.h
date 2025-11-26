#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QGraphicsRectItem>

class Obstaculo : public QGraphicsRectItem {
public:
    Obstaculo(int resistencia, int jugador, QGraphicsItem *parent = nullptr);

    void recibirDano(int d);
    int resistencia() const { return m_resistencia; }
    int jugadorDueño;

private:
    int m_resistencia;
};

#endif
