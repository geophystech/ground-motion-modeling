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

    // Rrup (hypocentral distance)
    as2008_point_array[i].lon = &vs30_grid[i].lon;
    as2008_point_array[i].lat = &vs30_grid[i].lat;
    as2008_point_array[i].vs30 = &vs30_grid[i].vs30;
    geod_inverse(&geoid, eq.lat, eq.lon, *as2008_point_array[i].lat,
                 *as2008_point_array[i].lon, &epicentral_distance, 0, 0);
    as2008_point_array[i].r_rup =
        sqrt(pow((epicentral_distance / 1000), 2) + pow(eq.depth, 2));

    // f1
    if (magnitude <= as2008_in.c1) {
      as2008_point_array[i].f1 =
          as2008_in.a1 + as2008_in.a4 * (magnitude - as2008_in.c1) +
          as2008_in.a8 * pow((8.5 - magnitude), 2) +
          (as2008_in.a2 + as2008_in.a3 * (magnitude - as2008_in.c1)) *
              log(sqrt(pow(as2008_point_array[i].r_rup, 2) +
                       pow(as2008_in.c4, 2)));
    } else {
      as2008_point_array[i].f1 =
          as2008_in.a1 + as2008_in.a5 * (magnitude - as2008_in.c1) +
          as2008_in.a8 * pow((8.5 - magnitude), 2) +
          (as2008_in.a2 + as2008_in.a3 * (magnitude - as2008_in.c1)) *
              log(sqrt(pow(as2008_point_array[i].r_rup, 2) +
                       pow(as2008_in.c4, 2)));
    }

    // f8
    if (as2008_point_array[i].r_rup < 100) {
      as2008_point_array[i].f8 = 0;
    } else {
      as2008_point_array[i].f8 = as2008_in.a18 *
                                 (as2008_point_array[i].r_rup - 100) *
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
    pga_point_array[i].hyp_distance = as2008_point_array[i].r_rup;
    pga_point_array[i].g = as2008_point_array[i].g * 100;
  };

  // print_as2008_points(as2008_point_array, NULL);

  // free(geoid);
  free(as2008_point_array);

  return pga_point_array;
};

