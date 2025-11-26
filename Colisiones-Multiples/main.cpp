#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include "particle.h"
#include "obstaculo.h"

using namespace std;

pair<double,double> normalObstaculo(const Obstaculo &o, double px, double py) {
    double half = o.lado / 2.0;
    double left = o.x - half;
    double right = o.x + half;
    double top = o.y - half;
    double bottom = o.y + half;

    double dl = fabs(px - left);
    double dr = fabs(right - px);
    double dt = fabs(py - top);
    double db = fabs(bottom - py);

    double mn = dl;
    int side = 0;
    if (dr < mn) { mn = dr; side = 1; }
    if (dt < mn) { mn = dt; side = 2; }
    if (db < mn) { mn = db; side = 3; }

    if (side == 0) return {-1.0, 0.0};
    if (side == 1) return {1.0, 0.0};
    if (side == 2) return {0.0, -1.0};
    return {0.0, 1.0};
}

void colisionPared(Particle &p, double ancho, double alto, ofstream &file, double tiempo) {
    bool tocó = false;
    if (p.x - p.radio < 0) { p.x = p.radio; p.vx *= -1; tocó = true; file << fixed << setprecision(3) << "COL_PARED " << tiempo << " P" << p.id << " LEFT\n"; }
    if (p.x + p.radio > ancho) { p.x = ancho - p.radio; p.vx *= -1; tocó = true; file << fixed << setprecision(3) << "COL_PARED " << tiempo << " P" << p.id << " RIGHT\n"; }
    if (p.y - p.radio < 0) { p.y = p.radio; p.vy *= -1; tocó = true; file << fixed << setprecision(3) << "COL_PARED " << tiempo << " P" << p.id << " TOP\n"; }
    if (p.y + p.radio > alto) { p.y = alto - p.radio; p.vy *= -1; tocó = true; file << fixed << setprecision(3) << "COL_PARED " << tiempo << " P" << p.id << " BOTTOM\n"; }
}

void colisionObstaculo(Particle &p, const Obstaculo &o, double e, ofstream &file, double tiempo) {
    if (!o.colision(p.x, p.y, p.radio)) return;

    // NORMAL DEL OBSTACULO
    auto n = normalObstaculo(o, p.x, p.y);
    double nx = n.first, ny = n.second;

    // COMPONENTE PERPENDICULAR
    double v_dot_n = p.vx * nx + p.vy * ny;
    double vperp_x = v_dot_n * nx;
    double vperp_y = v_dot_n * ny;

    // COMPONENTE PARALELA
    double vpar_x = p.vx - vperp_x;
    double vpar_y = p.vy - vperp_y;

    double vperp_x_n = -e * vperp_x;
    double vperp_y_n = -e * vperp_y;

    // NUEVA VELOCIDAD
    p.vx = vpar_x + vperp_x_n;
    p.vy = vpar_y + vperp_y_n;

    double push = p.radio * 0.5;
    p.x += nx * push;
    p.y += ny * push;

    file << fixed << setprecision(3) << "COL_OBS " << tiempo << " P" << p.id << " OBS" << o.id
         << " N(" << nx << "," << ny << ")\n";
}

void fusionParticulas(Particle &a, Particle &b, ofstream &file, double tiempo) {
    // A Y B ACTIVOS Y CHOCANDO
    double m1 = a.masa, m2 = b.masa;
    double mF = m1 + m2;

    // VELOCIDAD RESULTANTE
    double vxF = (m1 * a.vx + m2 * b.vx) / mF;
    double vyF = (m1 * a.vy + m2 * b.vy) / mF;

    // CENTRO DE MASA COMO POSICION NUEVA
    double xCM = (m1 * a.x + m2 * b.x) / mF;
    double yCM = (m1 * a.y + m2 * b.y) / mF;

    a.masa = mF;
    a.vx = vxF;
    a.vy = vyF;
    a.x = xCM;
    a.y = yCM;

    a.radio = sqrt(a.radio * a.radio + b.radio * b.radio);

    b.activa = false;

    file << fixed << setprecision(3) << "FUSION " << tiempo << " P" << a.id << " += P" << b.id
         << " -> masa=" << a.masa << " vx=" << a.vx << " vy=" << a.vy << "\n";
}

void colisionParticulas(Particle &a, Particle &b, ofstream &file, double tiempo) {
    if (!a.activa || !b.activa) return;

    double dx = b.x - a.x;
    double dy = b.y - a.y;
    double dist = sqrt(dx*dx + dy*dy);

    if (dist <= a.radio + b.radio) {
        fusionParticulas(a, b, file, tiempo);
    }
}

int main() {
    const double dt = 0.05;
    const double ancho = 200.0;
    const double alto = 150.0;
    const double coeffRestitucionObs = 0.6;

    //CREAR PARTICULAS
    vector<Particle> P;
    P.emplace_back(0, 20, 20,  30, 10, 2.0, 4.0);
    P.emplace_back(1, 60, 40, -20, 15, 2.0, 4.0);
    P.emplace_back(2, 100, 80, -10, -25,2.0, 4.0);
    P.emplace_back(3, 150, 30, -15, 20, 2.0, 4.0);

    // 4 OBSTACULOS
    vector<Obstaculo> O;
    O.emplace_back(0, 50, 50, 20);
    O.emplace_back(1, 120, 40, 20);
    O.emplace_back(2, 90, 100, 20);
    O.emplace_back(3, 160, 110, 20);

    ofstream file("salida.txt");
    file << "Simulacion inicio\n";

    const int pasos = 2000;
    for (int paso = 0; paso < pasos; ++paso) {
        double tiempo = paso * dt;
        file << "TIEMPO " << fixed << setprecision(3) << tiempo << "\n";

        // PARTICULA ACTUALIZADA
        for (int i = 0; i < (int)P.size(); ++i) {
            if (!P[i].activa) continue;

            P[i].actualizar(dt);

            // colisión con paredes (elástica)
            //COLISION CON PAREDES ELASTICAS
            colisionPared(P[i], ancho, alto, file, tiempo);

            // COLISION CON OBSTACULOS
            for (const auto &obs : O)
                colisionObstaculo(P[i], obs, coeffRestitucionObs, file, tiempo);
        }

        // cOLISIONES INELASTICAS
        for (int i = 0; i < (int)P.size(); ++i)
            for (int j = i + 1; j < (int)P.size(); ++j)
                colisionParticulas(P[i], P[j], file, tiempo);

        // ESCRIBIR POSICIONES
        for (const auto &p : P) {
            if (!p.activa) continue;
            file << "P" << p.id << " POS " << p.x << " " << p.y << " V " << p.vx << " " << p.vy << " M " << p.masa << "\n";
        }

        file << "-----\n";
    }

    file << "Simulacion fin\n";
    file.close();

    cout << "Simulacion completa. Ver 'salida.txt'.\n";
    return 0;
}
