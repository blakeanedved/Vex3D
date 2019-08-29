mac: main.cpp
	g++ -std=c++17 main.cpp -framework GLUT -framework OpenGL -L/usr/local/lib -lglfw -lglew -lassimp

linux: main.cpp
	g++ -std=c++17 main.cpp -L/usr/local/lib -lglfw3 -lGLEW -lGL -lpthread -lX11 -ldl

debug-mac:
	g++ -g -std=c++17 main.cpp -framework GLUT -framework OpenGL -L/usr/local/lib -lglfw -lglew -lassimp

debug-linux: main.cpp
	g++ -g -std=c++17 main.cpp -L/usr/local/lib -lglfw3 -lGLEW -lGL -lpthread -lX11 -ldl

clean:
	rm -rf a.out .DS_Store **/.DS_Store a.exe

all: mac
