#ifndef _point_h_
#define _point_h_

#include <string.h>

//Point in d-dimensional space
struct Point {
    double * coords;
    int d;
    Point(int d, double * coords) {
        this->d = d;
        coords = new double[d];
        memcpy(this->coords, coords, sizeof(double) * d);
    }
    Point(int d) {
        this->d = d;
        coords = new double[d];
    }
    ~Point() {
        delete[] coords;
    }
    double & operator[](int i) {
        return coords[i];
    }
    bool operator<(const Point & rhs) {
        for (int i = 0; i < d; i++) {
            if (coords[i] < rhs.coords[i])
                return true;
        }
        return false;
    }
};

#endif
