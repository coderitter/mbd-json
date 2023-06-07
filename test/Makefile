CC = gcc
CFLAGS = -Os -std=c99 -Wall -pedantic -I../include/

src = $(wildcard *.c)
src += $(wildcard ../src/*.c)
obj = $(src:.c=.o)
dep = $(obj:.o=.d)

.PHONY: build all clean

build: test.a

all: clean build

clean::
	rm -rf $(dep)
	rm -rf $(obj)
	rm -rf *.a

test: test.a
	./test.a	

test.a: $(obj)
	gcc $(CFLAGS) -o $@ $^	

-include $(dep)

%.d: %.c
	$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@