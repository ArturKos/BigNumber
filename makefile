FLAGS=-Wall -Wextra -O0 -g -pedantic
OBJS=main.cpp src/bignumber.cpp src/digit.cpp
TEST=test.cpp src/bignumber.cpp src/digit.cpp

.PHONY: bignumber 
.PHONT: test 

bignumber: $(OBJS)	
	$(CXX) $(FLAGS) $(OBJS) -o bignumber

test: $(TEST)
		$(CXX) $(FLAGS) $(TEST) -o test
clean:
		rm -f *.o
