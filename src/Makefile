# Target system.
# Leave blank for standard Linux builds.
# Put DINGOO for Dingoo A320 builds.
TARGET =

# Directories and files
DATADIR = /usr/shared/openxcom/
OBJDIR = ../obj/
BINDIR = ../bin/
SRCS = $(wildcard *.cpp */*.cpp)
OBJS = $(patsubst %.cpp, $(OBJDIR)%.o, $(notdir $(SRCS)))

# Target-specific settings
ifeq ($(TARGET),DINGOO)
CXX = mipsel-linux-g++
PKG-CONFIG = /opt/mipsel-linux-uclibc/usr/bin/pkg-config
BIN = openxcom.dge
else
PKG-CONFIG = pkg-config
BIN = openxcom
endif

# Compiler settings
CXXFLAGS ?= -Wall -O2
CXXFLAGS += $(addprefix -D,$(TARGET))
CXXFLAGS += -DDATADIR=\"$(DATADIR)\"
CXXFLAGS += $(shell $(PKG-CONFIG) --cflags sdl yaml-cpp)

LIBS = $(shell $(PKG-CONFIG) --libs sdl yaml-cpp) -lSDL_gfx -lSDL_mixer

# Rules
all: $(BINDIR)$(BIN)

$(BINDIR)$(BIN): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) $(LIBS) -o $(BINDIR)$(BIN)

$(OBJDIR)%.o:: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)%.o:: Basescape/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
$(OBJDIR)%.o:: Battlescape/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)%.o:: Engine/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)%.o:: Geoscape/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)%.o:: Interface/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)%.o:: Menu/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
$(OBJDIR)%.o:: Resource/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
$(OBJDIR)%.o:: Ruleset/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
$(OBJDIR)%.o:: Savegame/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)%.o:: Ufopaedia/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(BINDIR)$(BIN) $(OBJDIR)*.o

.PHONY: all clean
