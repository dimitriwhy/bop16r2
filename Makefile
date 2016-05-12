OS_NAME = $(shell uname -o)
LC_OS_NAME = $(shell echo $(OS_NAME) | tr '[A-Z]' '[a-z]')
ifeq ($(LC_OS_NAME), msys)
OPT = .exe
else 
OPT = .out
endif

CMP_CMD = g++ --std=gnu++11 -O3

.PHONY: test au2pp perform idwithid test_thread

test:
	$(CMP_CMD) test.cpp -o test$(OPT) -lcurl -pthread

au2pp:
	$(CMP_CMD) au2pp.cpp -o au2pp$(OPT) -lcurl

perform:
	$(CMP_CMD) perform.cpp -o perform$(OPT) -lfcgi++ -lcurl -lfcgi -pthread

idwithid:
	$(CMP_CMD) idwithid.cpp -o idwithid$(OPT) -lcurl -g

test_thread:
	$(CMP_CMD) test_thread.cpp -o test_thread.out -pthread -lcurl
