# C65
# Copyright (C) 2019 David Jolly
#
# C65 is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# C65 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

DIR_BUILD=../../build/test/
DIR_INC=../include/
DIR_INC_TYPE=../include/type/
DIR_SRC=./
DIR_SRC_TYPE=./type/
FLAGS=-march=native -std=c++17 -Wall -Werror
LIB=libc65-test.a

all: build archive

### LIBRARY ###

archive:
	@echo ''
	@echo '--- BUILDING LIBRARY -----------------------'
	ar rcs $(DIR_BUILD)$(LIB) $(DIR_BUILD)base_c65.o \
		$(DIR_BUILD)type_fixture.o
	@echo '--- DONE -----------------------------------'

build: build_base build_type

### BASE ###

build_base: base_c65.o

base_c65.o: $(DIR_SRC)c65.cpp $(DIR_INC)c65.h
	$(CXX) $(FLAGS) $(BUILD_FLAGS) $(TRACE_FLAGS) -c $(DIR_SRC)c65.cpp -o $(DIR_BUILD)base_c65.o

### TYPE ###

build_type: type_fixture.o

type_fixture.o: $(DIR_SRC_TYPE)fixture.cpp $(DIR_INC_TYPE)fixture.h
	$(CXX) $(FLAGS) $(BUILD_FLAGS) $(TRACE_FLAGS) -c $(DIR_SRC_TYPE)fixture.cpp -o $(DIR_BUILD)type_fixture.o
