#ifndef PARTICLE_H
#define PARTICLE_H

#include <cmath>

class Particle {
public:
    double x, y;
    double vx, vy;
    double masa;
    double radio;
    bool activa = true;
    int id;

    Particle() = default;

    Particle(int id, double x, double y, double vx, double vy, double masa, double radio)
        : id(id), x(x), y(y), vx(vx), vy(vy), masa(masa), radio(radio) {}

    void actualizar(double dt) {
        x += vx * dt;
        y += vy * dt;
    }

    double velocidadMagnitud() const {
        return std::sqrt(vx*vx + vy*vy);
    }
};

#endif // PARTICLE_H
