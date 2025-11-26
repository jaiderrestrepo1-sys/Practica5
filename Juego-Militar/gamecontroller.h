#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QGraphicsScene>
#include <vector>

class GameController : public QObject {
    Q_OBJECT

public:
    GameController(QGraphicsScene *scene, QObject *parent = nullptr);

    void colocarObstaculo(int x, int y, int resistencia, int jugador);
    void lanzar(double angulo_deg, double velocidad);

    int turnoActual() const { return turno; }

signals:
    void progreso(int vida1, int vida2);
    void jugadorGana(int jugador);
    void turnoCambiado(int turno);

public slots:
    void procesarImpacto(int d);

private:
    QGraphicsScene *scene;
    int turno = 1;

    int vida1 = 0, vida2 = 0;
    int obs1 = 0, obs2 = 0;
};

#endif
