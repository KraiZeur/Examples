BUILD_DIR=build
CC=g++
GDB=gdb
CFLAGS+=-Wall -g

ifeq ($(shell clang++ -v 2>/dev/null && echo 1 || echo 0),1)
CC=clang++
endif

DIR_GUARD=@mkdir -p $(BUILD_DIR)

CUR_FILES_CPP=$(shell find -name "*.cpp" -type f -printf '%P\n')
CUR_FILES_C=$(shell find -name "*.c" -type f -printf '%P\n')

CUR_FILES_O=$(patsubst %.cpp,%.o, $(CUR_FILES_CPP))
CUR_FILES_O+=$(patsubst %.c,%.o, $(CUR_FILES_C))

CUR_BINARIES=$(patsubst %.o,$(BUILD_DIR)/%, $(CUR_FILES_O))

### Make Rules :
### make
### make run V=executable
### make debug V=executable
### make debugmem V=executable
### make dump V=executable
### make clean

.PHONY:all
all:$(CUR_BINARIES)

$(BUILD_DIR)/%:$(BUILD_DIR)/%.o
	$(CC) $(CFLAGS) $< -o $@ $(LINKED_LIB)

$(BUILD_DIR)/%.o:%.cpp
	$(DIR_GUARD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o:%.c
	$(DIR_GUARD)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:run
run:$(BUILD_DIR)/$(V)
	./$<

.PHONY:debug
debug:$(BUILD_DIR)/$(V)
	@$(GDB) -tui -q ./$<

.PHONY:debugmem
debugmem:$(BUILD_DIR)/$(V)
	@valgrind --leak-check=full --show-leak-kinds=all -v ./$<

.PHONY:dump
dump:$(BUILD_DIR)/$(V)
	@objdump -S ./$< | less

.PHONY:clean
clean:
	rm -rf ./$(BUILD_DIR)
