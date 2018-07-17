
PLATFORM := $(shell uname)

GL_LIBS = `pkg-config --static --libs glfw3 glew`
EXT = 
DEFS = `pkg-config --cflags glfw3` -std=c++11
WARNINGS = -ansi -Wall -pedantic

# Any other platform specific libraries here...
ifneq (, $(findstring CYGWIN, $(PLATFORM)))
    GL_LIBS = -lopengl32 -lglfw3 -lglew32 -lGL
	EXT = .exe
    DEFS =-DWIN32
endif

ifneq (, $(findstring Darwin, $(PLATFORM)))
    GL_LIBS += -framework OpenGL
    EXT =
endif

LINK += shader.o Application.o Player.o World.o Shaders.o Ground.o Skybox.o Model.o Tree.o Car.o

.PHONY:  clean

all : run$(EXT)

run$(EXT) : main.o $(LINK)
	g++ $(DEFS) -Wall -pedantic -o run main.o $(LINK) $(GL_LIBS)

main.o : main.cpp $(LINK)
	g++ $(WARNINGS) $(DEFS) -c main.cpp

shader.o : shader.cpp shader.hpp
	g++ $(DEFS) -c shader.cpp

Application.o : Application.cpp Application.h
	g++ $(WARNINGS) $(DEFS) -c Application.cpp

Player.o : Player.cpp Player.h
	g++ $(WARNINGS) $(DEFS) -c Player.cpp

World.o : World.cpp World.h
	g++ $(WARNINGS) $(DEFS) -c World.cpp

Shaders.o : Shaders.cpp Shaders.h
	g++ $(WARNINGS) $(DEFS) -c Shaders.cpp

Ground.o : Ground.cpp Ground.h
	g++ $(WARNINGS) $(DEFS) -c Ground.cpp

Skybox.o : Skybox.cpp Skybox.h
	g++ $(WARNINGS) $(DEFS) -c Skybox.cpp

Model.o : Model.cpp Model.h
	g++ $(WARNINGS) $(DEFS) -c Model.cpp

Tree.o : Tree.cpp Tree.h
	g++ $(WARNINGS) $(DEFS) -c Tree.cpp

Car.o : Car.cpp Car.h
	g++ $(WARNINGS) $(DEFS) -c Car.cpp

clean:
	rm -f *.o run $(EXT)

