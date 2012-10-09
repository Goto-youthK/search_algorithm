CC = g++ -g
CFLAGS = -Wall -O2
LOADLIBES = -lm
OBJS = solve_puzzle.o bi_dir_a_star_search.o

solve_puzzle:	$(OBJS)
			$(CC) $(CFLAGS) $(OBJS) -o solve_puzzle

bi_dir_a_star_search.o	:	search.hpp hash.hpp init.hpp output.hpp

clean:
			rm -f $(OBJS) *~ *.bak
