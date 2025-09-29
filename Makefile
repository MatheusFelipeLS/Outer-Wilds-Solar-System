# Compilador e flags
CXX = g++
<<<<<<< HEAD
CXXFLAGS = -Wall -g -Iinclude -O3
LDFLAGS = -lGL -lGLU -lglut
=======
CXXFLAGS = -Wall -g -Iinclude
LDFLAGS = -lGL -lGLU -lglut -lSOIL
>>>>>>> 236f45c ([fix]: texture)

# Pastas
SRC_DIR = src
OBJ_DIR = obj
BIN = main

# Lista todos os .cpp em src/
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Converte src/file.cpp -> obj/file.o
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Regra padrão
all: $(BIN)

# Linka os objetos e gera o binário final
$(BIN): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Compila cada .cpp em .o (dentro de obj/)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN)
