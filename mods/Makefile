#FLAGS = -Wall -O3
FLAGS = -Wall --std=c99 -rdynamic -finstrument-functions
HEADERS = -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include
LINKER = -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/


# mods - trigger modules, reacting to text etc

MOD_ECHO = mod_echo.c
MOD_CAPS = mod_caps.c
MOD_ROT13 = mod_rot13.c
MOD_ATTRIB = mod_attrib.c
MOD_COLOR = mod_color.c
MOD_UNDERLINE = mod_underline.c
MOD_SORT = mod_sort.c
MOD_LEET = mod_leet.c
MOD_MOBY = mod_moby.c
MOD_SHAKE1 = mod_shake1.c
MOD_QUOTE = mod_quote.c
MOD_DUMBGAMES = mod_dumbgames.c
MOD_MD5 = mod_md5.c
MOD_EMOT = mod_emot.c
MOD_TO = mod_to.c
MOD_HELP = mod_help.c
MOD_SEP = mod_sep.c
MOD_TINYPAW = mod_tinypaw.c
MOD_PCRE = mod_pcre.c
MOD_HUN = mod_hun.c
MOD_NOP = mod_nop.c
MOD_AUTO_REJOIN = mod_auto_rejoin.c
MOD_CHAN = mod_chan.c
MOD_RAND = mod_rand.c
MOD_WHOAMI = mod_whoami.c
MOD_STRING = mod_string.c
MOD_PMME = mod_pmme.c
MOD_RSS = mod_rss.c
MOD_TINY = mod_tiny.c
MOD_MULTI = mod_multi.c
MOD_VAR = mod_var.c
MOD_HOWIE = mod_howie.c
MOD_SP = mod_sp.c
MOD_ALIAS = mod_alias.c
MOD_GUILE = mod_guile.c
MOD_RAINBOW = mod_rainbow.c
MOD_PACKMULTI = mod_packmulti.c
MOD_SMF = mod_smf.c
MOD_IDENTIFY = mod_identify.c
MOD_SYSVBANNER = mod_sysvbanner.c
MOD_CHAR = mod_char.c
MOD_RF = mod_rf.c
MOD_GEOIP = mod_geoip.c
MOD_MONGOTESTFMT = mod_mongotestfmt.c
MOD_FESTIVAL = mod_festival.c
MOD_BACKTRACE = mod_backtrace.c
MOD_VULN = mod_vuln.c
MOD_UU = mod_uu.c
MOD_STACK = mod_stack.c
MOD_QUEUE = mod_queue.c
MOD_SAVE = mod_save.c
MOD_FNSC = mod_fnsc.c
MOD_FNSASMX86 = mod_fnsasmx86.c
MOD_FORTUNE = mod_fortune.c
MOD_QUIZ = mod_quiz.c
MOD_CONSOLE = mod_console.c
MOD_SIG = mod_sig.c
MOD_TIMER = mod_timer.c
MOD_PRINT = mod_print.c
MOD_BITVEC = mod_bitvec.c
MOD_RELINKD = mod_relinkd.c
MOD_MAP = mod_map.c
MOD_EXEC = mod_exec.c
MOD_BSDAVLTREE = mod_bsdavltree.c
MOD_RANT = mod_rant.c
MOD_CMDS = mod_cmds.c
MOD_TAIL = mod_tail.c
MOD_FORK = mod_fork.c
MOD_HTML = mod_html.c
MOD_ME = mod_me.c
MOD_BIN = mod_bin.c
MOD_INTERNALS = mod_internals.c
MOD_CAT = mod_cat.c
MOD_AVL = mod_avl.c
MOD_XML = mod_xml.c

# pmods - parser modules, for handling how triggers are run/interpreted

PMOD_PARSE2 = pmod_parse2.c

# gmods - protocol/communication modules, for handling data/connections etc

GMOD_GRAW = gmod_graw.c
GMOD_GRELAY = gmod_grelay.c
GMOD_GIRC = gmod_girc.c
GMOD_GRELINKD = gmod_grelinkd.c
GMOD_GRELINKC = gmod_grelinkc.c
GMOD_GIRCD = gmod_gircd.c
GMOD_GNULL = gmod_gnull.c
GMOD_GDAEMON = gmod_gdaemon.c
GMOD_GIRCPONG = gmod_gircpong.c
GMOD_GSSL = gmod_gssl.c
GMOD_GPTY = gmod_gpty.c
GMOD_GEXEC = gmod_gexec.c

