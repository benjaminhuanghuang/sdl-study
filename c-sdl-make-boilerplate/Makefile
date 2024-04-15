C        = gcc
FLAGS    = -std=c99 -Wall -g
INCLUDES = -IC:\SDL2\include
LIBS   	 = -LC:\SDL2\lib -lmingw32 -lSDL2main -lSDL2
SRC      = sdl-demo.c
OBJS	 = $(SRC:.c=.o)
TARGET   = demo.exe


$(TARGET): $(OBJS)
	$(C) $(FLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LIBS) 

.c.o: 
	$(C) $(FLAGS) $(INCLUDES) -c $< -o $@

clean:
	del *.o *.exe *~
