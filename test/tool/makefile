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

DIR_BIN_LIB=../../bin/lib/
DIR_BUILD=../../build/test/
DIR_TOOL=./
EXE=c65-test
EXE_FLAGS=-pthread
FLAGS=-march=native -std=c++17 -Wall -Werror
LIB=libc65.a
LIB_FLAGS=-lreadline -lSDL2 -lSDL2main
LIB_TEST=libc65-test.a

all: launcher

### LAUNCHER ###

launcher:
	@echo ''
	@echo '--- BUILDING LAUNCHER ----------------------'
	$(CXX) $(FLAGS) $(BUILD_FLAGS) $(TRACE_FLAGS) $(DIR_TOOL)launcher.cpp $(DIR_BIN_LIB)$(LIB) $(DIR_BUILD)$(LIB_TEST) -o $(DIR_BUILD)$(EXE) \
		$(EXE_FLAGS) $(LIB_FLAGS)
	@echo '--- DONE -----------------------------------'
