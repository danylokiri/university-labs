#ifndef POINT_H
#define POINT_H

typedef struct {
    int x;
    int y;
    int z;
    int mass;
} Point;

void transfer_mass(Point *source, Point *target);
Point *find_nearest(Point *points, int n, Point *ref);
Point *find_lightest(Point *points, int n);

#endif
