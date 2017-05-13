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
#include <stdint.h>
#include <stdio.h>

int main(void) {

  struct geod_geodesic g;
  double s12;
  geod_init(&g, 6378137, 1 / 298.257223563); // WGS84
  geod_inverse(&g, 51.716192, 141.733750, 51.318924, 143.451146, &s12, 0, 0);
  printf("%.3f\n", s12);

  Earthquake test_eq = {43.04, 51.92, 13, 6.0};

  printf("lat = %f, lon = %f, depth = %f, Ml = %f, Mw = %f \n", test_eq.lat,
         test_eq.lon, test_eq.depth, test_eq.local_magnitude,
         test_eq.moment_magnitude);

  return 0;
}

void as2008_gmpe(const Earthquake EQ, VS30_point *const VS30_grid,
                 GMM_point *const OUT_pga_data){};
