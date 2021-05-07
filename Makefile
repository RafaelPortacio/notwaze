OUT := waze-server

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst src/%.cpp, build/%.o, $(SOURCES))


.PHONY: all
all: $(OUT)

.PHONY: serve
serve:
	./$(OUT)

.PHONY: clean
clean:
	rm -f $(OUT)
	rm -rf build

$(OUT): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(LIBS) -o $@ $^

build/%.o: src/%.cpp | build/
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $^

build/:
	mkdir -p $@
