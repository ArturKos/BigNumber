FLAGS=-Wall -Wextra
OBJS=main.cpp include/bignumber.cpp include/digit.cpp
TEST=test.cpp include/bignumber.cpp include/digit.cpp

cut: $(OBJS)
	$(CXX) $(FLAGS) $(OBJS) -o bignumber

test: $(TEST)
		$(CXX) $(FLAGS) $(TEST) -o test
clean:
		rm -f *.o
