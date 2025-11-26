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
