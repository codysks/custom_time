CXX = c++
CXXSTDLANG = -std=c++11
CXXFLAGS = -Wall -Werror -Wextra -O3

custom_time.cpp.o: custom_time.cpp
	$(CXX) $(CXXSTDLANG) $(CXXFLAGS) -c -o $@ $<
