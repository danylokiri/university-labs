#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "point.h"

#define MAX_POINTS 10

void generate_random_points(Point *points, int n) {
    for (int i = 0; i < n; i++) {
        points[i].x = rand() % 50 + 1;
        points[i].y = rand() % 50 + 1;
        points[i].z = rand() % 50 + 1;
        points[i].mass = rand() % 50 + 1;
    }
}

void reduce_points(Point *points, int *n) {
    int initial_total_mass = 0;
    for (int i = 0; i < *n; i++) {
        initial_total_mass += points[i].mass;
    }

    int active_count = *n;

    while (active_count > 1) {
        Point *lightest = find_lightest(points, *n);

      //  if (lightest == NULL) {
       //     printf("Warning: No lightest point found. Remaining active points:\n");
       //     for (int i = 0; i < *n; i++) {
       //         if (points[i].mass > 0) {
       //             printf("Point (%d, %d, %d) with mass %d\n", points[i].x, points[i].y, points[i].z, points[i].mass);
       //         }
        //    }
        //    break;
      //  }

        Point *nearest = find_nearest(points, *n, lightest);
      //  if (nearest == NULL) {
      //      printf("Error: Could not find the nearest point.\n");
      //      return;
       // }

        transfer_mass(lightest, nearest);

        active_count = 0;
        for (int i = 0; i < *n; i++) {
            if (points[i].mass > 0) {
                active_count++;
            }
        }

        if (active_count <= 1) {
            break;
        }
    }

    int final_mass = 0;
    for (int i = 0; i < *n; i++) {
        if (points[i].mass > 0) {
            final_mass += points[i].mass;
        }
    }

 //   if (final_mass != initial_total_mass) {
  //      printf("Error: Final mass does not match initial total mass.\n");
  //  } else {
  //      printf("Final mass matches the initial total mass.\n");
   // }
}




int main() {
    srand(time(NULL));

    int n = rand() % MAX_POINTS + 2;
    if (n > MAX_POINTS) n = MAX_POINTS;

    Point points[MAX_POINTS];

    generate_random_points(points, n);

    printf("Generated Points:\n");
    for (int i = 0; i < n; i++) {
        printf("Point %d: (%d, %d, %d) with mass %d\n", i, points[i].x, points[i].y, points[i].z, points[i].mass);
    }

    reduce_points(points, &n);

    printf("\nRemaining Point:\n");
    for (int i = 0; i < MAX_POINTS; i++) {
        if (points[i].mass > 0) {
            printf("Point: (%d, %d, %d) with mass %d\n",
                   points[i].x, points[i].y, points[i].z, points[i].mass);
            break;
        }
    }

    return 0;
}
