rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC_DIR := src
OBJ_DIR := build/obj
SRC_FILES := $(call rwildcard,$(SRC_DIR),*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

build/mbd-json.lib: $(OBJ_FILES)
	mkdir -p build/lib
	ar rvs build/lib/libmbd-json.a $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	gcc $(CPPFLAGS) -c -Iinclude -o $@ $<

clean:
	rm -r -f build
