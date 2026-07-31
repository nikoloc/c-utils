CFLAGS=-g -fsanitize=address,undefined

all: test

build/array: array_test.c array.h build
	$(CC) $< $(CFLAGS) -o $@

# build/list: list_test.c list.h build
# 	$(CC) $< $(CFLAGS) -o $@

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

build/arguments_parser: arguments_parser_test.c arguments_parser.h build
	$(CC) $< $(CFLAGS) -o $@

build/shell_parser: shell_parser_test.c shell_parser.h build
	$(CC) $< $(CFLAGS) -o $@

build/parse: parse_test.c parse.h build
	$(CC) $< $(CFLAGS) -o $@

test_array: build/array
	$<

# test_list: build/list
# 	$<

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

test_arguments_parser: build/arguments_parser
	$<

test_shell_parser: build/shell_parser
	$<

test_parse: build/parse
	$<

test: test_array test_string test_reader test_memory test_logger test_hashmap test_arguments_parser test_shell_parser test_parse

build:
	mkdir build

clean:
	rm -rf build 2>/dev/null
