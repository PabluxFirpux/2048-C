CFLAGS = -Wall -pedantic -g -lncursesw

LIBDIR = libs/
SRC = main.o

TARGET = 2048

all: $(TARGET)
	./$(TARGET)

$(TARGET): $(SRC)
	gcc $(SRC) -o $(TARGET) $(CFLAGS)

%.o : %.c
	gcc -c $? -o $@ $(CFLAGS)

clean :
	@echo "Removing trash"
	@rm -rf *.o || rm -rf *.out || echo "Already clean :)"
