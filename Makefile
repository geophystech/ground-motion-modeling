#   Copyright 2017 GEOPHYSTECH LLC. All Rights Reserved.
#   Author: Andrey Stepnov
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.

CC = gcc

CFLAGS  = -g -Wall -Werror -Wshadow -march=native -O2 -flto -std=c11

LFLAGS = 

SOURCES = third_party/GeographicLib-1.48/legacy/C/geodesic.c src/gmm.c 

EXECUTABLE = bin/gmm

all:
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCES)

.PHONY : clean
clean :
	-rm $(EXECUTABLE)
