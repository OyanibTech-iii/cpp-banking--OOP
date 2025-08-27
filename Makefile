# Makefile for Oyanib Banking System
# Advanced C++ OOP Implementation

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = oyanib_bank
SOURCES = main.cpp User.cpp Customer.cpp Account.cpp SavingsAccount.cpp Transaction.cpp Loan.cpp BankingSystem.cpp
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = User.h Customer.h Account.h SavingsAccount.h Transaction.h Loan.h BankingSystem.h

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)
	@echo "Build completed successfully!"
	@echo "Run with: ./$(TARGET)"

# Compile source files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean completed!"

# Run the program
run: $(TARGET)
	./$(TARGET)

# Install dependencies (for Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y build-essential g++

# Create backup
backup:
	@echo "Creating backup..."
	@if [ -d backup ]; then rm -rf backup; fi
	@mkdir backup
	@cp *.cpp *.h Makefile backup/
	@echo "Backup created in backup/ directory"

# Show help
help:
	@echo "Available targets:"
	@echo "  all        - Build the banking system (default)"
	@echo "  clean      - Remove build files"
	@echo "  run        - Build and run the program"
	@echo "  install-deps - Install build dependencies"
	@echo "  backup     - Create backup of source files"
	@echo "  help       - Show this help message"

# Phony targets
.PHONY: all clean run install-deps backup help
