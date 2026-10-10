#include <math.h>
#include <float.h>
#include <stdio.h>
#include "point.h"

void transfer_mass(Point *source, Point *target) {
    target->mass += source->mass;
    source->mass = 0;
}

Point *find_nearest(Point *points, int n, Point *ref) {
    Point *nearest = NULL;
    float min_distance = FLT_MAX;
    for (int i = 0; i < n; i++) {
        if (points[i].mass == 0 || &points[i] == ref) continue;

        float distance = sqrt(pow(points[i].x - ref->x, 2) +
                              pow(points[i].y - ref->y, 2) +
                              pow(points[i].z - ref->z, 2));

        if (distance < min_distance) {
            min_distance = distance;
            nearest = &points[i];
        }
    }
   // if (nearest == NULL) {
   //     printf("Warning: No nearest point found for point (%d, %d, %d)\n", ref->x, ref->y, ref->z);
   // } else {
   //     printf("Debug: Nearest point to (%d, %d, %d) found at (%d, %d, %d) with mass %d\n",
    //           ref->x, ref->y, ref->z, nearest->x, nearest->y, nearest->z, nearest->mass);
   // }
    return nearest;
}

Point *find_lightest(Point *points, int n) {
    Point *lightest = NULL;
    int min_mass = 500;
    for (int i = 0; i < n; i++) {
        if (points[i].mass > 0 && points[i].mass < min_mass) {
            min_mass = points[i].mass;
            lightest = &points[i];
        }
    }
 //   if (lightest == NULL) {
 //       printf("Warning: No lightest point found.\n");
 //   } else {
 //       printf("Debug: Lightest point found at (%d, %d, %d) with mass %d\n",
 //              lightest->x, lightest->y, lightest->z, lightest->mass);
 //   }
    return lightest;
}

