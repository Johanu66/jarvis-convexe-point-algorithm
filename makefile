LIB_PATHS= -L/usr/X11R6/lib -L/usr/lib
HDR_PATHS= -I/usr/X11R6/include
LIBS= -lglut -lGLU -lGL -lm -L/usr/X11/lib
#-lXext -lXmu -lXi -lX11 -g-std=c++98

CFLAGS=$(HDR_PATHS) -Wno-deprecated
CC=g++
#CC=gcc

ok : OpenGL 

.cc.o:
	$(CC) -c -O3 $(CFLAGS) $<

graham.o: graham.cc graham.h
	$(CC) -c -O3 $(CFLAGS) graham.cc -o graham.o

OBJECTS_main = OpenGL.o  
OBJECTS_graham = graham.o  

OpenGL:  $(OBJECTS_main) $(OBJECTS_graham)
	$(CC) $(OBJECTS_main) $(OBJECTS_graham) -o OpenGL $(LIB_PATHS) $(LIBS)

indenter :
	for i in *.h *.cc; do sed -i~ -f sed.sed $$i; done
	indent  *.h *.cc

clean :
	rm -fr *.o
	touch *.cc #*.h

delete :
	rm -f *.h~
	rm -f *.cc~