all:

	mkdir /darqbot/.darqbot/mods &
	mkdir /darqbot/.darqbot/pmods &
	mkdir /darqbot/.darqbot/gmods &

	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(GMOD_GEXEC) -o gmod_gexec.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(GMOD_GPTY) -o gmod_gpty.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(GMOD_GSSL) -o gmod_gssl.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(GMOD_GIRCPONG) -o gmod_gircpong.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(GMOD_GRELINKD) -o gmod_grelinkd.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(GMOD_GRELINKC) -o gmod_grelinkc.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(GMOD_GDAEMON) -o gmod_gdaemon.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(GMOD_GNULL) -o gmod_gnull.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(GMOD_GRAW) -o gmod_graw.so #...
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(GMOD_GIRCD) -o gmod_gircd.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(GMOD_GRAW) -o gmod_graw.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(GMOD_GRELAY) -o gmod_grelay.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(GMOD_GIRC) -o gmod_girc.so

	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(PMOD_PARSE2) -o pmod_parse2.so

	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared -I /usr/include/libxml2 -lxml2 $(MOD_XML) -o mod_xml.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_BIN) -o mod_bin.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_GUILE) -I/usr/include/guile -lguile-2.0 -o mod_guile.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared -I ./mod_avl_files/ ./mod_avl_files/texilib.c $(MOD_AVL) -o mod_avl.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_TIMER) -o mod_timer.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_FNSASMX86) -o mod_fnsasmx86.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_GEOIP) -lGeoIP -o mod_geoip.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_RF) -o mod_rf.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared -lcurl -lxml2 -I /usr/include/libxml2/ $(MOD_RSS) -o mod_rss.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_SORT) -o mod_sort.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_STRING) -o mod_string.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_RAND) -o mod_rand.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_SEP) -o mod_sep.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_DUMBGAMES) -o mod_dumbgames.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_COLOR) -o mod_color.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_ATTRIB) -o mod_attrib.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_QUOTE) -o mod_quote.so #...
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_HELP) -o mod_help.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_CAT) -o mod_cat.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_INTERNALS) -o mod_internals.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(GMOD_GEXEC) -o gmod_gexec.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(GMOD_GPTY) -o gmod_gpty.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared -lmatheval -lrecode $(MOD_ME) -o mod_me.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_HTML) -o mod_html.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_FORK) -o mod_fork.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_TAIL) -o mod_tail.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(GMOD_GSSL) -o gmod_gssl.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_MOBY) -o mod_moby.so #...
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_RELINKD) -o mod_relinkd.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_CMDS) -o mod_cmds.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_RANT) -o mod_rant.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_BSDAVLTREE) -o mod_bsdavltree.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_STACK) -o mod_stack.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_EXEC) -o mod_exec.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_MAP) -o mod_map.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_RELINKD) -o mod_relinkd.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_ALIAS) -o ./mod_alias.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_BITVEC) -o mod_bitvec.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_PRINT) -o mod_print.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_CHAN) -o mod_chan.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_SAVE) -o mod_save.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_FNSC) -o mod_fnsc.so
	(cd libfortune && make) ; gcc $(HEADERS) $(FLAGS) $(LINKER) -shared ./libfortune/fortune/libfortune.o -lrecode -lbsd $(MOD_FORTUNE) -o mod_fortune.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_QUIZ) -o mod_quiz.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_CONSOLE) -o mod_console.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_SIG) -o mod_sig.so

	#gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_FORTUNE) -o mod_fortune.so

# mods
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_ECHO) -o mod_echo.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_CAPS) -o mod_caps.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_ROT13) -o  mod_rot13.so
	#gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_ATTRIB) -o mod_attrib.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_LEET) -o mod_leet.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_MOBY) -o mod_moby.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_SHAKE1) -o mod_shake1.so
	#gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_QUOTE) -o mod_quote.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_REVERSE) -o mod_reverse.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -lbsd -lssl -shared $(MOD_MD5) -o mod_md5.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_EMOT) -o mod_emot.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_TO) -o mod_to.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_HELP) -o mod_help.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared -lcurl $(MOD_TINYPAW) -o mod_tinypaw.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared -lpcre $(MOD_PCRE) -o mod_pcre.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared -lhunspell $(MOD_HUN) -o mod_hun.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_NOP) -o mod_nop.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_AUTO_REJOIN) -o mod_auto_rejoin.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_WHOAMI) -o mod_whoami.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_PMME) -o mod_pmme.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared -lcurl $(MOD_TINY) -o mod_tiny.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_VAR) -o mod_var.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_HOWIE) -o mod_howie.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_SP) -o mod_sp.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_ALIAS) -o ./mod_alias.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_RAINBOW) -o mod_rainbow.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_PACKMULTI) -o mod_packmulti.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared -lcurl $(MOD_SMF) -o mod_smf.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_IDENTIFY) -o mod_identify.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_SYSVBANNER) -o mod_sysvbanner.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_CHAR) -o mod_char.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_MONGOTESTFMT) -o mod_mongotestfmt.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_FESTIVAL) -o mod_festival.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_BACKTRACE) -o mod_backtrace.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_VULN) -o mod_vuln.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_UU) -o mod_uu.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_STACK) -o mod_stack.so
	gcc $(HEADERS) $(FLAGS) $(LINKER) -shared $(MOD_QUEUE) -o mod_queue.so

install:
	mv -f mod*.so /darqbot/.darqbot/mods &
	mv -f pmod*.so /darqbot/.darqbot/pmods &
	mv -f gmod*.so /darqbot/.darqbot/gmods &

clean:
	rm -f *.so
	cd libfortune && make clean
