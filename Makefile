OS_NAME = $(shell uname -o)
LC_OS_NAME = $(shell echo $(OS_NAME) | tr '[A-Z]' '[a-z]')
ifeq ($(LC_OS_NAME), msys)
OPT = .exe
else 
OPT = .out
endif

CMP_CMD = g++ --std=gnu++11 -O3

.PHONY: test au2pp
test:
	$(CMP_CMD) test.cpp -o test$(OPT) -lcurl

au2pp:
	$(CMP_CMD) au2pp.cpp -o au2pp$(OPT) -lcurl

perform:
	$(CMP_CMD) perform.cpp -o perform$(OPT) -lfcgi++ -lcurl -lfcgi
