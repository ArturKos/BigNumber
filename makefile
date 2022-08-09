FLAGS=-Wall -Wextra
OBJS=main.cpp
TEST=test.cpp

cut: $(OBJS)
	$(CXX) $(FLAGS) $(OBJS) -o bignumber

test: $(TEST)
		$(CXX) $(FLAGS) $(TEST) -o test
clean:
		rm -f *.o
