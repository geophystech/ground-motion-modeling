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
#include <glib.h>
#include <stdlib.h>

size_t parse_config_file_desktop(const char *filename,
                                 DESKTOP_parameters *ptr_desktop_conf,
                                 AS2008_parameters *ptr_as2008_conf) {
  printf("\nREADING config file '%s'\n", filename);

  // for details please see gnome developers manual
  // https://developer.gnome.org/glib/stable/glib-Key-value-file-parser.html
  GKeyFile *config_file;
  GError *gerror = NULL;

  config_file = g_key_file_new();
  if (g_key_file_load_from_file(config_file, filename, G_KEY_FILE_NONE,
                                &gerror) == 0) {
    printf("\nERROR (%d) while reading config file: %s\n", gerror->code,
           gerror->message);
    g_error_free(gerror);
    exit(EXIT_FAILURE);
  }

  //
  // DESKTOP parameters
  //
  char *desktop = "DESKTOP";

  char *desktop_vs30_file_config = "vs30_file";
  ptr_desktop_conf->vs30_file_config = g_key_file_get_string(
      config_file, desktop, desktop_vs30_file_config, &gerror);
  if (gerror != NULL) {
    printf("\nerror (%d) while reading config: %s\n", gerror->code,
           gerror->message);
    g_error_free(gerror);
    exit(EXIT_FAILURE);
  }

  //
  // AS2008 parameters
  //
  char *as2008 = "AS2008";

  char *as2008_a1 = "a1";
  ptr_as2008_conf->a1 =
      g_key_file_get_double(config_file, as2008, as2008_a1, &gerror);
  if (gerror != NULL) {
    printf("\nerror (%d) while reading config: %s\n", gerror->code,
           gerror->message);
    g_error_free(gerror);
    exit(EXIT_FAILURE);
  }

  char *as2008_a2 = "a2";
  ptr_as2008_conf->a2 =
      g_key_file_get_double(config_file, as2008, as2008_a2, &gerror);
  if (gerror != NULL) {
    printf("\nerror (%d) while reading config: %s\n", gerror->code,
           gerror->message);
    g_error_free(gerror);
    exit(EXIT_FAILURE);
  }

  char *as2008_a3 = "a3";
  ptr_as2008_conf->a3 =
      g_key_file_get_double(config_file, as2008, as2008_a3, &gerror);
  if (gerror != NULL) {
    printf("\nerror (%d) while reading config: %s\n", gerror->code,
           gerror->message);
    g_error_free(gerror);
    exit(EXIT_FAILURE);
  }

  char *as2008_a4 = "a4";
  ptr_as2008_conf->a4 =
      g_key_file_get_double(config_file, as2008, as2008_a4, &gerror);
  if (gerror != NULL) {
    printf("\nerror (%d) while reading config: %s\n", gerror->code,
           gerror->message);
    g_error_free(gerror);
    exit(EXIT_FAILURE);
  }

  char *as2008_a5 = "a5";
  ptr_as2008_conf->a5 =
      g_key_file_get_double(config_file, as2008, as2008_a5, &gerror);
  if (gerror != NULL) {
    printf("\nerror (%d) while reading config: %s\n", gerror->code,
           gerror->message);
    g_error_free(gerror);
    exit(EXIT_FAILURE);
  }

  char *as2008_a8 = "a8";
  ptr_as2008_conf->a8 =
      g_key_file_get_double(config_file, as2008, as2008_a8, &gerror);
  if (gerror != NULL) {
    printf("\nerror (%d) while reading config: %s\n", gerror->code,
           gerror->message);
    g_error_free(gerror);
    exit(EXIT_FAILURE);
  }

  char *as2008_a10 = "a10";
  ptr_as2008_conf->a10 =
      g_key_file_get_double(config_file, as2008, as2008_a10, &gerror);
  if (gerror != NULL) {
    printf("\nerror (%d) while reading config: %s\n", gerror->code,
           gerror->message);
    g_error_free(gerror);
    exit(EXIT_FAILURE);
  }

  char *as2008_a18 = "a18";
  ptr_as2008_conf->a18 =
      g_key_file_get_double(config_file, as2008, as2008_a18, &gerror);
  if (gerror != NULL) {
    printf("\nerror (%d) while reading config: %s\n", gerror->code,
           gerror->message);
    g_error_free(gerror);
    exit(EXIT_FAILURE);
  }

  char *as2008_b = "b";
  ptr_as2008_conf->b =
      g_key_file_get_double(config_file, as2008, as2008_b, &gerror);
  if (gerror != NULL) {
    printf("\nerror (%d) while reading config: %s\n", gerror->code,
           gerror->message);
    g_error_free(gerror);
    exit(EXIT_FAILURE);
  }

  char *as2008_c = "c";
  ptr_as2008_conf->c =
      g_key_file_get_double(config_file, as2008, as2008_c, &gerror);
  if (gerror != NULL) {
    printf("\nerror (%d) while reading config: %s\n", gerror->code,
           gerror->message);
    g_error_free(gerror);
    exit(EXIT_FAILURE);
  }

  char *as2008_c1 = "c1";
  ptr_as2008_conf->c1 =
      g_key_file_get_double(config_file, as2008, as2008_c1, &gerror);
  if (gerror != NULL) {
    printf("\nerror (%d) while reading config: %s\n", gerror->code,
           gerror->message);
    g_error_free(gerror);
    exit(EXIT_FAILURE);
  }

  char *as2008_c4 = "c4";
  ptr_as2008_conf->c4 =
      g_key_file_get_double(config_file, as2008, as2008_c4, &gerror);
  if (gerror != NULL) {
    printf("\nerror (%d) while reading config: %s\n", gerror->code,
           gerror->message);
    g_error_free(gerror);
    exit(EXIT_FAILURE);
  }

  char *as2008_n = "n";
  ptr_as2008_conf->n =
      g_key_file_get_double(config_file, as2008, as2008_n, &gerror);
  if (gerror != NULL) {
    printf("\nerror (%d) while reading config: %s\n", gerror->code,
           gerror->message);
    g_error_free(gerror);
    exit(EXIT_FAILURE);
  }

  char *as2008_vlin = "vlin";
  ptr_as2008_conf->vlin =
      g_key_file_get_double(config_file, as2008, as2008_vlin, &gerror);
  if (gerror != NULL) {
    printf("\nerror (%d) while reading config: %s\n", gerror->code,
           gerror->message);
    g_error_free(gerror);
    exit(EXIT_FAILURE);
  }

  char *as2008_v1 = "v1";
  ptr_as2008_conf->v1 =
      g_key_file_get_double(config_file, as2008, as2008_v1, &gerror);
  if (gerror != NULL) {
    printf("\nerror (%d) while reading config: %s\n", gerror->code,
           gerror->message);
    g_error_free(gerror);
    exit(EXIT_FAILURE);
  }

  char *as2008_vs30_1100 = "vs30_1100";
  ptr_as2008_conf->vs30_1100 =
      g_key_file_get_double(config_file, as2008, as2008_vs30_1100, &gerror);
  if (gerror != NULL) {
    printf("\nerror (%d) while reading config: %s\n", gerror->code,
           gerror->message);
    g_error_free(gerror);
    exit(EXIT_FAILURE);
  }

  g_key_file_free(config_file);

  return (EXIT_SUCCESS);
};

VS30_point *read_vs30_grid(const char *filename) {

  // open file
  FILE *vs30_grid_file;
  if ((vs30_grid_file = fopen(filename, "r")) == NULL) {
    perror("\nERROR: VS30 grid file can not be openned");
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
