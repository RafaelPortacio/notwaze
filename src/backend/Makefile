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
OUT := waze-server

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.hpp)
OBJECTS = $(patsubst %.cpp, .build/%.o, $(SOURCES))


# Phonies
.PHONY: all
all: $(OUT)

.PHONY: serve
serve:
	./$(OUT)

.PHONY: clean
clean:
	rm -f $(OUT)
	rm -rf .build

# C++ Compilation Targets
$(OUT): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(LDLIBS) -Wall -o $@ $^

.build/%.o: %.cpp $(HEADERS) | .build/
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

.build/:
	mkdir -p $@

# Conan automatic dependency installation
.build/conanbuildinfo.mak: conanfile.txt | .build/
	cp $< .build/
	cd .build && conan install . --build=missing
