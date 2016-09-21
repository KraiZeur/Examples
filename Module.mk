BUILD_DIR=build
CC=clang++
GDB=gdb
CFLAGS=-Wall -g

DIR_GUARD=@mkdir -p $(BUILD_DIR)

CUR_FILES_CPP=$(shell find -name "*.cpp" -type f -printf '%P\n')
CUR_FILES_O=$(patsubst %.cpp,%.o, $(CUR_FILES_CPP))
CUR_BINARIES=$(patsubst %.o,$(BUILD_DIR)/%, $(CUR_FILES_O))

### Make Rules :
### make
### make run V=executable
### make debug V=executable

.PHONY:all
all:$(CUR_BINARIES)

$(BUILD_DIR)/%:$(BUILD_DIR)/%.o
	$(DIR_GUARD)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o:%.cpp
	$(DIR_GUARD)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:run
run:$(BUILD_DIR)/$(V)
	./$<

.PHONY:debug
debug:$(BUILD_DIR)/$(V)
	gdb -tui -q ./$<

clean:
	rm -rf ./$(BUILD_DIR)
