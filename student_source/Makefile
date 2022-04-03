SRC = route_lookup.c io.c io.h utils.c utils.h tables_engine.c tables_engine.h
CFLAGS = -Wall -O3

all: route_lookup

route_lookup: $(SRC)
	gcc $(CFLAGS) $(SRC) -o route_lookup -lm

.PHONY: clean

clean:
	rm -f route_lookup

# RL Lab 2022 Switching UC3M
# Álvaro Merino Argumánez
# Lucía Mayoral
