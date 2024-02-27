CXX = c++
CXXSTDLANG = -std=c++11
CXXFLAGS = -Wall -Werror -Wextra -O3
INCLUDES = -I ../include
LIBPATH = -L ..

TARGETS = test
.PHONY: all
all: $(TARGETS)

test.cpp.o: test.cpp
	$(CXX) $(CXXSTDLANG) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<
test: test.cpp.o
	$(CXX) $(CXXSTDLANG) $(CXXFLAGS) $(INCLUDES) $(LIBPATH) -o $@ $< -lcustom_time
.PHONY: clean
clean:
	rm -f *.o
