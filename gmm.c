
#include "GeographicLib-1.48/legacy/C/geodesic.h"
#include "gmm.h"
#include "stdio.h"

int main(void) {

  struct geod_geodesic g;
  double s12;
  geod_init(&g, 6378137, 1 / 298.257223563); // WGS84
  geod_inverse(&g, 51.716192, 141.733750, 51.318924, 143.451146, &s12, 0, 0);
  printf("%.3f\n", s12);

  return 0;
}
