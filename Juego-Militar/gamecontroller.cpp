#include "gamecontroller.h"
#include "projectile.h"
#include "obstaculo.h"
#include <cmath>

GameController::GameController(QGraphicsScene *scene_, QObject *parent)
    : QObject(parent), scene(scene_)
{
}

void GameController::colocarObstaculo(int x, int y, int resistencia, int jugador)
{
    Obstaculo *o = new Obstaculo(resistencia, jugador);
    o->setPos(x, y);
    scene->addItem(o);

    if (jugador == 1) {
        vida1 += resistencia;
        obs1++;
    } else {
        vida2 += resistencia;
        obs2++;
    }
}

void GameController::lanzar(double angulo_deg, double velocidad)
{
    double rad = angulo_deg * M_PI / 180.0;
    double vx = velocidad * std::cos(rad);
    double vy = -velocidad * std::sin(rad);

    double px = (turno == 1) ? 50 : (scene->width() - 50);
    double py = scene->height() - 50;

    Projectile *p = new Projectile(vx, vy, 5);
    p->setPos(px, py);
    scene->addItem(p);

    connect(p, &Projectile::impacto, this, &GameController::procesarImpacto);

    turno = (turno == 1 ? 2 : 1);
    emit turnoCambiado(turno);
}

void GameController::procesarImpacto(int d)
{
    for (auto *item : scene->items()) {
        if (item->data(0).toInt() == 1) {

            Obstaculo *o = static_cast<Obstaculo*>(item);
            int jugador = o->jugadorDueño;

            o->recibirDano(d);

            if (jugador == 1) {
                vida1 -= d;
                if (o->resistencia() <= 0) obs1--;
            } else {
                vida2 -= d;
                if (o->resistencia() <= 0) obs2--;
            }

            emit progreso(vida1, vida2);

            if (obs1 == 0) emit jugadorGana(2);
            if (obs2 == 0) emit jugadorGana(1);

            break;
        }
    }
}
