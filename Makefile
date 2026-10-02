CC := gcc

OUT  := lang_parser
SRC  := $(OUT).c
OBJS := $(OUT).o

.PHONY: default clean

default: $(OUT)

%.o: %.c
	$(CC) -c $< -o $@

$(OUT): $(OBJS)
	$(CC) $(OBJS) -o $@

clean:
	-rm -f $(OBJS)
	-rm -f $(OUT)
