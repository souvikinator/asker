CC=g++
FLAGS=-std=c++11
TARGET=asker

demo:
	$(CC) $(FLAG) -o $(TARGET) example/all_demo.cpp

clean:
	rm $(TARGET)
