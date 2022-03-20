CC = gcc -std=c11
CFLAGS = -Wall -Werror -Wextra
TESTTARGET = s21_test_decimal
OS := $(shell uname -s)

ifeq ($(OS), Linux)
	OPEN = xdg-open
	ADD_LIB = -lm -lrt -lpthread -D_GNU_SOURCE
	LIBS = -lcheck -lsubunit -lrt -lpthread -lm
	BREW = .
endif
ifeq ($(OS), Darwin)
	OPEN = open
	ADD_LIB = 
	LIBS = -lcheck
	BREW = {$HOME}/homebrew
endif

PATH := $(PATH):$(BREW)/bin
GCOV = --coverage

all: clean s21_decimal.a

$(TARGET): s21_decimal.o s21_decimal.a
	$(CC) s21_decimal.o $(CFLAGS) $(LIBS) $(GCOV) -L. s21_decimal.a -o $@

s21_decimal.o: s21_decimal.c
	$(CC) -c s21_decimal.c $(CFLAGS) $(LIBS) -o $@

s21_decimal.a: s21_decimal.o
	ar rcs $@ $^
	ranlib $@
	cp $@ lib$@

run: clean start

start:
	gcc s21_decimal.c $(CFLAGS) -lm
	./a.out

gcov_report: clean test/s21_test_decimal.c s21_decimal.c
	-mkdir report
	$(CC) $(GCOV) test/s21_test_decimal.c s21_decimal.c $(CFLAGS) $(ADD_LIB) $(LIBS) -lcheck
	$(CC) $(GCOV) -c s21_decimal.c $(CFLAGS) $(LIBS) -o s21_decimal.o
	$(CC) $(GCOV) -c test/s21_test_decimal.c $(CFLAGS) $(LIBS) -o test/s21_test_decimal.o
	$(CC) $(GCOV) -o $@ test/s21_test_decimal.o s21_decimal.o $(CFLAGS) $(LIBS) -lcheck
	-./$@
	gcov test/s21_test_decimal.c
	lcov -t test/s21_test_decimal -o test/s21_test_decimal.info -c -d .
	genhtml -o report test/s21_test_decimal.info
	$(OPEN) ./report/index.html

$(TESTTARGET): s21_decimal.a test/s21_test_decimal.o s21_decimal.o
	$(CC) test/s21_test_decimal.c s21_decimal.o $(CFLAGS) $(LIBS) -o $@
	

test/s21_test_decimal.o: test/s21_test_decimal.c s21_decimal.h
	$(CC) $(CFLAGS) $(LIBS) -c test/s21_test_decimal.c -o $@ 
	$(CC) $(CFLAGS) $(LIBS) -c s21_decimal.c -o $@

test: $(TESTTARGET)
	./$(TESTTARGET)

clean:	
		rm -rf math.a
		rm -rf gcov_report
		rm -rf s21_decimal
		rm -rf s21_test_decimal
		rm -rf *.o *.so *.a fizz *.gc* *.info report *.out *.gcda *.so *.gcno *.info
		rm -rf test/*.o test/*.so test/*.a  test/*.gcda  test/*.so  test/*.gcno  test/*.info

rebuild: clean all

.PHONY: all clean test rebild math.a