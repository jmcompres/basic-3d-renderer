# Definición de variables
CXX = g++
CXXFLAGS = -Wall
LDFLAGS = -lmingw32 -lSDL2main -lSDL2
TARGET = main
SRC = main.cpp

# Regla por defecto
all: $(TARGET)

# Regla para compilar el ejecutable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

# Regla para limpiar los archivos generados
clean:
	rm -f $(TARGET)

.PHONY: all clean
