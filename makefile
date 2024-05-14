GCC ?= g++
CCMODE = PROGRAM
INCLUDES =  -I/opt/homebrew/opt/libevent/include/
# CFLAGS =  -Wall $(MACRO) 
CFLAGS =  -Wall -std=c++17 $(MACRO)
TARGET = ftpSrv
SRCS := $(wildcard *.cpp)   
LIBS = -L /opt/homebrew/opt/libevent/lib/  -levent -lpthread



ifeq ($(CCMODE),PROGRAM)
$(TARGET): $(LINKS) $(SRCS) 
	$(GCC) $(CFLAGS) $(INCLUDES) -o $(TARGET)  $(SRCS) $(LIBS)
	@chmod +x $(TARGET)
	@echo make $(TARGET) ok.
clean:
	rm -rf $(TARGET)
endif

 
clean:
	rm -f $(TARGET)
 
.PHONY:install
.PHONY:clean
