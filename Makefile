CC = g++
DEBUG_FLAGS = -g -Wall -Wextra -Wpedantic -std=c++23 -I. -I$(HEADERS_DIR) -I$(SOURCES_DIR)
RELEASE_FLAGS = -O3 -Wall -Wextra -Wpedantic -std=c++23 -I. -I$(HEADERS_DIR) -I$(SOURCES_DIR)
TARGET = todo
SOURCES = $(wildcard *.cpp $(SOURCES_DIR)*.cpp $(HEADERS_DIR)*.cpp)
HEADERS = $(wildcard *.h *.hpp $(HEADERS_DIR)*.hpp $(HEADERS_DIR)*.h $(SOURCES_DIR)*.h $(SOURCES_DIR)*.hpp)
SOURCES_DIR = ./src/
HEADERS_DIR = ./include/
INSTALL_PATH = /usr/local/bin

all: debug

debug: $(SOURCES) $(HEADERS)
	@echo "Building debug version..."
	$(CC) $(DEBUG_FLAGS) $(SOURCES) -o $(TARGET)

release: $(SOURCES) $(HEADERS)
	@echo "Building release version..."
	$(CC) $(RELEASE_FLAGS) $(SOURCES) -o $(TARGET)

run: debug
	@echo "Running $(TARGET)..."
	./$(TARGET)

clean:
	@echo "Cleaning build artifacts..."
	rm -f $(TARGET) *.o

install: $(TARGET)
	@echo "Installing $(TARGET) to $(INSTALL_PATH)..."
	sudo install -m 755 $(TARGET) $(INSTALL_PATH)/$(TARGET)

uninstall:
	@echo "Removing $(TARGET) from system..."
	sudo rm -f $(INSTALL_PATH)/$(TARGET)

remove: clean uninstall

deploy: remove debug install

help:
	@echo "Available targets:"
	@echo "  all       - Build debug version (default)"
	@echo "  debug     - Build debug version"
	@echo "  release   - Build release version"
	@echo "  run       - Build and run debug version"
	@echo "  clean     - Remove build artifacts"
	@echo "  install   - Install to system (requires sudo)"
	@echo "  uninstall - Remove from system (requires sudo)"
	@echo "  deploy    - Uninstall, build, and install"
	@echo "  remove    - Clean and uninstall"
	@echo "  help      - Show this help message"

.PHONY: all debug release run clean install uninstall deploy remove help
