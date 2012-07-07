#FLAGS = -Wall -O3 --std=c99 -finstrument-functions
FLAGS= -Wall -O3 --std=c99 -D_GNU_SOURCE

DEBUG_FLAGS = -fprofile-arcs -ftest-coverage

SOURCES = bot.c misc.c conf.c dlist.c bot_network.c module.c mongodb.c stat.c global.c environ.c tokenize.c protocol.c randchar.c matchchar.c bot_unix.c safe.c debug.c pmodule.c gmodule.c xmodule.c zmodule.c fns.c console.c files.c xdb.c timer.c unix.c bit.c relink.c swap_inmem.c fd.c pid.c control.c unique.c avl.c swap_disk.c chunks.c sim.c fdpass.c

HEADERS = -I ./include -I ../include/ -I./mongodb-c-driver/src/

LINKER = -ggdb -g -lbsd -levent -ldl -lrt -rdynamic -lguile-2.0 ./mongodb-c-driver/libbson.a ./mongodb-c-driver/libmongoc.a -lssl -ldb -lm -lmatheval -lutil -lz -lcrypto -lcrypt

all:
	cd mongodb-c-driver && make
	gcc $(FLAGS) bot_wrapper.c -o ./bot_wrapper
	$(HACKENVS) gcc $(HEADERS) $(FLAGS) $(SOURCES) $(LINKER) main.c -o ./bot
	$(HACKENVS) gcc $(HEADERS) $(FLAGS) $(SOURCES) $(LINKER) main_rootd.c -o bot_rootd

run2:
	#rm -f /darqbot/.darqbot/mods/mod_quiz_files/__*
	mv -f ./bot /darqbot/bot_main_process && mv -f ./bot_wrapper /darqbot && cd / && cd /darqbot && $(HACKENVS) ./bot_wrapper
run2_rootd:
	#rm -f /darqbot/.darqbot/mods/mod_quiz_files/__*
	mv -f ./bot_rootd /darqbot/bot_rootd && cd  && cd /darqbot && ./bot_rootd -f /darqbot/conf_rootd -u /tmp/unix_rootd.sock

run:
	#rm -f /darqbot/.darqbot/mods/mod_quiz_files/__*
	cp ./bot /darqbot/bot_main_process
	cp ./bot_wrapper /darqbot
	cd / && cd /darqbot && $(HACKENVS) ./bot_wrapper
run_rootd:
	#rm -f /darqbot/.darqbot/mods/mod_quiz_files/__*
	cp ./bot_rootd /darqbot/bot_rootd
	cd / && cd /darqbot && $(HACKENVS) ./bot_rootd -f /darqbot/conf_rootd -u /tmp/unix_rootd.sock

mongo:
	cd mongodb-c-driver
	make clean
	make

world:
	make all
	cd mods && make -i 2>&1 && make install

clean:
	cd mongodb-c-driver && make clean
	rm -f ./bot
	rm -f ./bot_wrapper
	rm -f ./bot_rootd
	cd mods && make clean

