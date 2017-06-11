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
#define PGA_GRID_FILE_NAME "gmm_pga_out.xyz"

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

// desktop config
typedef struct {
  char *vs30_file_config;
  char *base_model;
} DESKTOP_parameters;

// VS30 data point
typedef struct {
  double lat;
  double lon;
  double vs30;
} VS30_point;

// s-file parameters
// http://seis.geus.net/software/seisan/node226.html
typedef struct {
  uint16_t year;
  uint16_t month;
  uint16_t day;
  char fix_o_time;
  uint16_t hour;
  uint16_t minute;
  double second;
  char loc_model_ind;
  char distance_ind;
  char event_id;
  double lat;
  double lon;
  double depth;
  char depth_ind;
  char loc_ind;
  char *agency;
  uint16_t sta_num;
  double rms;
  double m1;
  char m1_type;
  char *m1_agency;
  double m2;
  char m2_type;
  char *m2_agency;
  double m3;
  char m3_type;
  char m3_agency;
} S_file;

// earthquake location data
typedef struct {
  double lat;
  double lon;
  double depth;
  double local_magnitude;
  double moment_magnitude;
} Earthquake;

// Output data point after ALL calculations
typedef struct {
  double *lat;
  double *lon;
  double r_rup; // Hypocentral distance in km
  double g;     // Acceleration of gravity in percent rounded to ggg.gg
} GMM_point_pga;

// AS2008 formula paramaters
typedef struct {
  double a1;
  double a2;
  double a3;
  double a4;
  double a5;
  double a8;
  double a10;
  double a18;
  double b;
  double c;
  double c1;
  double c4;
  double n;
  double vlin;
  double v1;
  double vs30_1100;
  double t6;
} AS2008_parameters;

// AS2008 GMPE model data point
typedef struct {
  double *lat;
  double *lon;
  double *vs30;
  double r_rup; // Hypocentral distance in km
  double f1;
  double f5;
  double f8;
  double pga1100;
  double g; // Acceleration of gravity in fractions
} AS2008_point;

//****************
//*  PROTOTYPES  *
//****************

// Read VS30 grid file and return pointer to allocated array of VS30_points
VS30_point *read_vs30_grid(const char *filename);
// Read seisan S-file
S_file read_s_file(const char *filename);
// Parse config file
size_t parse_config_file_desktop(const char *filename,
                                 DESKTOP_parameters *desktop_conf,
                                 AS2008_parameters *ptr_as2008_conf);
// AS2008 GMPE
// void as2008_gmpe(const Earthquake eq, VS30_point *const vs30_grid,
//                 size_t vs30_size, GMM_point *const out_pga_data);
GMM_point_pga *as2008_gmpe(const Earthquake eq, AS2008_parameters as2008_in,
                           VS30_point *const vs30_grid);
void print_as2008_parameters(const AS2008_parameters *const AS2008_conf,
                             const char *const filename);
void print_as2008_points(const AS2008_point *const AS2008_point_array_const,
                         const char *const filename);
void print_pga_grid(const GMM_point_pga *const pga_grid_const,
                    const char *const filename);
