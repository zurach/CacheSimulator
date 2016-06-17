CC = g++
CFLAGS = -Wall -Werror -pedantic -std=c++11 -O3
OBJECTS = main.o full_asso_cache.o associative_cache.o cache.o
TARGET = cache-sim
PROJECT = ubuyukd1
FOLDER = ./ubuyukd1
SRC_FILES = \
	main.cpp \
	cache.cpp \
	cache.h \
	associative_cache.cpp \
	associative_cache.h \
	full_asso_cache.cpp \
	full_asso_cache.h \
	common_def.h \
	README \
	makefile

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

main.o: main.cpp common_def.h
	$(CC) $(CFLAGS) -c main.cpp

full_asso_cache.o: full_asso_cache.cpp full_asso_cache.h
	$(CC) $(CFLAGS) -c full_asso_cache.cpp

associative_cache.o: associative_cache.cpp associative_cache.h
	$(CC) $(CFLAGS) -c associative_cache.cpp

cache.o: cache.cpp cache.h
	$(CC) $(CFLAGS) -c cache.cpp

clean: 
	rm -f $(OBJECTS) $(TARGET)
	rm -rf $(FOLDER)

submit:
	mkdir $(FOLDER)
	cp $(SRC_FILES) $(FOLDER)
	tar -czvf $(PROJECT).tar.gz $(FOLDER)
