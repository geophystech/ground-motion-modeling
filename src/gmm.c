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
#include <time.h>

int main(int argc, char **argv) {

  // test case
  Earthquake eq_location = {51.92, 143.04, 13, 6.0};
  Earthquake *ptr_eq_location = &eq_location;

  printf("\n--------------EARTHQUAKE LOCATION--------------- \n lat = %f\tlon "
         "= %f\n depth = %f\tMl = %f\n Mw = %f \n",
         ptr_eq_location->lat, ptr_eq_location->lon, ptr_eq_location->depth,
         ptr_eq_location->local_magnitude, ptr_eq_location->moment_magnitude);

  DESKTOP_parameters desktop_conf = {0};
  DESKTOP_parameters *ptr_desktop_conf = &desktop_conf;

  AS2008_parameters as2008_conf = {0};
  AS2008_parameters *ptr_as2008_conf = &as2008_conf;

  if (argv[1] == NULL) {
    printf(" ERROR: No configuration file specified\n Please use:\n \tgmm "
           "<path_to_config_file>\n");
    exit(EXIT_FAILURE);

  } else {
    parse_config_file_desktop(argv[1], ptr_desktop_conf, ptr_as2008_conf);
  };

  print_as2008_parameters(ptr_as2008_conf, NULL);

  // get pointer to allocated vs30 grid
  clock_t begin = clock();
  VS30_point *vs30_grid = read_vs30_grid(ptr_desktop_conf->vs30_file_config);
  clock_t end = clock();
  printf("\nTime to read vs30 file takes %lf seconds \n",
         (double)(end - begin) / CLOCKS_PER_SEC);

  printf("\nTotal lines in grid file = %ld\n", grid_size_global);

  // Using AS2008 model
  begin = clock();
  GMM_point_pga *pga_grid = as2008_gmpe(eq_location, as2008_conf, vs30_grid);
  end = clock();
  printf("\nTime to AS2008 modeling file takes %lf seconds \n",
         (double)(end - begin) / CLOCKS_PER_SEC);

  // debugs
  print_gpa_grid(pga_grid, NULL);

  // for (size_t i = 0; i < grid_size_global; i++) {
  //  printf("%lf %lf %lf\n", vs30_grid[i].lon, vs30_grid[i].lat,
  //         vs30_grid[i].vs30);
  //};

  // free and exit
  free(vs30_grid);
  free(pga_grid);

  exit(EXIT_SUCCESS);
}

void print_as2008_points(const AS2008_point *const AS2008_point_array_const,
                         const char *const filename) {

  if (filename != 0) {
    // write to file log
  } else {

    // print to stdout
    printf("\n----------------AS2008 POINTS------------------- \n"
           "%s \t\t%s \t\t%s \t\t%s \t\t%s \t\t%s \t\t%s \t\t%s \t%s\n",
           "lon", "lat", "vs30", "r_rup", "f1", "f5", "f8", "pga1100", "g");
    for (size_t i = 0; i < grid_size_global; i++) {
      printf("%lf \t%lf \t%lf \t%lf \t\t%lf \t%lf \t%lf \t%lf \t%lf\n",
             *AS2008_point_array_const[i].lon, *AS2008_point_array_const[i].lat,
             *AS2008_point_array_const[i].vs30,
             AS2008_point_array_const[i].r_rup, AS2008_point_array_const[i].f1,
             AS2008_point_array_const[i].f5, AS2008_point_array_const[i].f8,
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
           "%s \t\t%s \t\t%s \t\t%s \n",
           "lon", "lat", "r_rup", "g%");

    for (size_t i = 0; i < grid_size_global; i++) {
      printf("%lf \t%lf \t%lf \t%3.2lf\n", *pga_grid_const[i].lon,
             *pga_grid_const[i].lat, pga_grid_const[i].r_rup,
             pga_grid_const[i].g);
    };
  };
};
