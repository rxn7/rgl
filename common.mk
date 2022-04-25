DIR_SRC += src
INC += -Iinclude -Isrc
CFLAGS += -std=gnu99 -Wall -fPIC -DRGL_BUILD
LIBS += -lepoxy -lm -lopenal
SRC += $(wildcard $(addsuffix /*.c, $(DIR_SRC)))
OBJ += $(patsubst %.c, %.o, $(SRC))

.PHONY:all

all:$(OBJ) $(OUT)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OUT): $(OBJ)
	$(CC) -shared $(CFLAGS) $(LIBS) $(OBJ) -o $@

clean:
	rm -r $(OBJ) $(OUT) 
