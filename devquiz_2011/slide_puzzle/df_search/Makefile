CC = g++ -g
CFLAGS = -Wall -O2
LOADLIBES = -lm
OBJS = solve_puzzle.o df_search.o

solve_puzzle:	$(OBJS)
			$(CC) $(CFLAGS) $(OBJS) -o solve_puzzle

df_search.o	:	search.hpp hash.hpp init.hpp output.hpp

clean:
			rm -f $(OBJS) *~ *.bak
