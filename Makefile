# DIRECTORIES
SRCDIR := src
BUILDDIR := build
TARGETDIR := bin
INCLUDEDIR := include

# TARGET
TARGET := ptmonitor

CC := g++

SRC := $(wildcard $(SRCDIR)/*.cpp)
OBJ := $(SRC:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
INCLUDE := $(wildcard $(INCLUDEDIR)/*.h)

CFLAGS :=`pkg-config --cflags --libs gtk+-3.0 glib-2.0 gthread-2.0` -rdynamic -std=c++17 -g 
CFLAGS += $(addprefix -I, $(INCLUDEDIR))

LIBS = -lslope -lm -lrt 

all: directories $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGETDIR)/$@ $^ $(CFLAGS) $(LIBS)

$(OBJ): $(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(INCLUDE)
	$(CC) -o $@ -c $< $(CFLAGS)


.PHONY: clean all directories print

directories:
	@mkdir -p build
	@mkdir -p bin

clean:
	rm $(BUILDDIR)/*.o
	rm $(TARGETDIR)/$(TARGET)
	rmdir bin
	rmdir build

print:
	@echo $(OBJ)