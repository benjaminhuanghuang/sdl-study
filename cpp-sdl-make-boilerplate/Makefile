C        = gcc
FLAGS    = -std=c99 -Wall -g
INCLUDES = -ISDL2/include
LIBS   	 = -LSDL2/lib -lmingw32 -lSDL2main -lSDL2
SRC      = sdl-demo.c
OBJS	   = $(SRC:.c=.o)
TARGET   = sdl-demo.exe


$(TARGET): $(OBJS)
	$(C) $(FLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LIBS) 

.c.o: 
	$(C) $(FLAGS) $(INCLUDES) -c $< -o $@

clean:
	del *.o *.exe *~
