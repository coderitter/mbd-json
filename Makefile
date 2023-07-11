CC := gcc
CFLAGS := -g -Wall -Wextra

SRC := $(wildcard src/*.c)
INCLUDE := $(wildcard include/*.h)
TEST := test/mbd_json.test.c
OBJ := $(SRC:src/%.c=build/obj/%.o)

build/test: $(SRC) $(INCLUDE) $(TEST) build/libmbd-json.a
	mkdir -p build
	$(CC) -std=gnu99 $(TEST) -o build/test \
	$(CFLAGS) \
	-Iinclude \
	libs/Unity/src/unity.c \
	-Ilibs/Unity/src \
	-lmbd-json -Lbuild \

build/libmbd-json.a: $(OBJ)
	mkdir -p build
	ar rcs $@ $(OBJ)

build/obj/%.o: src/%.c
	mkdir -p build/obj
	$(CC) -c $< -o $@ $(CFLAGS) -Iinclude

.PHONY: clean
clean:
	rm -r -f build

.PHONY: clean_libs
clean_libs:
	$(MAKE) -C libs/mbd-mqtt-packer  clean