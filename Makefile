# makefile 
VERSION = 1.0.0
CC = gcc
TARGET = tft_demo
DEBUG = -Wall -c

src_path = ./ ./src/
src = $(foreach dir, $(src_path), $(wildcard $(dir)*.c))

obj_name = $(patsubst %.c, %.o, $(src))

INC = -I./
INC += -I./include/ 


$(TARGET) : $(obj_name) 
	$(cc)   $(obj_name) -o $(TARGET) -L./lib -lgpiod
	@echo "\nmakefile compile version :$(VERSION), target :$(TARGET)\n"

%.o : %.c
	$(CC) $(DEBUG) $(INC) $^ -o $@ 
	
.PHONY :
clean :
	@echo "start clean target and objects \n"
	rm $(TARGET) $(obj_name)

