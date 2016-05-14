OS_NAME = $(shell uname -o)
LC_OS_NAME = $(shell echo $(OS_NAME) | tr '[A-Z]' '[a-z]')
OPT = .out
ifeq ($(LC_OS_NAME), msys)
OPT = .exe
endif
ifeq ($(LC_OS_NAME), cygwin)
OPT = .exe
endif

CMP_CMD = g++ --std=gnu++11 -O3 -w
LINK = -lcurl -pthread

.PHONY: test au2pp perform idwithid test_thread

test:
	$(CMP_CMD) test.cpp -o test$(OPT) $(LINK)

au2pp:
	$(CMP_CMD) au2pp.cpp -o au2pp$(OPT) $(LINK)

pp2au:
	$(CMP_CMD) pp2au.cpp -o pp2au$(OPT) $(LINK)

pp2pp:
	$(CMP_CMD) pp2pp.cpp -o pp2pp$(OPT) $(LINK)

au2au:
	$(CMP_CMD) au2au.cpp -o au2au$(OPT) $(LINK)

pp2au_test:
	$(CMP_CMD) pp2au_test.cpp -o pp2au_test$(OPT) $(LINK)

au2au_test:
	$(CMP_CMD) au2au_test.cpp -o au2au_test$(OPT) $(LINK)

pp2pp_test:
	$(CMP_CMD) pp2pp_test.cpp -o pp2pp_test$(OPT) $(LINK)

au2pp_test:
	$(CMP_CMD) au2pp_test.cpp -o au2pp_test$(OPT) $(LINK)

perform:
	$(CMP_CMD) perform.cpp -o perform$(OPT) -lfcgi++ -lcurl -lfcgi -pthread

test_thread:
	$(CMP_CMD) test_thread.cpp -o test_thread.out -pthread -lcurl
