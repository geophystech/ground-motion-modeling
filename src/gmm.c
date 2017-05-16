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
#include <math.h>
#include <stdlib.h>

int main(void) {

  // test case
  Earthquake eq_location = {51.92, 143.04, 13, 6.0};
  Earthquake *ptr_eq_location = &eq_location;

  printf("\n--------------EARTHQUAKE LOCATION--------------- \n lat = %f\tlon "
         "= %f\n depth = %f\tMl = %f\n Mw = %f \n",
         ptr_eq_location->lat, ptr_eq_location->lon, ptr_eq_location->depth,
         ptr_eq_location->local_magnitude, ptr_eq_location->moment_magnitude);

  AS2008_parameters as2008_conf = {
      0.804,  -0.9679, 0.265, -0.231, -0.398, -0.0372, 0.9445, -0.0067,
      -1.186, 1.88,    6.75,  4.5,    1.18,   865.1,   1500,   1100};
  AS2008_parameters *ptr_as2008_conf = &as2008_conf;

  print_as2008_parameters(ptr_as2008_conf, NULL);

  char *vs30_file_config = "examples/grids/vs30_test.xyz";

  // get pointer to allocated vs30 grid
  VS30_point *vs30_grid = read_vs30_grid(vs30_file_config);

  // Using AS2008 model
  GMM_point_pga *pga_grid = as2008_gmpe(eq_location, as2008_conf, vs30_grid);

  // debugs
  printf("\nNumber of lines in grid file = %ld\n", grid_size_global);
  print_gpa_grid(pga_grid, NULL);

  // printf("SIZE of vs30_grid is %lu\nNUMBER of elements is %lu\n",
  //       sizeof *vs30_grid, (sizeof *vs30_grid) / (sizeof vs30_grid[0]));
  for (size_t i = 0; i < grid_size_global; i++) {
    printf("%lf %lf %lf\n", vs30_grid[i].lon, vs30_grid[i].lat,
           vs30_grid[i].vs30);
  };

  // free and exit
  free(vs30_grid);
  free(pga_grid);

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

  // Parse vs30 grid file and filling vs30 grid array
  size_t line_number = 1;
  size_t line_len = 0;
  char *parsing_line = NULL;
  for (size_t i = 0; i < grid_size_global; i++) {
    getline(&parsing_line, &line_len, vs30_grid_file);
    if ((sscanf(parsing_line, "%lf %lf %lf\n", &vs30_point_array[i].lon,
                &vs30_point_array[i].lat, &vs30_point_array[i].vs30)) !=
        GRID_COLUMNS) {
      fprintf(stderr, "ERROR: LINE %ld at input vs30 grid file has wrong "
                      "format. Please use LAT LON VS30 order",
              line_number);
      exit(EXIT_FAILURE);
    } else {
      line_number++;
    };
  };
  free(parsing_line);

  // close file and return pointer
  fclose(vs30_grid_file);
  return vs30_point_array;
};

GMM_point_pga *as2008_gmpe(const Earthquake eq, AS2008_parameters as2008_in,
                           VS30_point *const vs30_grid) {

  // memory allocation for AS2008 points
  AS2008_point *as2008_point_array;
  as2008_point_array =
      (AS2008_point *)calloc(grid_size_global, sizeof(AS2008_point));

  //
  // Preparing variables to run AS2008 modeling
  //
  struct geod_geodesic geoid;                    // Init geodesic routines
  geod_init(&geoid, 6378137, 1 / 298.257223563); // WGS84
  double epicentral_distance;                    // for holding in loops

  double magnitude;
  (eq.moment_magnitude == 0) ? (magnitude = eq.local_magnitude)
                             : (magnitude = eq.moment_magnitude);

  if (magnitude < 5.5) {
    as2008_in.t6 = 1;
  };
  if (magnitude <= 6.5 && magnitude >= 5.5) {
    as2008_in.t6 = (0.5 * (6.5 - magnitude) + 0.5);
  };
  if (magnitude > 6.5) {
    as2008_in.t6 = 0.5;
  };

  //
  // AS2008 PGA modeling
  //
  for (size_t i = 0; i < grid_size_global; i++) {

    // hypocentral distance
    as2008_point_array[i].lon = &vs30_grid[i].lon;
    as2008_point_array[i].lat = &vs30_grid[i].lat;
    as2008_point_array[i].vs30 = &vs30_grid[i].vs30;
    geod_inverse(&geoid, eq.lat, eq.lon, *as2008_point_array[i].lat,
                 *as2008_point_array[i].lon, &epicentral_distance, 0, 0);
    as2008_point_array[i].hyp_distance =
        sqrt(pow((epicentral_distance / 1000), 2) + pow(eq.depth, 2));

    // f1
    if (magnitude <= as2008_in.c1) {
      as2008_point_array[i].f1 =
          as2008_in.a1 + as2008_in.a4 * (magnitude - as2008_in.c1) +
          as2008_in.a8 * pow((8.5 - magnitude), 2) +
          (as2008_in.a2 + as2008_in.a3 * (magnitude - as2008_in.c1)) *
              log(sqrt(pow(as2008_point_array[i].hyp_distance, 2) +
                       pow(as2008_in.c4, 2)));
    } else {
      as2008_point_array[i].f1 =
          as2008_in.a1 + as2008_in.a5 * (magnitude - as2008_in.c1) +
          as2008_in.a8 * pow((8.5 - magnitude), 2) +
          (as2008_in.a2 + as2008_in.a3 * (magnitude - as2008_in.c1)) *
              log(sqrt(pow(as2008_point_array[i].hyp_distance, 2) +
                       pow(as2008_in.c4, 2)));
    }

    // f8
    if (as2008_point_array[i].hyp_distance < 100) {
      as2008_point_array[i].f8 = 0;
    } else {
      as2008_point_array[i].f8 = as2008_in.a18 *
                                 (as2008_point_array[i].hyp_distance - 100) *
                                 as2008_in.t6;
    };

    // pga 1100
    as2008_point_array[i].pga1100 =
        exp((as2008_point_array[i].f1 +
             (as2008_in.a10 + as2008_in.b * as2008_in.n) *
                 log(as2008_in.vs30_1100 / as2008_in.vlin) +
             as2008_point_array[i].f8));

    // f5
    if (*as2008_point_array[i].vs30 < as2008_in.vlin) {
      as2008_point_array[i].f5 =
          as2008_in.a10 * log(*as2008_point_array[i].vs30 / as2008_in.vlin) -
          as2008_in.b * log(as2008_point_array[i].pga1100 + as2008_in.c) +
          as2008_in.b * log(as2008_point_array[i].pga1100 +
                            as2008_in.c * pow((*as2008_point_array[i].vs30 /
                                               as2008_in.vlin),
                                              as2008_in.n));
    } else if (*as2008_point_array[i].vs30 > as2008_in.vlin &&
               *as2008_point_array[i].vs30 < as2008_in.v1) {
      as2008_point_array[i].f5 =
          (as2008_in.a10 + as2008_in.b * as2008_in.n) *
          log(*as2008_point_array[i].vs30 / as2008_in.vlin);
    } else {
      as2008_point_array[i].f5 = (as2008_in.a10 + as2008_in.b * as2008_in.n) *
                                 log(as2008_in.v1 / as2008_in.vlin);
    };

    // pga
    as2008_point_array[i].g =
        exp(as2008_point_array[i].f1 + as2008_point_array[i].f5 +
            as2008_point_array[i].f8);
  };

  // memory allocation for PGA points
  GMM_point_pga *pga_point_array;
  pga_point_array =
      (GMM_point_pga *)calloc(grid_size_global, sizeof(GMM_point_pga));

  // collect ouput PGA grid
  for (size_t i = 0; i < grid_size_global; i++) {
    pga_point_array[i].lon = &vs30_grid[i].lon;
    pga_point_array[i].lat = &vs30_grid[i].lat;
    pga_point_array[i].hyp_distance = as2008_point_array[i].hyp_distance;
    pga_point_array[i].g = as2008_point_array[i].g * 100;
  };

  print_as2008_points(as2008_point_array, NULL);

  // free(geoid);
  free(as2008_point_array);

  return pga_point_array;
};

