all: main

main: main.cpp AirportAnimator.cpp
	g++ -g main.cpp AirportAnimator.cpp -lpthread -Wall -ansi -pedantic -std=c++20 -fpermissive -lncurses -o project2

clean:
	rm -f project2