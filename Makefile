CFLAGS=-g -fsanitize=address,undefined

all: test_array test_list test_string

build/array: array_test.c array.h build
	$(CC) $< $(CFLAGS) -o $@

build/list: list_test.c list.h build
	$(CC) $< $(CFLAGS) -o $@

build/string: dstring_test.c dstring.h build
	$(CC) $< $(CFLAGS) -o $@

build/reader: reader_test.c reader.h build
	$(CC) $< $(CFLAGS) -o $@

build/memory: memory_test.c memory.h build
	$(CC) $< $(CFLAGS) -o $@

build/logger: logger_test.c logger.h build
	$(CC) $< $(CFLAGS) -o $@

build/hashmap: hashmap_test.c hashmap.h build
	$(CC) $< $(CFLAGS) -o $@

test_array: build/array
	$<

test_list: build/list
	$<

test_string: build/string
	$<

test_reader: build/reader
	$<

test_memory: build/memory
	$<

test_logger: build/logger
	$<

test_hashmap: build/hashmap
	$<

test: test_array test_list test_string test_reader test_memory test_logger

build:
	mkdir build

clean:
	rm -rf build 2>/dev/null
