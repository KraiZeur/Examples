# manual : https://www.gnu.org/software/make/manual/html_node/

# default target if no target  specified
all:target

Q:=@

# ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# RULE/RECIPES

# if target is the user's target this rule will be hit
target: prerequisites
	# recipe
	$(Q)echo "target rule"
.PHONY: target 
# PHONY tell make that the recipe is not a file There are two reasons to use a phony target: to avoid a conflict with a file of the same name, and to improve performance. 

# generic rule if none of the aboves rules are hit
%: prerequisites
	# recipe
	$(Q)echo "generic rule"

prerequisites:
	$(Q)echo "prerequisites rule"
.PHONY: prerequisites

# targets : normal-prerequisites | order-only-prerequisites

#immediate : immediate ; deferred
#        deferred


# Static pattern rule

VAR_OBJS = foo.o bar.o

$(VAR_OBJS): %.o : %.c
	@echo "stem value is : $*"

# $* is a variable containing the target stem i.e value contained isnde the %
# ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# VARIABLES

input_value = step1

deffered = $(input_value)   # Makefile will expand the deffered value
immediate := $(input_value) # C assignation style

input_value = step2

variables:
	$(Q)echo "deffered = $(deffered) immediate = $(immediate)"



# ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# Real case example
# try it using make app -f Makefile
BASE_DIR:=$(abspath ./)

CC:=gcc

C_FLAGS+=-I $(BASE_DIR)/include

SRC_DIR := src
BUILD_DIR := build

BIN_NAME := app

C_FILES := $(shell find ./ -name "*.c" -type f -printf '%P\n')
O_FILES := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_FILES))

debug:
	@echo "$(C_FILES) "

$(BUILD_DIR):
	@echo "debug recipe"
	$(Q)mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR) # $(BUILD_DIR) is an | order-only-prerequisites
	$(Q)echo "Compiling $<"
	$(Q)$(CC) $(C_FLAGS) -c $< -o $@

$(BIN_NAME): $(O_FILES)
	$(Q)echo "Linking $^ into $@"
	$(Q)$(CC) $(C_FLAGS) $^ -o $@

run: $(BIN_NAME)
	./$<
.PHONY: run

clean: rm-objs
.PHONY: clean

rm-binary:
	$(Q)rm -rf $(BIN_NAME)
.PHONY: rm-binary

rm-objs:
	$(Q)rm -rf $(BUILD_DIR)
.PHONY: rm-objs