void print_as2008_points(const AS2008_point *const AS2008_point_array_const,
                         const char *const filename) {

  if (filename != 0) {
    // write to file log
  } else {

    // print to stdout
    printf("\n----------------AS2008 POINTS------------------- \n"
           "%s \t\t%s \t\t%s \t\t%s \t\t%s \t\t%s \t\t%s \t\t%s \t%s\n",
           "lon", "lat", "vs30", "hyp_distance", "f1", "f5", "f8", "pga1100",
           "g");
    for (size_t i = 0; i < grid_size_global; i++) {
      printf("%lf \t%lf \t%lf \t%lf \t\t%lf \t%lf \t%lf \t%lf \t%lf\n",
             *AS2008_point_array_const[i].lon, *AS2008_point_array_const[i].lat,
             *AS2008_point_array_const[i].vs30,
             AS2008_point_array_const[i].hyp_distance,
             AS2008_point_array_const[i].f1, AS2008_point_array_const[i].f5,
             AS2008_point_array_const[i].f8,
             AS2008_point_array_const[i].pga1100,
             AS2008_point_array_const[i].g);
    };
  };
};

void print_as2008_parameters(const AS2008_parameters *const as2008_conf,
                             const char *const filename) {

  if (filename != NULL) {
    // print to log file
  } else {
    // print to stdout
    printf(
        "\n--------------AS2008 PARAMETERS----------------- \n a1 = %lf\t\ta2 "
        "= %lf\n a3 = %lf\t\ta4 = %lf\n a5 = %lf\t\ta8 = %lf\n a10 = "
        "%lf\t\ta18 = %lf\n b = %lf\t\tc = %lf\n c1 = %lf\t\tc4 = %lf\n n = "
        "%lf\t\tvlin = %lf\n v1 = %5.2lf\t\tvs30_1100 = %5.2lf\n",
        as2008_conf->a1, as2008_conf->a2, as2008_conf->a3, as2008_conf->a4,
        as2008_conf->a5, as2008_conf->a8, as2008_conf->a10, as2008_conf->a18,
        as2008_conf->b, as2008_conf->c, as2008_conf->c1, as2008_conf->c4,
        as2008_conf->n, as2008_conf->vlin, as2008_conf->v1,
        as2008_conf->vs30_1100);
  }
};

void print_gpa_grid(const GMM_point_pga *const pga_grid_const,
                    const char *const filename) {

  if (filename != NULL) {
    // print to log file
  } else {
    // print to stdout
    printf("\n------------------ PGA GRID--------------------- \n"
           "%s \t\t%s \t\t%s \t%s \n",
           "lon", "lat", "hyp_distance", "g%");

    for (size_t i = 0; i < grid_size_global; i++) {
      printf("%lf \t%lf \t%lf \t%3.2lf\n", *pga_grid_const[i].lon,
             *pga_grid_const[i].lat, pga_grid_const[i].hyp_distance,
             pga_grid_const[i].g);
    };
  };
};
