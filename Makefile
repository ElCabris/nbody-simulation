TARGET = nbodies

CXX = mpic++
CXXFLAGS = -Wall -Iinclude

SRCDIR = src
BUILDDIR = build
BINDIR = bin

LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Lista de archivos fuente
SRCS := $(wildcard $(SRCDIR)/**/*.cpp) $(wildcard $(SRCDIR)/*.cpp)

# Lista de archivos objeto correspondientes a los archivos fuente
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))

# Regla de compilación de archivos objeto
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla de enlazado del ejecutable
$(BINDIR)/$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $^ -o $@ $(LIBS)

.PHONY: clean

clean:
	@rm -rf $(BUILDDIR) $(BINDIR)


