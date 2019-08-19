mac: main.cpp
	g++ -std=c++17 main.cpp -framework GLUT -framework OpenGL -L/usr/local/lib -lglfw -lglew

linux: main.cpp
	g++ -std=c++17 main.cpp -L/usr/local/lib -lglfw3 -lGLEW -lGL -lpthread -lX11 -ldl

debug-mac:
	g++ -g -std=c++17 main.cpp -framework GLUT -framework OpenGL -L/usr/local/lib -lglfw -lglew

debug-linux: main.cpp
	g++ -g -std=c++17 main.cpp -L/usr/local/lib -lglfw3 -lGLEW -lGL -lpthread -lX11 -ldl

clean:
	rm -rf a.out a.exe

all: mac
