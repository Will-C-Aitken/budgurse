CC = gcc
CFLAGS = -Wall -g
CPPFLAGS = -MMD -MP
LDLIBS = -lsqlite3


SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .

EXE = $(BIN_DIR)/budgurse
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TEST_DIR = test
TEST_EXE = $(TEST_DIR)/test
TEST_SRC = $(TEST_DIR)/test.c

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

test: $(TEST_EXE)
	./$(TEST_EXE)


$(TEST_EXE): $(TEST_SRC) $(OBJ_DIR)/entries.o
	$(CC) $(CFLAGS) $^ -I$(SRC_DIR) $(LDLIBS) -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@


clean:
	rm -rf $(EXE) $(TEST_EXE) $(OBJ_DIR)

-include $(OBJ:.o=.d)


