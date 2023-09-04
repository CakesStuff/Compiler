HEADERS_C=$(shell find . -type f -name "*.h")
SOURCES_C=$(shell find . -type f -name "*.c")
SOURCES_ASM=$(shell find . -type f -name "*.asm")
OBJECTS_ASM=$(patsubst %.asm, build/%.obj, $(SOURCES_ASM))

.PHONY: all compiler run debug test stdlib

all: always compiler stdlib

always:
	@mkdir -p build
	@mkdir -p build/stdlib

compiler: build/compiler $(HEADERS_C)

build/compiler: $(SOURCES_C)
	@clang -o build/compiler $^

stdlib: $(OBJECTS_ASM)

build/%.obj: %.asm
	@nasm -felf64 -o $@ $<

clean:
	@rm -rf build

run: all
	@build/compiler source.dl out
	@rm out.asm
	@rm out.o

debug: all
	@valgrind build/compiler source.dl out
	@rm out.o
	
test: all
	@build/compiler source.dl /dev/null