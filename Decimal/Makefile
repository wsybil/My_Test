CC = gcc -std=c11
CFLAGS = -Wall -Werror -Wextra
TESTTARGET = decimal_test
PROJECTNAME = s21_decimal
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

all: clean $(PROJECTNAME).a

$(TARGET): $(PROJECTNAME).o $(PROJECTNAME).a
	$(CC) $(PROJECTNAME).o $(CFLAGS) $(LIBS) $(GCOV) -L. $(PROJECTNAME).a -o $@

$(PROJECTNAME).o: $(PROJECTNAME).c
	$(CC) -c $(PROJECTNAME).c $(CFLAGS) $(LIBS) -o $@

$(PROJECTNAME).a: $(PROJECTNAME).o
	ar rcs $@ $^
	ranlib $@
	cp $@ lib$@

gcov_report: clean $(TESTTARGET).c $(PROJECTNAME).c
	-mkdir report
	$(CC) $(GCOV) $(TESTTARGET).c $(PROJECTNAME).c $(CFLAGS) $(ADD_LIB) $(LIBS) -lcheck
	$(CC) $(GCOV) -c $(PROJECTNAME).c $(CFLAGS) $(LIBS) -o $(PROJECTNAME).o
	$(CC) $(GCOV) -c $(TESTTARGET).c $(CFLAGS) $(LIBS) -o $(TESTTARGET).o
	$(CC) $(GCOV) -o $@ $(TESTTARGET).o $(PROJECTNAME).o $(CFLAGS) $(LIBS) -lcheck
	-./$@
	gcov $(TESTTARGET).c
	lcov -t $(TESTTARGET) -o $(TESTTARGET).info -c -d .
	genhtml -o report test/s21_test_decimal.info
	$(OPEN) ./report/index.html

$(TESTTARGET): $(PROJECTNAME).a $(TESTTARGET).o $(PROJECTNAME).o
	$(CC) $(TESTTARGET).c $(PROJECTNAME).o $(CFLAGS) $(LIBS) -o $@
	

$(TESTTARGET).o: $(TESTTARGET).c $(PROJECTNAME).h
	$(CC) $(CFLAGS) $(LIBS) -c $(TESTTARGET).c -o $@ 
	$(CC) $(CFLAGS) $(LIBS) -c $(PROJECTNAME).c -o $@

test: $(TESTTARGET)
	./$(TESTTARGET)

clean:	
		rm -rf math.a
		rm -rf gcov_report
		rm -rf $(PROJECTNAME)
		rm -rf $(TESTTARGET)
		rm -rf *.o *.so *.a fizz *.gc* *.info report *.out *.gcda *.so *.gcno *.info

rebuild: clean all

.PHONY: all clean test rebild math.a

check:
	cppcheck --enable=all --suppress=missingIncludeSystem s21_*.c s21_*.h tests/*.c
	cp ../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../materials/linters/cpplint.py --extensions=c ../src/s21_*.c ../src/s21_*.h; rm CPPLINT.cfg
ifeq ($(UNAME), Darwin)
	CK_FORK=no leaks --atExit -- ./$(TESTTARGET)
endif
ifeq ($(UNAME), Linux)
	valgrind ./$(TESTTARGET)
endif
