
SRC_DIR = .

SRC = $(wildcard $(SRC_DIR)/*.cpp)
BIN = $(patsubst %.cpp, %, $(SRC))

.PHONY: all
all: $(BIN)
$(SRC_DIR)/% : $(SRC_DIR)/%.cpp
	g++ $< -o $@

.PHONY: clean
clean:
	rm $(BIN)
