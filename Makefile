build:
	g++ main.cpp food.cpp snake.cpp game.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o snake -Wall

run:
	./snake