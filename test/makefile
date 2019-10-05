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

BUILD_FLAGS_DBG=BUILD_FLAGS=-g
BUILD_FLAGS_REL=BUILD_FLAGS=-O3\ -DNDEBUG
DIR_BUILD=../build/test/
DIR_ROOT=./
DIR_SRC=./src/
DIR_TOOL=./tool/
JOB_SLOTS=4
TEST=./c65-test -q
TRACE?=0
TRACE_FLAGS_DBG=TRACE_FLAGS=-DTRACE_COLOR\ -DTRACE=
TRACE_FLAGS_REL=TRACE_FLAGS=-DTRACE_COLOR\ -DTRACE=

all: debug

debug: begin_debug clean init lib_debug exe_debug

release: begin_release clean init lib_release exe_release

### SETUP ###

begin_debug:
	@echo ''
	@echo '============================================'
	@echo 'BUILDING TEST SUITE (DEBUG)'
	@echo '============================================'

begin_release:
	@echo ''
	@echo '============================================'
	@echo 'BUILDING TEST SUITE'
	@echo '============================================'

clean:
	rm -rf $(DIR_BUILD)

init:
	mkdir -p $(DIR_BUILD)

### LIBRARY ###

lib_debug:
	@echo ''
	@echo '============================================'
	@echo 'BUILDING TEST SUITE LIBRARIES (DEBUG)'
	@echo '============================================'
	cd $(DIR_SRC) && make $(BUILD_FLAGS_DBG) $(TRACE_FLAGS_DBG)$(TRACE) build -j$(JOB_SLOTS)
	cd $(DIR_SRC) && make archive

lib_release:
	@echo ''
	@echo '============================================'
	@echo 'BUILDING TEST SUITE LIBRARIES'
	@echo '============================================'
	cd $(DIR_SRC) && make $(BUILD_FLAGS_REL) $(TRACE_FLAGS_REL)$(TRACE) build -j$(JOB_SLOTS)
	cd $(DIR_SRC) && make archive

### EXECUTABLE ###

exe_debug:
	@echo ''
	@echo '============================================'
	@echo 'BUILDING TEST SUITE EXECUTABLES (DEBUG)'
	@echo '============================================'
	cd $(DIR_TOOL) && make $(BUILD_FLAGS_DBG) $(TRACE_FLAGS_DBG)$(TRACE)

exe_release:
	@echo ''
	@echo '============================================'
	@echo 'BUILDING TEST SUITE EXECUTABLES'
	@echo '============================================'
	cd $(DIR_TOOL) && make $(BUILD_FLAGS_REL) $(TRACE_FLAGS_REL)$(TRACE)

### TESTS ###

run_test:
	@echo ''
	@echo '============================================'
	@echo 'RUNNING TEST SUITE'
	@echo '============================================'
	@echo ''
	@echo '--- RUNNING TESTS --------------------------'
	@cd $(DIR_BUILD) && if $(TEST); \
	then \
		echo '--- PASSED ---------------------------------'; \
	else \
		echo '--- FAILED ---------------------------------'; \
		exit 1; \
	fi

### MISC ###

lines:
	@echo ''
	@echo '============================================'
	@echo 'CALCULATING LINE COUNT'
	@echo '============================================'
	cloc $(DIR_ROOT)

static:
	@echo ''
	@echo '============================================'
	@echo 'RUNNING STATIC ANALYSIS'
	@echo '============================================'
	cppcheck --enable=all --std=c++17 $(DIR_SRC) $(DIR_TOOL)
