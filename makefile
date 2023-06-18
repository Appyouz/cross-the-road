make:
	g++ -std=c++17 ./src/*.cpp -I./include -o ./bin/main -lSDL2 -lSDL2_ttf -ldl


clean:
	rm -rf *.o
