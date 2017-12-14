CXX=       	g++
CXXFLAGS= 	-g -gdwarf-2 -std=gnu++11 -Wall -Iinclude
LDFLAGS=	-lpthread
HEADERS=	$(wildcard include/htcache/*.h)
APPS=		$(patsubst src/%,bin/%,$(patsubst %.cpp,%,$(wildcard src/*.cpp)))
TESTS=		$(patsubst tests/%,bin/%,$(patsubst %.cpp,%,$(wildcard tests/*.cpp)))

all:    $(APPS) $(TESTS)

bin/%:	src/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

bin/test_%:	tests/test_%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

tests: tests/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@

clean:
	rm -f $(APPS) $(TESTS)

.PHONY: all clean
