.PHONY: compile clean 

DIR_SRC += src
OBJ_DIR += obj
INC += -Iinclude -Isrc
CFLAGS += -std=gnu99 -Wall -fPIC -DRGL_BUILD
LIBS += -lepoxy -lm -lopenal
SRC += $(wildcard $(addsuffix /*.c, $(DIR_SRC)))
OBJ += $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC))

compile: $(OBJ) $(OUT)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OUT): $(OBJ)
	@mkdir -p $(@D)
	$(CC) -shared $(CFLAGS) $(LIBS) $(OBJ) -o $@

clean:
	rm -r $(OBJ) $(OUT) 
