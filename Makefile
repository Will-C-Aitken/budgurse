CC = gcc
CFLAGS = -Wall -g
CPPFLAGS = -MMD -MP
LDLIBS = -lsqlite3


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
TEST_SRC = $(TEST_DIR)/test.c
TEST_HDR = $(TEST_DIR)/test.h
TEST_DB = $(DATA_DIR)/test.db
TEST_FILTER = $(OBJ_DIR)/budgurse.o


all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


test: $(TEST_EXE)
	rm -f $(TEST_DB)
	valgrind ./$(TEST_EXE)

$(TEST_EXE): $(TEST_SRC) $(filter-out $(TEST_FILTER),$(OBJ))
	$(CC) $(CFLAGS) $^ -I$(SRC_DIR) $(LDLIBS) -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@


count_lines:
	wc -l $(SRC) $(HDR) $(TEST_SRC) $(TEST_HDR) Makefile


clean:
	rm -rf $(EXE) $(TEST_EXE) $(OBJ_DIR) $(TEST_DB)


-include $(OBJ:.o=.d)


