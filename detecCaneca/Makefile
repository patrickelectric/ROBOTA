OPENCV=	`pkg-config --libs --cflags opencv`
all:
	
	gcc src/maindebug.cpp -o debugador $(OPENCV) -lpthread -Wall
	gcc src/main.cpp -o detector $(OPENCV) -lpthread  -Wall 
	gcc src/obstaculoDetec.cpp -o obstaculoDetector $(OPENCV) -lpthread  -Wall 
	g++ src/controleRobota.cpp -o controleRobota -lpthread  -Wall -lrt -lstdc++
