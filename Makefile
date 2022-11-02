FSDB = fsdb 

SRC = $(wildcard ./*.c)

OBJ = $(SRC:.c=.o)

CC = cc 

RM = rm 

CFLAGS = -g
LIBS = -lext2fs -lm 
LDFLAGS = 

.PHONY: clean 

all: $(FSDB)

debug: CFLAGS += -ggdb -O0
debug: $(FSDB)

$(FSDB): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS) $(LIBS)

.c.o:
	$(CC) -c $< $(CFLAGS)

clean:
	$(RM) ./$(OBJ) ./$(FSDB)







