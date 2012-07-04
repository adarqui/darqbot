all:
	gcc extract_quotes.c -Wall -o extract_quotes
	gcc script_extract.c -Wall -g3 -ggdb -o script_extract
	gcc script_extract2.c -Wall -g3 -ggdb -I ../include ../dlist.c -o script_extract2 
	gcc trace_print.c -Wall -ggdb -I ../include -o ./trace_print
	gcc fix_phrack.c -Wall -I ../include ../dlist.c -o fix_phrack

clean:
	rm -f ./extract_quotes ./script_extract ./script_extract2 ./trace_print ./fix_phrack
