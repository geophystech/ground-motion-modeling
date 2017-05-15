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

//****************
//*  DEFINES     *
//****************
#define GRID_COLUMNS 3


//****************
//*  INCLUDES    *
//****************

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

//****************
//*  GLOBALS     *
//****************

// number of grid points from grid file
int64_t grid_size_global;

//****************
//*    TYPES     *
//****************

// VS30 data point
typedef struct {
  double lat;
  double lon;
  uint32_t vs30;
} VS30_point;

// earthquake location data
typedef struct {
  double lat;
  double lon;
  double depth;
  double local_magnitude;
  double moment_magnitude;
} Earthquake;

// AS2008 GMPE model data point
typedef struct {
  double *lat;
  double *lon;
  uint32_t *vs30;
  double hyp_distance; // Hypocentral distance in km
  double f_1; // Part of the AS2008 formula responsible for the attenuation from
              // the distance
  double f_5; // Part of the AS2008 formula responsible for the nonlinear
              // behavior of soils
  double f_8; // Part of the AS2008 formula responsible for remote sources
  double pga_1100;
  double g; // Acceleration of gravity in fractions
} AS2008_point;

// Output data point after ALL calculations
typedef struct {
  double *lat;
  double *lon;
  double hyp_distance; // Hypocentral distance in km
  double g;            // Acceleration of gravity in percent
} GMM_point;

//****************
//*  PROTOTYPES  *
//****************

// Read VS30 grid file and return pointer to allocated array of VS30_points
VS30_point *read_vs30_grid(const char *filename);
// AS2008 GMPE
void as2008_gmpe(const Earthquake eq, VS30_point *const vs30_grid,
                 size_t vs30_size, GMM_point *const out_pga_data);
