CC = gcc
CFLAGS = -Wall -g
CPPFLAGS = -MMD -MP
LDLIBS = -lsqlite3 -lncurses

SRC_DIR = src
OBJ_DIR = obj
DATA_DIR = data
BIN_DIR = .

EXE = $(BIN_DIR)/budgurse
SRC = $(wildcard $(SRC_DIR)/*.c) 
HDR = $(wildcard $(SRC_DIR)/*.h)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TEST_DIR = test

TEST_EXE = $(TEST_DIR)/test
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)
TEST_HDR = $(TEST_DIR)/test.h
TEST_DB = $(DATA_DIR)/test.db
TEST_FILTER = $(OBJ_DIR)/budgurse.o

# target : prerequisite
# make with no target will compile budgurse since first rule
all: $(EXE)

# link object files and libraries
# | requires that the dir exists but does not check for changes
$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $^ $(LDLIBS) -o $@

# compile object files from source
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


test: $(TEST_EXE)
	rm -f $(TEST_DB)
	valgrind ./$(TEST_EXE)

# filter out budgurse.o when compiling test to run test.c main instead
$(TEST_EXE): $(TEST_SRC) $(filter-out $(TEST_FILTER),$(OBJ))
	$(CC) $(CFLAGS) $^ -I$(SRC_DIR) $(LDLIBS) -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

count_lines:
	wc -l $(SRC) $(HDR) $(TEST_SRC) $(TEST_HDR) Makefile

clean:
	rm -rf $(EXE) $(TEST_EXE) $(OBJ_DIR) $(TEST_DB)

-include $(OBJ:.o=.d)

