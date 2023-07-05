rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC_DIR := src
OBJ_DIR := build/obj
SRC_FILES := $(call rwildcard,$(SRC_DIR),*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

build/mbd_json.lib: $(OBJ_FILES)
	mkdir -p build/lib
	ar rvs build/lib/libmbd_json.a $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	gcc $(CPPFLAGS) -c -Iinclude -o $@ $<

libriscv32_mbd_json.a:
	mkdir -p build
	riscv32-unknown-elf-gcc -std=c99 -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -g -c src/mbd_json.c \
	-o build/riscv32-mbd_json.o  \
	-g -Wall \
	-Iinclude
	riscv32-unknown-elf-ar rcs build/libriscv32-mbd_json.a build/riscv32-mbd_json.o

libmbd_json.a:
	mkdir -p build
	gcc -g -c src/mbd_json.c \
	-o build/mbd_json.o  \
	-g -Wall \
	-Iinclude
	ar rcs build/libmbd_json.a build/mbd_json.o	

tests:
	mkdir -p build
	gcc test/mbd_json.test.c \
	-o build/tests \
	src/mbd_json.c \
	-g -Wall \
	-Iinclude	

clean:
	rm -r -f build
