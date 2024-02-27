CXX = c++
CXXSTDLANG = -std=c++11
CXXFLAGS = -Wall -Werror -Wextra -O3
INCLUDES = -I ../include

custom_time.cpp.o: custom_time.cpp
	$(CXX) $(CXXSTDLANG) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<
clean:
	rm -f *.o
