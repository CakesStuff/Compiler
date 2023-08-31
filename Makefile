HEADERS_C=$(shell find . -type f -name "*.h")
SOURCES_C=$(shell find . -type f -name "*.c")

.PHONY: all compiler

all: always compiler

always:
	@mkdir -p build

compiler: build/compiler

build/compiler: $(SOURCES_C) $(HEADERS_C)
	@clang -o build/compiler $^

clean:
	@rm -rf build

run: always compiler
	@build/compiler source.dl o.asm
	@nasm -felf64 o.asm
	@rm o.asm
	@ld -o out o.o
	@rm o.o