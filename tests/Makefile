FLAGS = -std=c99 -Wall

HEADERS = -I ../include -I../mongodb-c-driver/src/

LINKER = -ggdb -g -lbsd -levent -ldl -lrt -rdynamic -lguile-2.0 ../mongodb-c-driver/libbson.a ../mongodb-c-driver/libmongoc.a -lssl -ldb -lm -lmatheval -lutil -lz -lcrypto -lcrypt

all:
	gcc $(FLAGS) $(HEADERS) $(LINKER) structs.c -S

clean:
	rm structs.s
