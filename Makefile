CXX = c++
CPPFLAGS ?=
CXXFLAGS ?= -std=c++11 -O2 -Wall -Wextra -Wpedantic
LDFLAGS ?=
LDLIBS ?=

.PHONY: all clean

all: username

username: username.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $< $(LDLIBS) -o $@

clean:
	$(RM) username
