HEADERS_C=$(shell find . -type f -name "*.h")
SOURCES_C=$(shell find . -type f -name "*.c")

.PHONY: all compiler run debug test

all: always compiler

always:
	@mkdir -p build

compiler: build/compiler $(HEADERS_C)

build/compiler: $(SOURCES_C)
	@clang -o build/compiler $^

clean:
	@rm -rf build

run: always compiler
	@build/compiler source.dl out
	@rm out.asm
	@rm out.o

debug: always compiler
	@valgrind build/compiler source.dl out
	@rm out.asm
	@rm out.o
	
test: always compiler
	@build/compiler source.dl /dev/null