
SRCDIR := src
SOURCES := main.c bmp2map.c map.c sac.c liste.c algorithmes.c
OBJECTS := $(SOURCES:%.c=%.o)
PROGRAM := main

$(PROGRAM): $(OBJECTS)
	gcc -Wall -o $@ $(OBJECTS) -lm

%.o: $(SRCDIR)/%.c
	gcc -Wall -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(PROGRAM)
