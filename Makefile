a: main.cpp
	g++ -std=c++17 main.cpp -framework GLUT -framework OpenGL -L/usr/local/lib -lglfw -lglew

clean:
	rm -rf a.out a.exe

all: a
