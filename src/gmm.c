//   Copyright 2017 GEOPHYSTECH LLC. All Rights Reserved.
//   Author: Andrey Stepnov
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
#include "../third_party/GeographicLib-1.48/legacy/C/geodesic.h"
#include "gmm.h"
#include <stdlib.h>

int main(void) {

  // test case
  Earthquake test_eq = {43.04, 51.92, 13, 6.0};
  printf("lat = %f, lon = %f, depth = %f, Ml = %f, Mw = %f \n", test_eq.lat,
         test_eq.lon, test_eq.depth, test_eq.local_magnitude,
         test_eq.moment_magnitude);

  char *testVS30_1 = "examples/testVS30_1.txt";

  // get pointer to allocated vs30 grid
  VS30_point *vs30_grid = read_vs30_grid(testVS30_1);

  // debugs
  printf("Number of lines = %ld\n", grid_size_global);

  // printf("SIZE of vs30_grid is %lu\nNUMBER of elements is %lu\n",
  //       sizeof *vs30_grid, (sizeof *vs30_grid) / (sizeof vs30_grid[0]));
  for (size_t i = 0; i < grid_size_global; i++) {
    printf("%lf %lf %d\n", vs30_grid[i].lon, vs30_grid[i].lat,
           vs30_grid[i].vs30);
  };

  struct geod_geodesic g;
  double s12;
  geod_init(&g, 6378137, 1 / 298.257223563); // WGS84
  geod_inverse(&g, 51.716192, 141.733750, 51.318924, 143.451146, &s12, 0, 0);
  printf("%.3f\n", s12);

  // free and exit
  free(vs30_grid);

  return 0;
}

VS30_point *read_vs30_grid(const char *filename) {

  // open file
  FILE *vs30_grid_file;
  if ((vs30_grid_file = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "ERROR: VS30 grid file can not be openned");
    exit(EXIT_FAILURE);
  }

  // count of vs30 grid size
  int ch;
  while (!feof(vs30_grid_file)) {
    ch = fgetc(vs30_grid_file);
    if (ch == '\n') {
      grid_size_global++;
    }
  }
  rewind(vs30_grid_file);

  // memory allocation for vs30 grid
  VS30_point *vs30_point_array;
  vs30_point_array = (VS30_point *)calloc(grid_size_global, sizeof(VS30_point));
  if (vs30_point_array == NULL) {
    fprintf(stderr, "ERROR: Can not memory allocation for VS30 grid");
    exit(EXIT_FAILURE);
  }

  // filling vs30 grid array
  for (size_t i = 0; i < grid_size_global; i++) {
    fscanf(vs30_grid_file, "%lf %lf %d", &vs30_point_array[i].lon,
           &vs30_point_array[i].lat, &vs30_point_array[i].vs30);
  };

  // close file and return pointer
  fclose(vs30_grid_file);
  return vs30_point_array;
};

void as2008_gmpe(const Earthquake eq, VS30_point *const vs30_grid,
                 size_t vs30_size, GMM_point *const out_pga_data){};
