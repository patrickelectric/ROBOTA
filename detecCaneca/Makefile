OPENCV=	`pkg-config --libs --cflags opencv`
all:
	
	g++ src/maindebug.cpp -o debugador $(OPENCV) -lpthread
	g++ src/main.cpp -o detector $(OPENCV) -lpthread 
#	./detector
