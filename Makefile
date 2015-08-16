BUILD_DIR=BuildDir
BITSTREAM_TEST=$(BUILD_DIR)/BitstreamTest.exe
CC=g++
CFLAGS=-c -Wall -O0 -g3 -std=c++11
INC=-Ilib

.cpp.o:
	$(CC) $(CFLAGS) $(INC) $< -o $@

test: $(BITSTREAM_TEST)
	./$(BITSTREAM_TEST)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BITSTREAM_TEST): $(BUILD_DIR)/BitstreamTest.o
	$(CC) -o $(BITSTREAM_TEST) $(BUILD_DIR)/BitstreamTest.o

$(BUILD_DIR)/BitstreamTest.o: test/BitstreamTest.cpp lib/Bitstream.h $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC) -o$(BUILD_DIR)/BitstreamTest.o test/BitstreamTest.cpp

clean:
	rm -rf $(BUILD_DIR)

