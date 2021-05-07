# Conan stuff
include conanbuildinfo.mak

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
	$(CXX) $(LDFLAGS) $(LIBS) -o $@ $^

.build/%.o: %.cpp | .build/
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $^

.build/:
	mkdir -p $@

# Conan automatic dependency installation
conanbuildinfo.mak: conanfile.txt
	conan install . --build=missing
