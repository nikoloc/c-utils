CFLAGS=-g -fsanitize=address,undefined

all: test_array test_list test_string

build/array: array_test.c array.h build
	$(CC) $< $(CFLAGS) -o $@

build/list: list_test.c list.h build
	$(CC) $< $(CFLAGS) -o $@

build/string: dynamic_string_test.c dynamic_string.h build
	$(CC) $< $(CFLAGS) -o $@

build/reader: reader_test.c reader.h build
	$(CC) $< $(CFLAGS) -o $@

test_array: build/array
	$<

test_list: build/list
	$<

test_string: build/string
	$<

test_reader: build/reader
	$<

test: test_array test_list test_string test_reader

build:
	mkdir build

clean:
	rm -rf build 2>/dev/null

.PHONY: clean test_array test_list test_string
