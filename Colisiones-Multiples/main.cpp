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
