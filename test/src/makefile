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
DIR_INC_SYSTEM=../include/system/
DIR_INC_TYPE=../include/type/
DIR_SRC=./
DIR_SRC_SYSTEM=./system/
DIR_SRC_TYPE=./type/
FLAGS=-march=native -std=c++17 -Wall -Werror
LIB=libc65-test.a

all: build archive

### LIBRARY ###

archive:
	@echo ''
	@echo '--- BUILDING LIBRARY -----------------------'
	ar rcs $(DIR_BUILD)$(LIB) $(DIR_BUILD)base_c65.o \
		$(DIR_BUILD)system_memory.o $(DIR_BUILD)system_processor.o $(DIR_BUILD)system_video.o \
		$(DIR_BUILD)type_buffer.o
	@echo '--- DONE -----------------------------------'

build: build_base build_system build_type

### BASE ###

build_base: base_c65.o

base_c65.o: $(DIR_SRC)c65.cpp $(DIR_INC)c65.h
	$(CXX) $(FLAGS) $(BUILD_FLAGS) $(TRACE_FLAGS) -c $(DIR_SRC)c65.cpp -o $(DIR_BUILD)base_c65.o

### TYPE ###

build_system: system_memory.o system_processor.o system_video.o

system_memory.o: $(DIR_SRC_SYSTEM)memory.cpp $(DIR_INC_SYSTEM)memory.h
	$(CXX) $(FLAGS) $(BUILD_FLAGS) $(TRACE_FLAGS) -c $(DIR_SRC_SYSTEM)memory.cpp -o $(DIR_BUILD)system_memory.o

system_processor.o: $(DIR_SRC_SYSTEM)processor.cpp $(DIR_INC_SYSTEM)processor.h
	$(CXX) $(FLAGS) $(BUILD_FLAGS) $(TRACE_FLAGS) -c $(DIR_SRC_SYSTEM)processor.cpp -o $(DIR_BUILD)system_processor.o

system_video.o: $(DIR_SRC_SYSTEM)video.cpp $(DIR_INC_SYSTEM)video.h
	$(CXX) $(FLAGS) $(BUILD_FLAGS) $(TRACE_FLAGS) -c $(DIR_SRC_SYSTEM)video.cpp -o $(DIR_BUILD)system_video.o

### TYPE ###

build_type: type_buffer.o

type_buffer.o: $(DIR_SRC_TYPE)buffer.cpp $(DIR_INC_TYPE)buffer.h
	$(CXX) $(FLAGS) $(BUILD_FLAGS) $(TRACE_FLAGS) -c $(DIR_SRC_TYPE)buffer.cpp -o $(DIR_BUILD)type_buffer.o
