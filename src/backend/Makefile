# Conan stuff
include .build/conanbuildinfo.mak

CPPFLAGS += $(addprefix -D, $(CONAN_DEFINES))
CPPFLAGS += $(addprefix -I, $(CONAN_INCLUDE_DIRS))
LDFLAGS += $(addprefix -L, $(CONAN_LIB_DIRS))
LDLIBS += $(addprefix -l, $(CONAN_LIBS))
LDLIBS += $(addprefix -l, $(CONAN_SYSTEM_LIBS))
CFLAGS += $(CONAN_CFLAGS)
CXXFLAGS += $(CONAN_CXXFLAGS)

# C++ stuff
CPPFLAGS += -std=c++17
CFLAGS += -Wall

# Recipe-related Variables
MAINS = server.cpp get-shortest-path.cpp
SOURCES = $(filter-out $(MAINS), $(wildcard *.cpp))
HEADERS = $(wildcard *.hpp)
OBJECTS = $(patsubst %.cpp, .build/%.o, $(SOURCES))

# Compilation mode
OPTIMIZE ?= yes
ifeq ($(OPTIMIZE),yes)
    CXXFLAGS += -O3 -march=native
    LDFLAGS  += -O3 -march=native
else ifeq ($(OPTIMIZE),no)
else
    $(error "bad value for OPTIMIZE: $(OPTIMIZE); must be 'yes' or 'no' (default='yes').")
endif


# Phonies
.PHONY: all
all: waze-server get-shortest-path

.PHONY: serve
serve:
	./waze-server

.PHONY: test
test: test-dijkstra test-astar-euclidean test-astar-manhattan

.PHONY: clean
clean:
	rm -f waze-server
	rm -rf .build

# C++ Compilation Targets
waze-server: .build/server.o $(OBJECTS)
	$(CXX) -g -Wall -o $@ $^ $(LDFLAGS) $(LDLIBS)

get-shortest-path: .build/get-shortest-path.o $(OBJECTS)
	$(CXX) -g -Wall -o $@ $^ $(LDFLAGS) $(LDLIBS)

.build/%.o: %.cpp $(HEADERS) | .build/
	$(CXX) -g $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

.build/:
	mkdir -p $@

# Conan automatic dependency installation
.build/conanbuildinfo.mak: conanfile.txt | .build/
	cp $< .build/
	cd .build && conan install . --build=missing

# Tests
.PHONY: test-dijkstra
test-dijkstra: ../../test-graphs get-shortest-path test-shortest-path.sh
	./test-shortest-path.sh $< dijkstra

.PHONY: test-astar-euclidean
test-astar-euclidean: ../../test-graphs get-shortest-path test-shortest-path.sh
	./test-shortest-path.sh $< astar-euclidean

.PHONY: test-astar-manhattan
test-astar-manhattan: ../../test-graphs get-shortest-path test-shortest-path.sh
	./test-shortest-path.sh $< astar-manhattan
