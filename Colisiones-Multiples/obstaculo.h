#ifndef OBSTACULO_H
#define OBSTACULO_H

class Obstaculo {
public:
    double x, y;
    double lado;
    int id;

    Obstaculo() = default;
    Obstaculo(int id, double x, double y, double lado) : id(id), x(x), y(y), lado(lado) {}

    bool colision(double px, double py, double radio) const {
        double half = lado / 2.0;
        return (px + radio > x - half &&
                px - radio < x + half &&
                py + radio > y - half &&
                py - radio < y + half);
    }
};

#endif // OBSTACULO_H
