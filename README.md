darqbot
=======

Extremely versatile &amp; modular knowledge bot written in C.Darqbot is provided under Gnu Public License, See COPYING for details.

     Copyright (C) 2012 Andrew Darqui

     This file is part of darqbot.

     darqbot is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation, either version 3 of the License, or
     (at your option) any later version.

     darqbot is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with darqbot.  If not, see <http://www.gnu.org/licenses/>.

     Contact: [website: http://www.adarq.org]
      email: andrew.darqui@g m a i l . c o m




darqbot v0.1 - July 3, 2012.
By: Andrew Darqui (andrew.darqui@g m a i l . c o m]
Website: http://www.adarq.org


darqbot is Version 0.1 at the time of this README, there is much to be done. This document is by no means comprehensive. Comprehensive documentation (including code comments & flow paths)) and proper portability across multiple *nix Operating Systems will be achieved by version 1.0. 

Join us in #jumping @ EFNet. Also special thanks to those in #jumping, #bothole, and #C @ EFNet for their occasional feedback.




.,:::PREAMBLE:::,.
------------------

PUSHAF.

The goal of darqbot is to provide a "knowledge bot" with very unique & extremely versatile functionality, much of which is geared towards facilitating learning of a group of people on a social chat network. The overall design of this bot is inspired by my attempt to clone my brain into software. 

Keep in mind this is version 0.1, which means this software is still in it's infantile stage. Surely there will be plenty of bug fixes, refactoring, and optimization to come. It is running pretty good in Valgrind, though I do not free the swap_inmem structures because those are to remain frozen in time until an absolute exit. That being said, I am finally happy with the overall core & structure of the bot. Due to the stackable communication layers and overall modularity of the bot, it has finally reached a stage where it can really start growing. I plan to keep this project going for years, adding important functionality as well as "test code" for api's and such as modules, for examples & general toying around.

POPAF.




.,;;;TOC:::,.
-------------

What follows is a brief explanation of:
- Currently implemented and future checklists
- Quick build example
- Very brief one sentence explanation of each module
- Brief explanation of the various example conf files
- Brief demonstration of each module



.,:::CHECKLISTS:::,.
--------------------

Below you will find the currently implemented checklist (0.1) and the checklists for future versions 1.0 & 2.0. Forgive me if I leave out anything.

Version 0.1 Checklist:
---------------------'
[x] Every communication component is modularized via dynamically loaded shared objects
    mods : modules that offer services to end users
    pmods: parser modules that create a language to provide services by mods, to end users
    gmods: lower level communication modules used to establish a link in order to reach end users
[x] GMOD communication is layered: each gmod handles its own layer to reduce redundant code & improve modularity.
    gmod's can be stacked in a variety of manners to achieve more complex solutions, for example:
     a. graw <-> girc <-> grelay <-> parse2 <-> mods  <-- creates a raw tcp connection and parses it via girc (irc module)
     b. graw <-> gssl <-> girc <-> grelay <-> parse2 <-> mods <-- does the same as above but now incorporates ssl
     c. gdaemon <-> graw <-> gssl <-> girc <-> grelay <-> parse2 <-> mods <-- daemonized ...
    Data passes up and down the gmod <-> pmod <-> mod0 <-> modN chain.
    Control data & destruction data similarly passes up & down that chain.
    Any module can tap into all lowest level input/output data by setting their mod->input & mod->output fnptr's
[x] Ability to unload, load, reload, lock, and unlock any module.
[x] Modules can save/restore their structures via swap_inmem.c routines. Allows modules to be reloaded & data to remain unchanged.
[x] Event-driven IO achieved via libevent. Avoids the need for threading or forking.
[x] Every darqbot function can be interposed & logged via the "-finstrument-functions" gcc switch.
[x] Create a fun & effective "social testbed" for testing various libraries & programs (mod_avl.c for example)
[x] Darqbot can fight through various memory & stack corruption + invalid instructions via "sigprotect", which longjmp's to a "safe" context
[x] Modules can use Berkley DB (xdb.c) or MongoDB (mongodb.c) primitives. 
[x] pmod_parse2 provides a powerful "pipeable lisp-like syntax" capable of deep nesting, which end-users can use to interactive with the bot. ie:
    ^e (^echo hello there|^rand(word)) |^md5|^space(1)|^sort|^^(^rand caps moby leet))
[x] The relink daemon (gmod_grelnkd) and relink client (gmod_grelinkc) allow for the bot to be re-programmed without a loss of connectivity. Conf files which utilize the relink client, contact the relinkd daemon upon restarting & resync the previous connection.
[x] Ability to spawn various daemons such as gmod_gircd (ircd server). Not only can darqbot connect to irc, but it can also spawn an ircd in one conf file & connect to that same ircd in another. gmod_gircd provides a no-flooding minimal daemon for testing or "more fun".
    - Spawned services can create portals into the inter-workings of the bot, such as the #console channel in gmod_gircd.
[x] Can run in a chroot environment.
[x] Very easy module API (mods especially). Contributors not wishing to work on the core, but instead contribute to "fun modules", can do so very easily. mod_echo.c & gmod_gnull.c are the simplest of examples.
[x] A simple timer interface is provided through timer.c & modules can make use of it by setting their mod_xxx_info.timer pointer to a timer handler.
[x] "powerful" dlist_t structure, capable of handling doubly linked lists, arrays, and avl trees.
[x] "powerful" tokenizer using dlist_t
[x] "powerful" unique.c interface, removes the need for modules to maintain data based on channel or user for example, instead, it is centralized within the unique.c routines based on a bitwise or of any of these flags: ID, TAG, HOST, CHAN, NICK.
[x] bit-vector primitives (bit.c)
[x] customizable "module organizer" which dictates the internal representation of modules and how they are accessed (currently by list or avl tree).
[x] Stat collection (usage statistics) per trigger/module.
[x] Very powerful embedded scheme interpreter (gnu guile)
[x] Custom trigger prefixes, default trigger prefix is ^.



Version 1.0 Checklist:
---------------------'
[-] autoconf/configure, portable across *BSD, Solaris, & Linux
[-] Thorough source code documentation (flow etc)
[-] Thorough functionality documentation (config files, triggers, etc)
[-] Dynamically expanding/shrinking io buffers (instead of fixed size)
[-] Distributed framework using libspread1
    - The bot should be able to spread data in various forms across an ever-changing number of bot-links
[-] Perfect hash system for organizing triggers, embedded within dlist_t.
[-] On-disk swapping of data structures for re-loading after possible crashes or restarts
    - In-mem swapping already exists
[-] gmod_gfdpass: FD Passing daemon, which is able to pass raw sockets over a unix domain socket with credentials
[-] Raw tcp/ip/udp/icmp communication, ability to create/destroy rfc-compliant TCP connections using raw sockets.
[-] TCP connection resuming via gmod_gfdpass AND on-disk swapping of TCB/tcp-ip state info
    - Must be able to resume tcp connections after reboot based on saved state in on-disk swapped state structs.
[-] Migration of xdb & mongodb into a transparent db -> the_db, settable inside of /darqbot/conf or on the fly
    - the_db must be able to synchronize on-disk Berkley db data with networked mongodb data & vice versa
[-] Finish sink.c. Allows the bot to register "sinks" for which to monitor various types of data: debug info, network communication etc.
[-] Incorporating wordnet
[-] Porting my "textgimp" project to a module, capable of creating text patterns/objects as well as graphical representations




Version 2.0 Checklist:
---------------------'
[-] SPARC & Intel inline assembly "fns" hooks for various functions (libc)
[-] Reincorporate "everything as function pointers" via a script which can modify/unmodify the entire source in one go. Complete patching.
[-] Incorporation of various machine learning components.
[-] Console based video screen cap module.
[-] Ability to communicate on more mediums, FB,twitter,gmail,other forums,aim etc.









.,:::Quick Build Example:::,.
-----------------------------

to build:
 mkdir /darqbot
 mkdir /darqbot/.darqbot
 cp examples/*.conf /darqbot/.darqbot/
 cp examples/conf /darqbot
 make world
 make run

 now in the bot type: efnet on

root@life:/darqbot-dev# make world
make all
make[1]: Entering directory `/darqbot-dev'
gcc -Wall -O3 --std=c99 -D_GNU_SOURCE bot_wrapper.c -o ./bot_wrapper
gcc -I ./include -I ../include/ -I./mongodb-c-driver/src/ -Wall -O3 --std=c99 -D_GNU_SOURCE bot.c misc.c conf.c dlist.c bot_network.c module.c mongodb.c stat.c global.c environ.c tokenize.c protocol.c randchar.c matchchar.c bot_unix.c safe.c debug.c pmodule.c gmodule.c xmodule.c zmodule.c fns.c console.c files.c xdb.c timer.c unix.c bit.c relink.c swap_inmem.c fd.c pid.c control.c unique.c avl.c swap_disk.c chunks.c sim.c -ggdb -g -lbsd -levent -ldl -lrt -rdynamic -lguile-2.0 ./mongodb-c-driver/libbson.a ./mongodb-c-driver/libmongoc.a -lssl -ldb -lm -lmatheval -lutil -lz -lcrypto -lcrypt main.c -o ./bot
bot_unix.c: In function ‘bot_evhook_unix_read’:
bot_unix.c:325: warning: dereferencing type-punned pointer will break strict-aliasing rules
gcc -I ./include -I ../include/ -I./mongodb-c-driver/src/ -Wall -O3 --std=c99 -D_GNU_SOURCE bot.c misc.c conf.c dlist.c bot_network.c module.c mongodb.c stat.c global.c environ.c tokenize.c protocol.c randchar.c matchchar.c bot_unix.c safe.c debug.c pmodule.c gmodule.c xmodule.c zmodule.c fns.c console.c files.c xdb.c timer.c unix.c bit.c relink.c swap_inmem.c fd.c pid.c control.c unique.c avl.c swap_disk.c chunks.c sim.c -ggdb -g -lbsd -levent -ldl -lrt -rdynamic -lguile-2.0 ./mongodb-c-driver/libbson.a ./mongodb-c-driver/libmongoc.a -lssl -ldb -lm -lmatheval -lutil -lz -lcrypto -lcrypt main_rootd.c -o bot_rootd
bot_unix.c: In function ‘bot_evhook_unix_read’:
bot_unix.c:325: warning: dereferencing type-punned pointer will break strict-aliasing rules
make[1]: Leaving directory `/darqbot-dev'
cd mods && make -i 2>&1 && make install
make[1]: Entering directory `/darqbot-dev/mods'
mkdir /darqbot/.darqbot/mods &
mkdir /darqbot/.darqbot/pmods &
mkdir /darqbot/.darqbot/gmods &
mkdir: cannot create directory `/darqbot/.darqbot/mods': File exists
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared gmod_gexec.c -o gmod_gexec.so
mkdir: cannot create directory `/darqbot/.darqbot/pmods': File exists
mkdir: cannot create directory `/darqbot/.darqbot/gmods': File exists
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared gmod_gpty.c -o gmod_gpty.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared gmod_gssl.c -o gmod_gssl.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared gmod_gircpong.c -o gmod_gircpong.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared gmod_grelinkd.c -o gmod_grelinkd.so
gmod_grelinkd.c: In function ‘grelinkd_op_relink’:
gmod_grelinkd.c:631: warning: assignment from incompatible pointer type
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared gmod_grelinkc.c -o gmod_grelinkc.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared gmod_gdaemon.c -o gmod_gdaemon.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared gmod_gnull.c -o gmod_gnull.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared gmod_graw.c -o gmod_graw.so #...
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared gmod_gircd.c -o gmod_gircd.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared gmod_graw.c -o gmod_graw.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared gmod_grelay.c -o gmod_grelay.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared gmod_girc.c -o gmod_girc.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared pmod_parse2.c -o pmod_parse2.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_bin.c -o mod_bin.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_guile.c -I/usr/include/guile -lguile-2.0 -o mod_guile.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared -I ./mod_avl_files/ ./mod_avl_files/texilib.c mod_avl.c -o mod_avl.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_timer.c -o mod_timer.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_fnsasmx86.c -o mod_fnsasmx86.so
mod_fnsasmx86.c: Assembler messages:
mod_fnsasmx86.c:293: Warning: using `%cl' instead of `%ecx' due to `b' suffix
mod_fnsasmx86.c:307: Warning: using `%al' instead of `%eax' due to `b' suffix
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_geoip.c -lGeoIP -o mod_geoip.so
In file included from mod_geoip.h:31,
                 from mod_geoip.c:26:
/custom_builds/include/GeoIPCity.h:45: warning: declaration does not declare anything
mod_geoip.c: In function ‘geoip_change_string’:
mod_geoip.c:154: warning: assignment discards qualifiers from pointer target type
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_rf.c -o mod_rf.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared -lcurl -lxml2 -I /usr/include/libxml2/ mod_rss.c -o mod_rss.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_sort.c -o mod_sort.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_string.c -o mod_string.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_rand.c -o mod_rand.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_sep.c -o mod_sep.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_dumbgames.c -o mod_dumbgames.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_color.c -o mod_color.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_attrib.c -o mod_attrib.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_quote.c -o mod_quote.so #...
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_help.c -o mod_help.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_cat.c -o mod_cat.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_internals.c -o mod_internals.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared gmod_gexec.c -o gmod_gexec.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared gmod_gpty.c -o gmod_gpty.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared -lmatheval -lrecode mod_me.c -o mod_me.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_html.c -o mod_html.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_fork.c -o mod_fork.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_tail.c -o mod_tail.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared gmod_gssl.c -o gmod_gssl.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_moby.c -o mod_moby.so #...
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_relinkd.c -o mod_relinkd.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_cmds.c -o mod_cmds.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_rant.c -o mod_rant.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_bsdavltree.c -o mod_bsdavltree.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_stack.c -o mod_stack.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_exec.c -o mod_exec.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_map.c -o mod_map.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_relinkd.c -o mod_relinkd.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_alias.c -o ./mod_alias.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_bitvec.c -o mod_bitvec.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_print.c -o mod_print.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_chan.c -o mod_chan.so
mod_chan.c: In function ‘chan_list’:
mod_chan.c:704: warning: unused variable ‘buf’
mod_chan.c:703: warning: unused variable ‘dptr’
mod_chan.c:702: warning: unused variable ‘channel’
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_save.c -o mod_save.so
mod_save.c: In function ‘save_bot’:
mod_save.c:170: warning: unused variable ‘channel’
mod_save.c:169: warning: unused variable ‘nick’
mod_save.c:168: warning: unused variable ‘server’
mod_save.c:166: warning: unused variable ‘dptr’
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_fnsc.c -o mod_fnsc.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared ./libfortune/fortune/libfortune.o -lrecode -lbsd mod_fortune.c -o mod_fortune.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_quiz.c -o mod_quiz.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_console.c -o mod_console.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_sig.c -o mod_sig.so
#gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_fortune.c -o mod_fortune.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_echo.c -o mod_echo.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_caps.c -o mod_caps.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_rot13.c -o  mod_rot13.so
#gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_attrib.c -o mod_attrib.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_leet.c -o mod_leet.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_moby.c -o mod_moby.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_shake1.c -o mod_shake1.so
#gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_quote.c -o mod_quote.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared  -o mod_reverse.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -lbsd -lssl -shared mod_md5.c -o mod_md5.so
mod_md5.c: In function ‘md5_change_string’:
mod_md5.c:131: warning: implicit declaration of function ‘MD5Init’
mod_md5.c:133: warning: implicit declaration of function ‘MD5Data’
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_emot.c -o mod_emot.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_to.c -o mod_to.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_help.c -o mod_help.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared -lcurl mod_tinypaw.c -o mod_tinypaw.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared -lpcre mod_pcre.c -o mod_pcre.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared -lhunspell mod_hun.c -o mod_hun.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_nop.c -o mod_nop.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_auto_rejoin.c -o mod_auto_rejoin.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_whoami.c -o mod_whoami.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_pmme.c -o mod_pmme.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared -lcurl mod_tiny.c -o mod_tiny.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_var.c -o mod_var.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_howie.c -o mod_howie.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_sp.c -o mod_sp.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_alias.c -o ./mod_alias.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_rainbow.c -o mod_rainbow.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_packmulti.c -o mod_packmulti.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared -lcurl mod_smf.c -o mod_smf.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_identify.c -o mod_identify.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_sysvbanner.c -o mod_sysvbanner.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_char.c -o mod_char.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_mongotestfmt.c -o mod_mongotestfmt.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_festival.c -o mod_festival.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_backtrace.c -o mod_backtrace.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_vuln.c -o mod_vuln.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_uu.c -o mod_uu.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_stack.c -o mod_stack.so
gcc -I ./include/ -I ./mod_text/ -I ../include/ -I ../mongodb-c-driver/src/ -I /custom_builds/include -Wall --std=c99 -rdynamic -finstrument-functions -ggdb -g -levent -ldl -fPIC -lc -L /custom_builds/lib/ -shared mod_queue.c -o mod_queue.so
make[1]: Leaving directory `/darqbot-dev/mods'
make[1]: Entering directory `/darqbot-dev/mods'
mv -f mod*.so /darqbot/.darqbot/mods &
mv -f pmod*.so /darqbot/.darqbot/pmods &
mv -f gmod*.so /darqbot/.darqbot/gmods &
make[1]: Leaving directory `/darqbot-dev/mods'
root@life:/darqbot-dev# 

* now issue 'make run'
* then from console you can issue 'efnet on', 'local on', etc






A note on chrooting:
- certain modules need a lot of libs, so, you might have to mirror mount a few dirs if you want everything to work (such as guile etc):

mount -B /lib/ /darqbot/lib
mount -o remount,ro /darqbot/lib
#mount -B /usr/lib /darqbot/usr/lib/
#mount -o remount,ro /darqbot/usr/lib
#mount -B /usr/bin /darqbot/usr/bin
#mount -o remount,ro /darqbot/usr/bin
mount -B /usr /darqbot/usr/
mount -o remount,ro /darqbot/usr
mkdir /darqbot/tmp
chmod 777 /darqbot/tmp
chmod +t /darqbot/tmp
mount -B /dev /darqbot/dev
mount -o remount,ro /darqbot/dev
#mount -B /usr/local /darqbot/usr/local/
#mount -o remount,ro /darqbot/usr/local/
mount -B /proc /darqbot/proc
mount -o remount,ro /darqbot/proc



In order to Valgrind:
- Remove mod_guile from /databot/conf




.,:::Module Overview:::,.
-------------------------

gmod_gdaemon.c    : when run, daemonizes
gmod_gexec.c      : runs an executable
gmod_girc.c       : interacts with an irc server 
gmod_gircd.c      : acts as an ircd server
gmod_gircpong.c   : responds to PONG (primarily for gmod_grelinkd)
gmod_gnull.c      : example null gmod, does nothing
gmod_gpty.c       : creates a pty
gmod_graw.c       : can act in client or server mode, establishes connections or receives connections on TCP and/or UNIX DOM sockets
gmod_grelay.c     : relay module, used to pass control to a parser (parse2) or a specific module such as mod_sysvbanner
gmod_grelinkc.c   : relink client
gmod_grelinkd.c   : relink daemon
gmod_gssl.c       : forces SSL communication
mod_alias.c       : creates, deletes, or runs aliases. Used to save long expressions into short triggers, such as ^a ls /etc
mod_attrib.c      : performs attributes on "irc text". bold, underline, multi (force multi line output), 
mod_auto_rejoin.c : monitors lowest level i/o, rejoins if kicked
mod_avl.c         : gnu avl tree test code, prints pretty trees too with texitree.
mod_backtrace.c   : prints a current backtrace, pretty much the same every time.
mod_bin.c         : execute bins found in mods/mod_bin_files (ie: /darqbot/.darqbot/mods/mod_bin_files) 
mod_bitvec.c      : tests bit.c primitives, demonstrates bitwise operators
mod_bsdavltree.c  : tests bsd's tree.h primitives
mod_caps.c        : modifies capitalization: upper, lower, alternating char, alternating word
mod_cat.c         : reads, writes, or appends files found in mods/mod_cat_files (ie: /darqbot/.darqbot/mods/mod_cat_files)
mod_chan.c        : currently broken due to code changes: maintains lists of channels, users, etc
mod_char.c        : changes text in a variety of ways, including "randomly" via RASCII, RUPPER, RPUNCT etc..
mod_cmds.c        : returns current pipeline of commands instead of executing them
mod_color.c       : can modify foreground or background colors of text on irc
mod_console.c     : allows end users to control the bot console over irc
mod_dumbgames.c   : include dumb games (many dumb games in the future), such as dice/roulette.
mod_echo.c        : probably the most important module, needed to echo text that the bot generates. ^e = shorthand form.
mod_emot.c        : returns an emoticon based on a keyword
mod_exec.c        : executes a pipeline of commands
mod_festival.c    : currently broken: pipes data over speech to text (festival)
mod_fnsasmx86.c   : "fns" asm x86 functions, eventually will contain "libc" in intel asm.
mod_fnsc.c        : "fns C functions, eventually will contain custom "libc" in C
mod_fork.c        : causes the current pipeline to branch off into a fork child and continue executing
mod_fortune.c     : returns a unix fortune, same syntax as the command line version.
mod_geoip.c       : ip to location module
mod_guile.c       : embedded scheme-lisp interpreter, gnu guile.
mod_help.c        : help system, returns general help or a help message of a specific module
mod_howie.c       : embedded howie AIML bot
mod_html.c        : uses curl to establish connections & retrieve html urls
mod_hun.c         : hunspell spell correction
mod_identify.c    : identifies a user based on their hostname stored in mongodb
mod_internals.c   : returns info/sizes/lengths of the current state of internal buffers
mod_leet.c        : leet speech module
mod_map.c         : similar to scheme's (map ..), map something to each argument
mod_md5.c         : md5's current data in the output io buf
mod_me.c          : libmatheval example code, can solve equations & give derivative.
mod_moby.c        : transforms a word or words via moby thesaurus
mod_mongotestfmt.c: mongodb test module
mod_nop.c         : no operation
mod_packmulti.c   : packs buf data into fixed size sub-length strings
mod_pcre.c        : perl compatible regular expressions
mod_pmme.c        : causes final output to be pm'd to the end user who triggered it
mod_print.c       : can print data in various formats, currently only converts binary to & from
mod_queue.c       : enqueues or dequeues data
mod_quiz.c        : establishes quizzes per channel, maintains win/loss stats, great for 'flash cards'
mod_quote.c       : returns a quote from the quote db's
mod_rainbow.c     : rainbow colorizes irc text, can use custom color schemes
mod_rand.c        : returns a random word of text, random number, or random data
mod_rant.c        : saves text per user per chan, great for piping blocks of text to smf/html/cat/etc
mod_relinkd.c     : currently broken: interface to gmod_grelinkd
mod_rf.c          : based on a supplied list of functions, return a random function (deprecated, ^() replaces ^rf)
mod_rot13.c       : rot13's text
mod_rss.c         : manages RSS feeds, ability to display ONLY rss updates
mod_save.c        : currently broken: saves a running bot's current state into a conf file
mod_sep.c         : contains code for ^:, ^!, ^#
mod_shake1.c      : returns a random Shakespearean insult
mod_sig.c         : raises a signal (ie: ^sig 11)
mod_smf.c         : smf forum posting code, need to fix this for use with a list of smf forums
mod_sort.c        : ability to sort by char or word, forward/backward or randomly.
mod_sp.c          : test code for Stanford lexical parser
mod_stack.c       : push/pop data to a stack
mod_string.c      : a variety of fun string functions
mod_sysvbanner.c  : even more fun, output text via sysvbanner
mod_tail.c        : ability for users to tail files, such as /var/log/.....
mod_timer.c       : set or delete timer's per channel that run pipelines of commands.
mod_tiny.c        : custom tinyurl module (uses adarq.org tinyurl webcode)
mod_tinypaw.c     : custom tinyurl module using keywords (uses adarq.org tinypaw webcode)
mod_to.c          : currently broken: pipe output to another bot (inter-bot communication)
mod_uu.c          : base64 encoding, allows for transferring of raw data
mod_var.c         : modify important bot variables such as flood limits, flood sleep times, buffer sizes, etc
mod_vuln.c        : currently broken: irony.
mod_whoami.c      : tells you who you are
pmod_parse2.c     : powerful parser which provides the interface to using the various mod_*'s












.,:::BRIEF OVERVIEW OF EXAMPLE CONF FILES:::,.
----------------------------------------------

examples/conf: this is the global conf file, it should be in /darqbot/conf

examples/*.conf: these are conf files for various services/daemons the bot will spawn.




/darqbot/conf:
 - By removing/adding modules to +modules, +pmodules, +gmodules, you can control what the bot has access too.


efnet.conf - connect to efnet
ircdcli.conf - connect to local ircd server
localder.conf - connect to local ircd using new trig prefix of '!'
localssl-relink.conf - connect to local SSL'd ircd using relink
relinkd.conf - spawn the relink daemon
efnet-relink.conf - connect to EfNet through the relink daemon
ircdserver-nod.conf - spawn a local ircd server without daemonizing
local-relink.conf - connect to local ircd through the relink daemon
parse2-server.conf - spawn a parser server
relinkd-nod.conf - spawn relinkd without daemonizing
exec-server.conf - spawn an exec server which acts as a shell
local.conf - connect to local ircd
localssl.conf - connect to local ircd over ssl
pipe-tcptounix.conf - listen on a tcp socket and then connect to local ircd via unix domain, tcp <-pipe-> unix






.,:::BRIEF DEMO OF EACH MODULE:::,.
-----------------------------------

Very quick overview. Before we get started, 4 very important triggers:


mod_sep.c: ^: ^! ^# ^()
-----------------------

^: <- This trigger protects all of the data prior to it's usage:

< kx> ^e hi
< dbot> hi
< kx> ^e hi |^md5
< dbot> 63346d9b3c29ca03e984693c7200f3b8
< kx> ^e hi |^: |^md5                       <-- protected
< dbot> hi



^! <- Crushes all ^: delimiters

< kx> ^e hi |^: |^md5                       <-- protected
< dbot> hi
< kx> ^e hi |^: |^! |^md5                   <-- crush
< dbot> 63346d9b3c29ca03e984693c7200f3b8



^# <-- sets or unsets a comment

< kx> ^e hi |^# |^md5 |^bold|^blah|^osodksdkogsd inside the comment |^#|end of comment
< dbot> hi end of comment



^() <-- allows nesting and filling of text anywhere in the pipeline

< kx> ^e ^(^caps(^(low)) AAA)               <-- becomes ^caps(low) AAA
< dbot> aaa
< kx> ^e ^(^caps(^(low)) AAA) |^^(^e md5)   <-- becomes ^caps(low) AAA |^md5
< dbot> 47bce5c74f589f4867dbd57e9ca9f808



Ok now on to the various modules:
---------------------------------

mod_echo.c: ^e, ^echo, ^say
---------------------------
- tells the bot to echo the text

< kx> hi|^e
< dbot> hi
< kx> ^e hi
< dbot> hi
< kx> ^echo hi|^e|hello
< dbot> hihello
< kx> ^echo hi|^e| hello
< dbot> hi hello



mod_alias.c: ^a, ^alias, ^alias(get:getmore:add:del:run)
--------------------------------------------------------
- get, add, delete, and run an alias


< kx> ^a(get) ls|^e
< dbot> |^e|^guile (define p (opendir "$$2")) (define x (lambda () (let ((buf (readdir p))) (display buf) (cond ((eof-object?
        buf) 'end) (else (newline) (x)) )))) (x)
< kx> ^a(getmore) ls|^e
< dbot> warm-n9!~ng@c-98-211-222-243.hsd1.fl.comcast.net!#darqbot
< kx> ^a(run) ls /etc|^e
< dbot> . hosts.deny motd hosts alternatives passwd hosts.allow nsswitch.conf .. resolv.conf group #<eof>
< kx> ^a ls /etc|^e
< dbot> . hosts.deny motd hosts alternatives passwd hosts.allow nsswitch.conf .. resolv.conf group #<eof>


nesting example:

< kx> ^a(add) test_nesting3 \|^echo loop -> \|^var(nesting_cur)\|^: ... \|^a test_nesting3
< kx> ^a test_nesting3|^e
< dbot> loop -> 2... loop -> 3... loop -> 4...


adding an alias, if it contains pipes, escape them:

< kx> ^a(add) suphiro ^e sup hey whatsup wassup hi\|^rand\| \|^:\|hirogen hiro tosser 2cf TOUSEEF rahman 2ceef h1r0g3n
      PUMGAGYM\|^rand
< kx> ^a(get) suphiro|^e
< dbot> ^e sup hey whatup wassup hi|^rand| |^:|hirogen hiro tosser 2cf TOUSEEF rahman 2ceeef h1r0g3n PUMAGYM|^rand
< kx> ^a suphiro
< dbot> wassup tosser




mod_attrib.c: ^bold, ^underline, ^multi
---------------------------------------
- sets output as bold, underline, and/or multi
- can't demo bold/underline in this README.txt, so I'll demo ^multi:

< kx> ^e multi|^sysvbanner
< dbot>                                                                                   #    #  #    #  #        #####     #
        ##  ##  #    #  #          #       #    # ## #  #    #  #          #       #    #    #  #    #  #          #       #
        #    #  #    #  #          #       #    #    #   ####   ######     #       #
< kx> not multi... ^^^
< kx> ^e multi|^sysvbanner|^multi
< dbot>
< dbot>
< dbot>   #    #  #    #  #        #####     #
< dbot>   ##  ##  #    #  #          #       #
< dbot>   # ## #  #    #  #          #       #
< dbot>   #    #  #    #  #          #       #
< dbot>   #    #  #    #  #          #       #
< dbot>   #    #   ####   ######     #       #
< dbot>



mod_auto_rejoin.c: works by itself, via auto_rejoin->input
----------------------------------------------------------



mod_avl.c: ^avl, ^avl(ins:del:clear:size:tree), ^avl_ins, ^avl_del, ^avl_clear, ^avl_size, ^avl_tree
----------------------------------------------------------------------------------------------------
- inserts/deletes/lists/treeify's avl tree data

< kx> ^avl(ins) hi abe lincoln george bush president obama
< kx> ^avl(list)|^e
< dbot> hi(bush(abe,george),obama(lincoln,president))
< kx> ^avl(del) bush
< kx> ^avl(list)|^e
< dbot> hi(george(abe),obama(lincoln,president))
< kx> ^avl(tree)|^e|^multi
< dbot>                              hi
< dbot>                         __.-'  `--..__
< dbot>                        george         obama
< dbot>                      _'         __..-'     \
< dbot>                     abe        lincoln      president



mod_backtrace.c: ^backtrace, ^backtrace(num)
--------------------------------------------
- prints a backtrace

< kx> ^e|^backtrace(1)
< dbot> /.darqbot///mods/mod_backtrace.so(backtrace_change_string+0x163) [0x413e00a7]
< kx> ^var(maxflooding) 20
< kx> ^e|^backtrace(20)|^multi
< dbot> /.darqbot///mods/mod_backtrace.so(backtrace_change_string+0x163) [0x413e00a7]
< dbot> /.darqbot///mods/mod_backtrace.so(backtrace_run+0x189) [0x413dfeda]
< dbot> /.darqbot//pmods/pmod_parse2.so(parse2_handle_text_pipes+0x4fe) [0x41a010cc]
< dbot> /.darqbot//pmods/pmod_parse2.so(parse2_build_text_list_run+0x3c8) [0x41a0198e]
< dbot> /.darqbot//pmods/pmod_parse2.so(parse2_handle_text+0xeb) [0x41a00b85]
< dbot> /.darqbot//pmods/pmod_parse2.so(parse2_run+0x1e6) [0x41a00a79]
< dbot> /.darqbot//gmods/gmod_grelay.so(grelay_input+0x457) [0x41a7276e]
< dbot> ./bot_main_process(gmodule_up+0x1c9) [0x8067e69]
< dbot> /.darqbot//gmods/gmod_girc.so(girc_input+0x46c) [0x41a4d77c]
< dbot> ./bot_main_process(gmodule_up+0x1c9) [0x8067e69]
< dbot> /.darqbot//gmods/gmod_graw.so(graw_evhook_link+0x19d) [0x41a28501]
< dbot> /usr/lib/libevent-1.4.so.2(event_base_loop+0x3c4) [0x4003eee4]
< dbot> /usr/lib/libevent-1.4.so.2(event_loop+0x29) [0x4003f039]
< dbot> ./bot_main_process(main+0xe8) [0x807aa48]
< dbot> /lib/i686/cmov/libc.so.6(__libc_start_main+0xe6) [0x40539ca6]
< dbot> ./bot_main_process() [0x8058921]



mod_bin.c: ^bin, ^bin(program,arguments)
----------------------------------------
- executes a binary found in $CONFDIR/mods/mod_bin_files

< kx> ^bin(pic2ascii, http://i.imgur.com/....jpg)|^e|^multi
< dbot> 8XXS@8X8XX@88S ;;;;;t          ;:;;;:
< dbot> 8;X S @XtSS; 8%%8.  %:8X8%8X8.t8%..%tt;.
< dbot> 8 88tX8888S 8@8t8;  %8S8X88X88%8S888t8;.
< dbot> 8 8 8 ;:XS%t;8%88@8X8 :;;;;;:8: 8tStt%;.
< dbot> S8X@SS%%%.S8XX S88S8 ;S. ..8 ;8.8  8tS8:
< dbot> X; %% SS@8:S@t88X8888Xt .  8%@S888X@XX8.
< dbot> S:@@X%X:.;:8 @ :t8.X:%tt8;8;t88S8S8X@X8
< dbot> :;::;;%:::;t;:8S8888S888X;S8@8%t8:8;8;88
< dbot> :SXS%8XSSXXS%88888888X8S88@8 S :;t888888
< dbot> :S.S8S%888S@%@8X88@8S888@8@:;:8S%XSS%XSX
< dbot> ;;S;888%@@88%@  8 :S 8 S  X;X8@:88888t::
< dbot> %8@S@88X888SS. 8 8  ;8t . @: S88XS%8S%:;
< dbot> SX%X88@tXSS%88 ;8   %@:;: 8:X:t:;t88%8tS
< dbot> @8888888;%t;;X8X8888@8t88S8%X8X@@@:%t8X.
< dbot> @8888888888888888888888888@8888888888@88

for fun, pic2ascii is:

#!/bin/sh
if test $# -eq 0;
 then
        echo failed
        exit 1
 fi
URL=$1
shift
OPTS=$@
if [ -z "$OPTS" ];
 then
  OPTS="-W 40"
 fi
FILE=`date +%N`
wget --quiet -O /tmp/$FILE $URL
img2txt -f irc $OPTS /tmp/$FILE
rm -f /tmp/$FILE







13:13 < kx> ^bin(pic2ascii,http://sphotos.xx.fbcdn.net/hphotos-snc6/285593_10150258587891778_5046164_n.jpg -W 20)|^e|^multi
13:13 < uas>     .;.@%8
13:13 < uas>   .;. XX%@%8@%;::;t%
13:13 < uas>   :: XX :@ 8@t@tt: ;
13:13 < uas>  %8 .8    .t8%tt  .
13:13 < uas> :S @t   ;;:t@@t t:::
13:13 < uas> ;S:. ;%.X8: X;t;;SSS
13:13 < uas>  S@:X..;   .;8: S%%@
13:13 < uas>  .XXt.8% .  S8 ;8@S;
13:13 < uas>    X;SX@.    X:%SSXt
13:13 < uas>     8@...:tSX: :;8 %
13:13 < uas>  . .%:@8tS. :t8SX@;
13:13 < uas>     ;@@@;       ;:..
13:13 < uas>   .   .   .   .





mod_bitvec.c: ^bv, ^bitvec, ^bv(init:fini:set:clr:or:xor:and:not:eq:zero:copy)
------------------------------------------------------------------------------
- demonstrate bitwise operations

< kx> ^bv(init) 8|^bv(info)|^e|^multi
< dbot> x: 0000000000000000000000000000000000000000000000000000000000000000
< dbot> y: 0000000000000000000000000000000000000000000000000000000000000000
< dbot> z: 0000000000000000000000000000000000000000000000000000000000000000
< kx> ^bv(eq) x 582301091|^bv(eq) y 012941209|^bv(info)|^e|^multi
< dbot> x: 1100010110101100101011010100010000000000000000000000000000000000
< dbot> y: 1001100111101110101000110000000000000000000000000000000000000000
< dbot> z: 0000000000000000000000000000000000000000000000000000000000000000
< kx> ^bv(set) x 30|^bv(set) y 40|^e|^multi
< kx> ^bv(set) x 30|^bv(set) y 40|^bv(info)^e|^multi
< kx> ^bv(set) x 30|^bv(set) y 40|^bv(info)|^e|^multi
< dbot> x: 1100010110101100101011010100011000000000000000000000000000000000
< dbot> y: 1001100111101110101000110000000000000000100000000000000000000000
< dbot> z: 0000000000000000000000000000000000000000000000000000000000000000
< kx> ^bv(or)|^bv(info)|^e|^multi
< dbot> x: 1100010110101100101011010100011000000000000000000000000000000000
< dbot> y: 1001100111101110101000110000000000000000100000000000000000000000
< dbot> z: 1101110111101110101011110100011000000000100000000000000000000000
< kx> ^bv(and)|^bv(info)|^e|^multi
< dbot> x: 1100010110101100101011010100011000000000000000000000000000000000
< dbot> y: 1001100111101110101000110000000000000000100000000000000000000000
< dbot> z: 1000000110101100101000010000000000000000000000000000000000000000
< kx> ^bv(xor)|^bv(info)|^e|^multi
< dbot> x: 1100010110101100101011010100011000000000000000000000000000000000
< dbot> y: 1001100111101110101000110000000000000000100000000000000000000000
< dbot> z: 0101110001000010000011100100011000000000100000000000000000000000
< kx> ^bv(not) x|^bv(info)|^e|^multi
< dbot> x: 0011101001010011010100101011100111111111111111111111111111111111
< dbot> y: 1001100111101110101000110000000000000000100000000000000000000000
< dbot> z: 0101110001000010000011100100011000000000100000000000000000000000
< kx> ^bv(not) y|^bv(info)|^e|^multi
< dbot> x: 0011101001010011010100101011100111111111111111111111111111111111
< dbot> y: 0110011000010001010111001111111111111111011111111111111111111111
< dbot> z: 0101110001000010000011100100011000000000100000000000000000000000
< kx> ^bv(zero) x|^bv(info)|^e|^multi
< dbot> x: 0000000000000000000000000000000000000000000000000000000000000000
< dbot> y: 0110011000010001010111001111111111111111011111111111111111111111
< dbot> z: 0101110001000010000011100100011000000000100000000000000000000000
< kx> ^bv(zero) y|^bv(info)|^e|^multi
< dbot> x: 0000000000000000000000000000000000000000000000000000000000000000
< dbot> y: 0000000000000000000000000000000000000000000000000000000000000000
< dbot> z: 0101110001000010000011100100011000000000100000000000000000000000
< kx> ^bv(zero) z|^bv(info)|^e|^multi
< dbot> x: 0000000000000000000000000000000000000000000000000000000000000000
< dbot> y: 0000000000000000000000000000000000000000000000000000000000000000
< dbot> z: 0000000000000000000000000000000000000000000000000000000000000000




mod_bsdavltree.c: ^bavlt, ^bavlt({str,num},{add,del,find,clear,info,depth,preorder,inorder,postorder,forward,reverse})
----------------------------------------------------------------------------------------------------------------------
- bsd tree.h macros test

< kx> ^e|^bavlt(str,add) ^(^quote)|^e
< dbot> ^e,_a,a,about,am,amour_^e,and,anymore,average,civilise,confident,deserve,do,don,doubt,feel,fight,fornicate,fucking,going,happens,I,interviews,It,know,l,m,me,no,normally,nut,person,s,shouldn,So,t,talk,the,them,think,thinks,this,to,unless,want,whatever,win,winning,with,women,you,




mod_caps.c: ^caps, ^caps(low:altw:altc:randw:randc)
---------------------------------------------------
- mess with upper/lower case characters of strings

< kx> ^quote(tyson)|^caps|^e
< dbot> PEOPLE LOVE YOU WHEN YOU'RE SUCCESSFUL, BUT IF YOU'RE NOT, WHO REALLY CARES ABOUT YOU?
< kx> ^quote(tyson)|^caps(altc)|^e
< dbot> YoU'Re SmArT tOo LaTe AnD oLd ToO sOoN.
< kx> ^quote(tyson)|^caps(altw)|^e
< dbot> ONE morning I woke UP and FOUND my FAVORITE pigeon, JULIUS, had DIED i WAS devastated AND was GONNA use HIS crate AS my
        STICKBALL bat TO honor HIM. i LEFT the CRATE on MY stoop AND went IN to GET something AND i RETURNED to SEE the
        SANITATION man PUT the CRATE into THE crusher. I rushed HIM and CAUGHT him FLUSH on THE temple WITH a TITANIC right
        HAND he WAS out COLD, convulsing ON the FLOOR like AN infantile RETARD.
< kx> ^quote(tyson)|^caps(randw)|^e
< dbot> SOMETIMES YOU GUYS have no PRIDE, SO no MATTER what I SAY, YOU guys ... IT doesn't AFFECT YOU because you don't care
        about NOTHING but MONEY. so EVERY now and then i KICK your F**KING ASS and stomp ON YOU AND PUT SOME kind OF pain and
        inflict some OF THE pain on you because YOU deserve TO FEEL the PAIN THAT I FEEL.




mod_cat.c: ^cat, ^cat(>:>>:<)
-----------------------------
- write, read, or append to a file in $CONFDIR/mod_cat_files/

< kx> ^rant(push)
< kx> hello
< kx> tex
< kx> t
< kx> ok
< kx> ^rant(pop)|^cat(>) hello|^e
< kx> ^e|^cat(<) hello
< dbot>  hello tex t ok
< kx> ^e|^quote|^md5|^cat(>>) hello
< kx> ^e|^cat(<) hello
< dbot>  hello tex t ok 84f3c6cc441d359c97f507203aa45773




mod_chan.c: ^chan <-- will fix soon
-----------------------------------



mod_char.c: ^char, ^char(RASCII:RALPHA:RALNUM:RLOWER:RUPPER:RDIGIT:RPUNCT:RBINARY:RSAME:RSPACE)
-----------------------------------------------------------------------------------------------
- modify chars

< kx> ^e hello|^md5|^sysvbanner|^char(RPUNCT)|^multi
< dbot>
< dbot>  %`%-?|[         &          {    '         \}{    \`*#'
< dbot>  ,        :)`}]  &    )    (:    ?    )   .   *  \     ]    [`    (@&/{    `//%
< dbot>  %        !    ^ \    {   > `    &    (  + ,   |       ;   ]  $   |    !  $    ,
< dbot>   =:{]{   ]    ( ~(\>^]\    :    )@,/:=< -  /  "  (%~!)   :    .  <>\`(   /
< dbot>        =  [    ?      '     &         ;  $   ; { *        !^=[*]  /    ,  <
< dbot>  <     ]  &    *      ~     %         (   +   \  $        ]    :  %    .  #    ;
< dbot>   $#)~;   >^_}|       =   /%],~       #    :)?   ;]=>%')  '    (  =!`/`    (}'_
< dbot>
< kx> ^e hello|^md5|^sysvbanner|^char(RBINARY)|^multi
< dbot>
< dbot>  0100010         1          0    0         100    01101
< dbot>  1        01101  0    0    11    0    1   1   0  1     0    10    01001    1001
< dbot>  1        0    1 1    1   1 1    0    1  1 0   1       0   1  0   1    1  0    0
< dbot>   01011   0    1 1101011    1    0011111 0  0  0  11101   0    1  01110   0
< dbot>        0  0    0      1     0         1  1   0 0 0        010001  0    0  1
< dbot>  0     1  0    1      0     0         1   1   1  0        0    1  1    1  0    0
< dbot>   00000   10000       0   01011       1    101   0011001  1    1  01001    1011
< dbot>



mod_cmds.c: ^cmds
-----------------
- print trigger pipeline, don't execute it

< kx> ^e hello|^md5|^sysvbanner|^char(RBINARY)|^multi|^cmds
< dbot> ^e hello|^md5|^sysvbanner|^char(RBINARY)|^multi|



mod_color.c: ^fgcolor, ^bgcolor:
--------------------------------
- changes the foreground or background color of text

small examples:

^fgcolor(orange) orange | ^fgcolor(cyan) ice | ^caps | ... | ^fgcolor(pink) mijo |^echo
^bgcolor(orange) ORANGE | ^bgcolor(cyan) ICE||^bgcolor(pink) mijo|^echo
^fgcolor(red) U |^fgcolor(white) S |^fgcolor(blue) A |^echo



mod_console.c: ^console
-----------------------
- run commands on console


< kx> ^console help|^e|^multi
< dbot> darqbot console help: <command> args..
< dbot>  modload, modunload, modreload, modlist, modlock, modunlock ::: ...
< dbot>  pmodload, pmodunload, pmodreload, pmodlist, pmodlock, pmodunlock ::: ...
< dbot>  gmodload, gmodunload, gmodreload, gmodlist, gmodlock, gmodunlock ::: ...
< dbot>  zmodload, zmodunload, zmodreload, zmodlist, zmodlock, zmodunlock ::: ...
< dbot>  modiolist, gmodiolist, zmodiolist ::: ...
< dbot>  modtimerlist, gmodtimerlist, zmodtimerlist ::: ...
< dbot>  testmem ::: ...
< dbot>  testfd ::: ...
< dbot>  close ::: ...
< dbot>  trace ::: ...
< dbot>  debug ::: ...
< dbot>  pid ::: ...
< dbot>  print_all ::: ...
< dbot>  info ::: ...
< dbot>  exiti, quit ::: ...
< dbot>  off ::: ...
< dbot>  on ::: ...
< dbot>  help ::: ...
< dbot> darqbot console help: <tag> <command> args..
< dbot>  info ::: ...
< dbot>  raw <text> ::: sends <text> directly over fd_link
< dbot>  swapiminfo ::: ...
< dbot>  on ::: ...
< dbot>  off ::: ...
< dbot>  reload ::: ...
< dbot>  kill ::: ...
< dbot>  mute ::: ...
< dbot>  unmute ::: ...
< dbot>  trace ::: ...
< dbot>  debug ::: ...
< dbot>  break ::: ...

< kx> ^console info ircdcli-dbot|^e|^multi
< dbot> printing out config info for bot=ircdcli-dbot:
< dbot>  ID: 463422989
< dbot>  status: on
< dbot>  trace: 0 , debug: 0 => gd->trace=2 , gd->debug=1
< dbot>  servers:
< dbot>  nicks:
< dbot>  channels:
< dbot>  logfile=
< dbot>  vars: multi=0, usleep=0, maxbufsz=10024, bufsz=10024, maxflood=1000, maxnesting=20, nesting=20, nesting_cur=0,
        allowpm=0
< dbot>  subs: 1
< dbot>  gmodules: 3
< dbot>   ^graw(client...host=localhost:7000...host=127.0.0.1:7000...proto=tcp...other_options)
< dbot>   ^girc(nick=dbot:dbot:dbot:dbot:dbot...channel=#jumping,#darqbot,#bosshogs...other_stuff)
< dbot>   ^grelay(p,^parse2)






mod_dumbgames.c: ^dice, ^roulette
---------------------------------
- a module for dumb games

< kx> ^dice|^e
< dbot> [5] [5]
< kx> ^dice|^e
< dbot> [6] [6]
< kx> ^dice|^e
< dbot> [5] [4]
< kx> 1 1
< kx> ^dice|^e
< dbot> [5] [2]
< kx> ^dice|^e
< dbot> [6] [1]





mod_emot.c: ^emot, ^emot(keyword)
---------------------------------
- print emoticon based on keyword

< kx> ^emot spider|^e
< dbot> :-D*
< kx> ^emot spider|^e
< dbot> /\(oo)/\

eh.




mod_exec.c: ^exec
-----------------
- execs pipeline text


 
^exec often times works with ^map:

< kx> ^e|^map(normal, ,PIPE,^leet(2,0)) a b c "abcdefghijk k"
< dbot> ^leet(2,0) a |^leet(2,0) b |^leet(2,0) c |^leet(2,0) "abcdefghijk |^leet(2,0) k"|
< kx> ^e|^map(normal, ,PIPE,^leet(2,0)) a b c "abcdefghijk k"|^exec
< dbot> 4 6 c "46cd3fgh1jk k"


here's a nice example, my ^a rssupdate alias:

< kx> ^a(get) rssupdate|^e
< dbot> ^rss(info)|^map(normal, ,PIPE,^rss(update,nofork))|^e|^exec|^multi|^e


example for when trigger prefix changes and doesn't matchup with alias syntax:

07:51 < ng> !a(get) suphiro|!subchar(^!)|!e|^exec
07:51 < der> !e sup hey whatup wassup hi|!rand| |!:|hirogen hiro tosser 2cf TOUSEEF rahman 2ceeef h1r0g3n PUMAGYM|!rand^exec
07:51 < ng> !a(get) suphiro|!subchar(^!)|!e|!exec
07:51 < der> whatup PUMAGYM





mod_festival.c: ^festival
-------------------------
- unfinished



mod_fnsc.c: ^fnsc, ^fnsc(test)
------------------------------
- ...

^fnsc(test)



mod_fnsasmx86.c: ^fnsasmx86, ^fnsasmx86(test)
---------------------------------------------
- ...

^fnsasmx86(test)



mod_fork.c: ^fork
-----------------
- fork's .. everything left in the pipeline occurs in a fork'd environment

< kx> ^internals(getpid)|^e
< dbot> getpid=12815
< kx> ^fork|^internals(getpid)|^e
< dbot> getpid=12824                     <-- child
< kx> ^internals(getpid)|^e
< dbot> getpid=12815




mod_fortune.c: ^fortune, ^fortune(getopt_style_arguments)
---------------------------------------------------------
- displays a unix-style fortune.. this is built with a custom fortune modded program turned into a lib
- you could easily just put a shell script or fortune itself into mod_bin_files

< kx> ^fortune|^e|^multi
< dbot> "As soon as you are willing to discard observational data because it conflicts
< dbot>  with religion, you are giving up any hope of ever really understanding the
< dbot>  universe. As soon as you pick religion as the touchstone of reality, then we
< dbot>  have to start discussing how one can demonstrate the correctness of one
< dbot>  religion over another when different *religions* disagree."
< dbot>               --Wilson Heydt (whheydt@PacBell.COM)
< dbot>    "The answer is simple: kill the heretics.  History shows us that
< dbot>     this is the actual solution that competing religions apply -- trial
< dbot>     by combat or trial by ordeal. God is the final arbiter. What a sad
< dbot>     waste of human potential it has proven to be."
< dbot>              [Paul Hager (hagerp@iuvax.cs.indiana.edu)]
< kx> ^fortune(100% computers)|^e|^multi
< dbot> Bell Labs Unix -- Reach out and grep someone.

hah.





mod_geoip.c: ^geoip, ^geoip <host>
----------------------------------
- returns ip location info

some random person on irc:

< kx> ^geoip CPE00179a9c9a03-CM001ac3116fda.cpe.net.cable.rogers.com|^e
< dbot> CPE00179a9c9a03-CM001ac3116fda.cpe.net.cable.rogers.com:N/A:CA:Canada:ON:Ontario:Ottawa:N/A:0:45.416698:-75.699997:0:0:America/Rainy_River:0.0.0.0:0.255.255.255




mod_guile.c: ^guile, ^guile (lisp..)
------------------------------------
- embedded guile lisp parser, my fav

since scheme/lisp syntaxes can get quite long, you can easily store them in an alias:

< dbot> |^e|^guile (define p (opendir "$$2")) (define x (lambda () (let ((buf (readdir p))) (display buf) (cond ((eof-object?
        buf) 'end) (else (newline) (x)) )))) (x)
< kx> ^a(get) cat|^e
< dbot> |^e|^guile (define (read-string char-set input-port) (let ((char (peek-char input-port))) (if (eof-object? char) char
        (list->string (let loop ((char char)) (if (or (eof-object? char)) '() (begin (read-char input-port) (cons char (loop
        (peek-char input-port)))))))))) (define p (open-input-file "$$2")) (display (read-string 's p)) |^multi
< kx> ^guile (display 1)|^e
< dbot> 1
< kx> ^e|^guile (display (cons 'a '(b c))) (display (map + '(1 2 3))) (display (let ((x 2)) (+ x 3))) (define double (lambda (f
      x ) (f x x ))) (display (double + 10)) (display (sort '(1 3 2) <))
< dbot> (a b c)(1 2 3)520(1 2 3)

very versatile..



turning a channel into a lisp parser:
-------------------------------------
< kx> ^nop(ina) \|^e\|^guile
< kx> (display 1)
< dbot> 1
< kx> (display (+ 1 3 9))
< dbot> 13




mod_howie.c: ^howie, ^howie(on:off)
-----------------------------------
- embedded howie AIML bot

example of turning the chat into howie-chat

< kx> ^nop(ina) \|^e\|^howie
< kx> hi howie
< dbot> [./bot_main_process](12815) debug: -> howie_run_on: about to execve /usr/bin/python kx: Hi there .
< kx> oops
< dbot> kx: Did you make a mistake.
< kx> yes i did actually
< dbot> kx: I understand. When?
< kx> just now
< dbot> kx: Right now? It's only just begun.
< kx> it has begun
< dbot> kx: "It" being what?
< kx> yo
< dbot> kx: Can I help you?
< kx> no
< dbot> kx: Is there anything you want to tell me?
< kx> nope
< dbot> kx: Well let me know if you think of something, but your mind seems made up.
< kx> howie bot is pretty smart
< kx> aren't you howie bot
< dbot> kx: I don't know whether or not I am howie bot.   I am an artificial linguistic entity.
< kx> who programmed you
< dbot> kx: I was created by Your Name.
< kx> forgot to fill that out huh
< dbot> kx: Yup.
< kx> ridiculous
< kx> ^nop(call)




mod_html.c: ^html, ^html(options..)
- raw html module

issuing a GET:
< kx> ^html www.adarq.org/index.html|^e|^multi
< dbot> HTTP/1.1 200 OK
< dbot> Date: Wed, 04 Jul 2012 08:57:34 GMT
< dbot> Server: Apache/2.2.17 (Ubuntu)
< dbot> Last-Modified: Sat, 08 Oct 2011 04:58:42 GMT
< dbot> ETag: "1b11b8-48-4aec26965a480"
< dbot> Accept-Ranges: bytes
< dbot> Content-Length: 72
< dbot> Vary: Accept-Encoding
< dbot> Content-Type: text/html
< dbot> X-Pad: avoid browser bug
< dbot>
< dbot> <meta HTTP-EQUIV="REFRESH" content="0; url=http://www.adarq.org/forum">


issuing a GET through a socks5 proxy:
12:38 < ng> ^html(-P localhost:9050 -s) http://www.google.com|^e|^multi
12:39 < uas> HTTP/1.1 302 Found
12:39 < uas> Location: http://www.google.gr/
12:39 < uas> Cache-Control: private
12:39 < uas> Content-Type: text/html; charset=UTF-8
12:39 < uas> Set-Cookie: expires=; expires=Mon, 01-Jan-1990 00:00:00 GMT; path=/; domain=www.google.com
12:39 < uas> Set-Cookie: path=; expires=Mon, 01-Jan-1990 00:00:00 GMT; path=/; domain=www.google.com
12:39 < uas> Set-Cookie: domain=; expires=Mon, 01-Jan-1990 00:00:00 GMT; path=/; domain=www.google.com
12:39 < uas> Set-Cookie: expires=; expires=Mon, 01-Jan-1990 00:00:00 GMT; path=/; domain=.www.google.com
12:39 < uas> Set-Cookie: path=; expires=Mon, 01-Jan-1990 00:00:00 GMT; path=/; domain=.www.google.com
12:39 < uas> Set-Cookie: domain=; expires=Mon, 01-Jan-1990 00:00:00 GMT; path=/; domain=.www.google.com





mod_hun.c: ^hun
---------------
- spell-check

< kx> ^hun hi i liek to mispell it|^e
< dbot> hi i liek={like,lie,leek,lies,lien,link,lier,lick,lied,lieu,lief,lie k} to mispell={ispell,misspell,m ispell,mi
        spell,mi-spell,Ispell,misperceive,misplace,misplay,misapply,misapplier} it

< kx> ^hun spelcheck|^e
< dbot> spelcheck={squelcher,speechless,spelunker,spelunking,sepulcher,splotchy}





mod_identify.c: ^identify, ^identify(get,add,del,getmore)
---------------------------------------------------------
- identify you from the mongodb

< kx> ^identify|^e
< dbot> ADARQ

< kx> ^identify(get) adarq|^e
< dbot>  _id : 7 ... 4f967414d42b57d0b903f0d3  key : 2 ... ADARQ  value : 2 ... /.*!.*@.*local.*/  comment : 2 ...
        z!~x@localhost!#jumping

^identify(add) tag /regex/




mod_internals.c: ^internals, ^internals(all,in_sz,out_sz,in_strlen,out_strlen,in_data,trig_prefix,getpid)
---------------------------------------------------------------------------------------------------------
- returns internal io/bot info

< kx> ^internals(all)|^e
< dbot> in_sz=56 out_sz=0 in_strlen=0 out_strlen=0 in_data= trig_prefix= getpid=12815




mod_leet.c: ^leet, ^leet(0-3:0-1)
---------------------------------
- leetify's text

< kx> hi my name is darqbot and i love to irc in such cool ways|^e|^leet
< dbot> hi my n4me is d4rq6ot and i l0ve t0 irc in such co01 w4ys
< kx> hi my name is darqbot and i love to irc in such cool ways|^e|^leet(1)
< dbot> hi my n4me i5 darq6ot and i l0ve t0 irc in such c0o1 way5
< kx> hi my name is darqbot and i love to irc in such cool ways|^e|^leet(2)
< dbot> h1 my n4m3 15 d4rq607 4nd 1 10v3 70 1rc 1n 5uch c001 w4y5
< kx> hi my name is darqbot and i love to irc in such cool ways|^e|^leet(3)
< dbot> |-|1 |\/|y n/\/\/\3 15 |>4|2q]307 /-\|/||) 1 |_0v3 +0 !/2c !|/| 5(_)c}{ c()01 \\'/\`(5
< kx> hi my name is darqbot and i love to irc in such cool ways|^e|^leet(0,1)
< dbot> aycheye emwhy enayeeme eyees deeayearecuebeeohtea ayeendee eye elohveee teaoh eyearesee eyeen esyooseaaych seaohohel
        dubyewayewhyes




mod_map.c: ^map, ^map(noral,DELIM-FOR-PARSER,REPLACEMENT-DELIM,text-toprepend)
--------------------------------------
- maps something to each element of text

< kx> ^e|^map(normal, ,PIPE,^leet(2,0)) a b c "abcdefghijk k"
< dbot> ^leet(2,0) a |^leet(2,0) b |^leet(2,0) c |^leet(2,0) "abcdefghijk |^leet(2,0) k"|
< kx> ^e|^map(normal, ,PIPE,^leet(2,0)) a b c "abcdefghijk k"|^exec
< dbot> 4 6 c "46cd3fgh1jk k"




mod_md5.c: ^md5
---------------
- md5 something

< kx> hi my name is darqbot and i love to irc in such cool ways|^e|^leet(0,1)|^md5
< dbot> 7a410f6b314311f8ae7d2fc1eb07f5bc



mod_me.c: ^me, ^me(x=?,y=?,z=?) function
----------------------------------------
- libmath eval test code


< kx> ^e|^me(x=1,y=2,z=100.3) x*(y+y)/1+2+z^x-z*log(x)
< dbot> f(x,y,z)=106.300000 ... vars={xyz} ::: f'(x)=((((y+y)+(x*0))+((z^x)*(log(z)+(x*(0/z)))))-((0*log(x))+(z*(1/x))))=365.899019

< kx> ^e|^me(x=1,y=2,z=100.3) x*(y+y)/1+2+z^x-z*log(x)*pi+sin(x)/cos(y)
< dbot> f'(x)=(((((y+y)+(x*0))+((z^x)*(log(z)+(x*(0/z)))))-((((0*log(x))+(z*(1/x)))*pi)+((z*log(x))*0)))+(((cos(x)*cos(y))-(sin(x)*(-(0*sin(y)))))/(cos(y)^2)))=149.798931

< kx> ^me sin(x)|^e
< dbot> f(x,y,z)=0.000000 ... vars={x} ::: f'(x)=cos(x)=1.000000

< kx> ^me tan(x)|^e
< dbot> f(x,y,z)=0.000000 ... vars={x} ::: f'(x)=(1/(cos(x)^2))=1.000000

as you can see, it solves for x,y,and/or z while also taking the derivative and solving for that as well.




mod_moby.c: ^moby, ^moby(list)
------------------------------
- fun old school thesaurus


< kx> hi my name is darqbot and i love to irc in such cool ways|^e|^moby
< dbot> hi my distinction is darqbot and ego enjoy headed for irc intrusive likeness coolth ways

< kx> ^e|^moby(list) love
< dbot> love,amor,aphrodite,astarte,bomfog,benthamism,christian charity,christian
love,cupid,eros,freya,kama,love,venus,accord,accordance,admiration,admire,adoration,adore,adulate,adulation,affair,affection,affinity,agape,agreement,aim
at,allegiance,altruism,amiability,amiableness,amicability,amicableness,amity,amor,amorousness,amour,angel,appreciate,ardency,ardor,attachment,attraction,babe,baby,baby-doll,bang,bask in,be all heart,be desirous of,be fond of,be partial to,be




mod_mongotestfmt.c: ^mongotestfmt
---------------------------------
- junk code for testing mongo routines



mod_nop.c: ^nop, ^nop
---------------------
- very powerful, more on this later.. as you can see from the above examples though, you can cause it to process text and prepend triggers such as ^e, so anything typed will get processed and echo'd


here's some examples, including the above examples:

< kx> ^nop(ina) \|^e\|^howie <-- turn chat into howie chat

^nop(call) <-- clear all hooks

turn chat into lisp interpreter:
< kx> ^nop(ina) \|^e\|^guile
< kx> (display 1)
< dbot> 1
< kx> (display (+ 1 3 9))
< dbot> 13


^nop(outp:outa:inp:ina:coutp:couta:cina:cinp:call:match:cmatch:outprob:inprob:prob)
 ^nop(prob) 3
 ^nop(ina) \|^echo\|^moby ..... also \|^echo\|^moby\|^to #jumping
 ^nop(match) #bosshogs #jumping heyhohi

^nop(prob) 2
^nop(ina) \|^echo\|^moby
^nop(match) #jumping



ask a user who typed something, auto, a question, using ^nop:
^nop(ina) \|^zero\|^echo\|^whoami : \|^quote ?\|^caps(low)

moby chat mirror:
^nop(ina) \|^echo\|^moby
^nop(ina) \|^!\|^echo\|^moby  <-- \|^! to crush any field sep pipes
^nop(prob) 2
^nop(match) #jumping

moby chat send to:
^nop(ina) \|^echo\|^moby\|^to #jumping
^nop(match) #bosshogs



pipe to jumping, howie/moby, remove leading nick:
^nop(ina) \|^e\|^howie\|^moby\|^pcre(-V) /.*: / \|^to #jumping





mod_help.c: ^help, ^help ^trigger
---------------------------------
- displays overall help or specific help message for a given trigger

< kx> ^help|^multi|^e
< dbot> ^multi (4),^echo (3695),^bold (4),^attrib (4),^: (-1),^! (-1),^# (-1),^alias (222),^a (222),^answer (2042),^ans
        (2042),^backtrace (51),^avl_del (360),^avl (360),^auto_rejoin (-1),^avl_clear (360),^avl_list (360),^avl_ins
        (360),^avl_tree (360),^bgc (-1),^bavlt (623),^bgcolor (-1),^bin (238),^color (-1),^caps (252),^bv (60),^bsdavltree
        (623),^caesar (12287),^char (516),^cat (31),^cmds (64),^dice (48),^dequeue (79),^console (117),^e (3695),^dumbgames
        (48),^howie (184),^fgcolor (-1),^festival (63),^enqueue (79),^emot (50),^exec (17816),^fgc (-1),^forkfortune
        (358),^fnsc (-1),^fnsasmx86 (69),^fork (35),^geoip (56),^fortune (358),^help (164),^guile (1422),^index (235),^hun
        (66),^html (138),^identify (221),^md5 (208),^leet (189),^internals (39),^map (381),^memset (235),^me (34),^moby
        (613),^say (3695),^quote (705),^pcre (194),^p (72),^nop (2),^packmulti (114),^push (184),^pop (184),^pmme (29),^print
        (72),^queue (79),^quiz (2042),^rindex (235),^relinkd (301),^rand (2986),^rainbow (1032),^rant (144)





mod_packmulti.c: ^packmulti(num)
--------------------------------
- packs multi-line text into fixed length strings

< kx> ^help|^e|^packmulti(100)|^multi
< dbot> ^multi (4),^echo (3690),^bold (4),^attrib (4),^: (-1),^! (-1),^# (-1),^alias (222),^a (222),^answer
< dbot> (2042),^ans (2042),^backtrace (49),^avl_del (355),^avl (355),^auto_rejoin (-1),^avl_clear (355),^avl
< dbot> _list (355),^avl_ins (355),^avl_tree (355),^bgc (-1),^bavlt (623),^bgcolor (-1),^bin (237),^color (-
< dbot> 1),^caps (246),^bv (23),^bsdavltree (623),^caesar (12287),^char (511),^cat (25),^cmds (63),^dice (47
< dbot> ),^dequeue (79),^console (111),^e (3690),^dumbgames (47),^howie (184),^fgcolor (-1),^festival (63),^
< dbot> enqueue (79),^emot (48),^exec (17810),^fgc (-1),^forkfortune (356),^fnsc (-1),^fnsasmx86 (69),^fork
< dbot> (34),^geoip (55),^fortune (356),^help (163),^guile (1407),^index (235),^hun (66),^html (138),^identi
< dbot> fy (221),^md5 (201),^leet (184),^internals (33),^map (362),^memset (235),^me (34),^moby (613),^say (
< dbot> 3690),^quote (695),^pcre (194),^p (72),^nop (2),^packmulti (113),^push (184),^pop (184),^pmme (29),^
< dbot> print (72),^queue (79),^quiz (2042),^rindex (235),^relinkd (301),^rand (2984),^rainbow (1032),^rant





mod_pcre.c: ^pcre, ^pcre(-i,-v,~v,-u,-e,-V) /regex/
---------------------------------------------------
- perl compatible regular expressions


< n9> ^e |^chan(users)|^pcre(-i ~v) /blast/
< vkx> blast

removing nick from howie:
 ^e|^howie fa[u
23:23 < vkx> n9: Please don't be so rude.
23:23 < n9> ^e|^howie sup? |^pcre(-v) /.*: /
23:23 < vkx> Not much.
23:23 < n9> nice



08:12 <@ng> ^e hello my name is dbot |^pcre(-V) /dbot/
08:12 < uas> hello my name is
08:12 <@ng> ^e hello my name is dbot |^pcre(~v) /dbot/
08:12 < uas> dbot 





mod_pmme.c: ^pmme
-----------------
- it'll pm you

^e stuff|^pmme




mod_print.c: ^print, ^p, ^p(b/c), ^p(rb/c)
------------------------------------------
- print func, only handles binary right now, needs to handle hex/all types of stuff

< kx> ^e|^p(b/c) i-am-darqbot
< dbot> 100101101011010010000110101101101011010000100110100001100100111010001110010001101111011000101110
< kx> ^e|^p(b/c) i-am-darqbot|^p(rb/c)
< dbot> i-am-darqbot
< kx> ^e|^p(rb/c) 100101101011010010000110101101101011010000100110100001100100111010001110010001101111011000101110
< dbot> i-am-darqbot





mod_queue.c: ^queue, ^enqueue, ^dequeue, ^queue(enqueue:dequeue:clear:list:size)
--------------------------------------------------------------------------------
- queue datastruct

< kx> ^queue(info)|^e
< kx> ^queue(list)|^e
< kx> ^enqueue 1
< kx> ^enqueue 2
< kx> ^enqueue 3
< kx> ^enqueue 4
< kx> ^queue(list)|^e
< dbot> [back] 4 3 2 1 [front]
< kx> ^dequeue|^e
< dbot> 1
< kx> ^dequeue|^e
< dbot> 2
< kx> ^dequeue|^e
< dbot> 3
< kx> ^dequeue|^e
< dbot> 4
< kx> ^dequeue|^e





mod_quiz.c: ^quiz, ^quiz(...)
-----------------------------
- quiz tool, very fun

< kx> ^quiz(list)
< dbot> POSIX_HEADERS POSIX_API INTEL UNIX_SIGNALS INTEL_VM INTEL_FPU TEST INTEL_MMX_SSE ERRNO PHRACK INTEL_64 SEC_GLOSS
        POSIX_DEFINITIONS INTEL_SAFE
< kx> ^quiz(setup) intel
< dbot> QUIZ (intel) question => Decimal adjust after addition
< kx> ^ans daa
< dbot> winner is kx! NEXT QUIZ (intel) question => Save processor extended states to memory
< kx> ^quiz(setup) posix intel
< dbot> previous answer was [XSAVE]! QUIZ (intel) question => Bit scan forward
< kx> ^ans bsf
< dbot> winner is kx! NEXT QUIZ (intel) question => Conditional move if sign (negative)
< kx> ^ans cmovs
< kx> ^quiz(info)
< dbot> quiz id: 218606873
< dbot>  channel: #darqbot , timeout: 60 , size: 222
< dbot>  multi: 1 , newlines: 1 , hidekey: 1 , fixkey=1
< dbot>  topics: intel
< dbot>  correct answers: 4 , incorrect answers: 1 , questions asked: 4
< dbot>  winners: kx(4)

contact me for my quiz db's...

the format of a quiz in mod_quiz_files should be (mod_quiz_files/sec_gloss):


*-property:::+++
     (N) Synonym for "confinement property" in the context of the Bell- LaPadula model. Pronunciation: star property.

+++
3DES:::+++
     (N) See: Triple Data Encryption Algorithm.

+++
A1 computer system:::+++
     (O) /TCSEC/ See: Tutorial under "Trusted Computer System Evaluation Criteria". (Compare: beyond A1.)

+++
ABA Guidelines:::+++
     (N) "American Bar Association (ABA) Digital Signature Guidelines"
     [DSG], a framework of legal principles for using digital
     signatures and digital certificates in electronic commerce.

+++
Abstract Syntax Notation One^^^ASN.1^^^ASN1:::+++
     (N) A standard for describing data objects. [Larm, X680] (See:
     CMS.)

     Tutorial: OSIRM defines computer network functionality in layers.
     Protocols and data objects at higher layers are abstractly defined
     to be implemented using protocols and data objects from lower
     layers. A higher layer may define transfers of abstract objects
     between computers, and a lower layer may define those transfers
     concretely as strings of bits. Syntax is needed to specify data
     formats of abstract objects, and encoding rules are needed to
     transform abstract objects into bit strings at lower layers. OSI
     standards use ASN.1 for those specifications and use various
     encoding rules for those transformations. (See: BER.)

     In ASN.1, formal names are written without spaces, and separate
     words in a name are indicated by capitalizing the first letter of
     each word except the first word. For example, the name of a CRL is
     "certificateRevocationList".

+++

see how it allows for multiple lines using +++ delims?


another example of one liners (mod_quiz_files/intel):

MOV:::Move data between general-purpose registers; move data between memory and general-purpose or segment registers; move immediates to general-purpose registers
CMOVE^^^CMOVZ:::Conditional move if equal/Conditional move if zero
CMOVNE^^^CMOVNZ:::Conditional move if not equal/Conditional move if not zero
CMOVA^^^CMOVNBE:::Conditional move if above/Conditional move if not below or equal
CMOVAE^^^CMOVNB:::Conditional move if above or equal/Conditional move if not below
CMOVB^^^CMOVNAE:::Conditional move if below/Conditional move if not above or equal
CMOVBE^^^CMOVNA:::Conditional move if below or equal/Conditional move if not above
CMOVG^^^CMOVNLE:::Conditional move if greater/Conditional move if not less or equal
CMOVGE^^^CMOVNL:::Conditional move if greater or equal/Conditional move if not less
CMOVL^^^CMOVNGE:::Conditional move if less/Conditional move if not greater or equal
CMOVLE^^^CMOVNG:::Conditional move if less or equal/Conditional move if not greater
CMOVC:::Conditional move if carry
CMOVNC:::Conditional move if not carry
CMOVO:::Conditional move if overflow


also notice, ^^^ is like ||, multiple answers







mod_quote.c: ^quote, ^quite(album)
----------------------------------
- pull quotes from random albums or specific album


< kx> ^quote(big)|^e
< dbot> by the free choice of those who love science well enough to 'make themselves eunuchs for the Kingdom of Heaven's sake.'
< kx> ^quote|^e
< dbot> and what I do with it is nobody else's concern.
< kx> ^quote(tyson)|^e
< dbot> You can set a good example that you don't have to throw blows or be belligerent to get your point across.


example format of mod_quote_files/tyson:

  "I'm not used to sensitivity any more. When I see people cry when I lose, save your tears. I don't know how to handle people crying any more. I lost my sensitivity like that. Please, you embarrass me when you cry because I don't know what to do when you cry. I don't know what to say."
  "When you see me smash somebody's skull, you enjoy it."
  "When you see me smash somebody's skull, you enjoy it."
  "Everybody in boxing probably makes out well except for the fighter."
  "I don't have the desire to hurt anyone anymore. I see a fly, but I don't have the nerve to get up and kill it."






mod_rainbow.c: ^rainbow, ^rainbow(..)
-------------------------------------
- modifies text to fit a specified color scheme, alternating between each color

RAINBOW_OPS=rand,nochar,nospace,fgdef,bgdef,fgwhite,fgblack,fgblue,fggreen,fgred,fgbrown,fgpurple,fgorange,fgyellow,fglime,fgteal,fgcyan,fgroyal,fgpink,fggrey,fgsilver,bgwhite,bgblack,bgblue,bggreen,bgred,bgbrown,bgpurple,bgorange,bgyellow,bglime,bgteal,bgcyan,bgroyal,bgpink,bggrey,bgsilver

quick examples:

sysvbanner examples: red background, black inside, HELLO
^e |^sysvbanner hello|^multi |^char(RLOWER,MINVERT) |^rainbow(nospace,nochar,bgred)

squad, rainbow background:
^e |^sysvbanner SQUAD.|^multi |^char(RLOWER,MINVERT) |^rainbow(nospace,nochar,bgdef)

sick black background, squad with punct style:
^e |^sysvbanner SQUAD.|^multi |^char(RPUNCT,MSAME)|^rainbow

good example of normal and MINVERT: 'bleh' with red chars, black background, then red background, black chars
^e |^sysvbanner bleh.|^multi |^char(RBINARY,MSAME) |^rainbow(nospace,nochar,bgred)
^e |^sysvbanner bleh.|^multi |^char(RBINARY,MSAME) |^char(RBINARY,MINVERT) |^rainbow(nospace,nochar,bgred)



bgdef = normal background rainbow colors
fgdef = normal foreground rainbow colors






mod_rand.c: ^rand, ^rand(num,word,data)
---------------------------------------
- randomize stuff


< kx> ^quote(tyson)|^e|^rand(word)
< dbot> a
< kx> ^quote(tyson)|^e|^rand(word)
< dbot> streets.
< kx> ^quote(tyson)|^e|^rand(word)
< dbot> with
< kx> ^e|^rand(num)
< dbot> 222957612
< kx> ^e|^rand(num)
< dbot> 82235522
< kx> ^e|^rand(data)
< dbot> ��t��l�8h�ZJǾ��{��#(�o���¥�ؔ L;���,!�GlME�6��܆����h]2@�?�-3FY`g������$-_��Q���Q�TR�T�P�%)��Ɏ`�;1�h�ۢ��Ԉ���z��\�l����|_]�X        �����@��T4[��՗�2���]u4���IΪaۜ!yh��T��H*��0�1�U�D��9�C�e߻�G��[�f�����0��Wޱ[���dN���`Rp1���SZ/�
.p�^��u�Mс]�R"���%T�VLq�N��|G��A,PYz!�}~ϡR�#w��S3�2�:IԶPe�����KE���X#KF�RHZ%;FX9�aN7�s�BP�1�0��SL�����5�;���$���eͫS�DC�6�30�Bѐw���

trimming random data:

< kx> ^rand(data)|^!|^strlen(10)|^char(RALNUM)|^e
< dbot> tkKvk88hJg
< kx> ^rand(data)|^!|^strlen(10)|^char(RALNUM)|^e
< dbot> vHgVsiQXs5
< kx> ^rand(data)|^!|^strlen(10)|^char(RALNUM)|^e
< dbot> jTMCzQe7jR
< kx> ^rand(data)|^!|^strlen(10)|^char(RALNUM)|^e
< dbot> 9iPcbiDdKM
< kx> ^rand(data)|^!|^strlen(10)|^char(RALNUM)|^e
< dbot> f8a9Bnta1r

you have to use the ^! crush trigger in order to crush possible random generated break triggers (^:)

< kx> ^rand(data)|^!|^strlen(10)|^e
< dbot> ‰)@ÀT`§+;
< kx> ^rand(data)|^!|^strlen(10)|^e
< dbot> Pƒ%IUgƒSùˆ
< kx> ^rand(data)|^!|^strlen(10)|^e
< dbot> ;êT ¿«ÚKÖá

notice ^!..






mod_rant.c: ^rant, ^rant(push:size:pop:clear)
---------------------------------------------
- captures multi line text and saves it for future use

< kx> ^rant(push)
< kx> hey
< kx> whats
< kx> going
< kx> on
< kx> over
< kx> there
< kx> ^rant(size)|^e
< dbot> 8
< kx> ^rant(size)|^e
< dbot> 9
< kx> ^rant(size)|^e
< dbot> 10
< kx> ^rant(size)|^e
< dbot> 11
< kx> ^rant(pop)|^e
< dbot>  hey whats going on over there ^rant(size)|^e ^rant(size)|^e ^rant(size)|^e ^rant(size)|^e




mod_relinkd.c: ^relinkd, temporarily broken
-------------------------------------------
- formerly control the relink daemon. able to purge connections, swap connections, etc



mod_rf.c: ^rf, ^rf(func1:::func2:::funcN) <-- deprecated
--------------------------------------------------------
- provides us with a random function, applies input to it

< kx> ^e hello bro what the heck is up|^rf(^echo:::^sort:::^leet(3):::^reverse:::^moby)
< dbot> bro heck hello is the up what
< kx> ^e hello bro what the heck is up|^rf(^echo:::^sort:::^leet(3):::^reverse:::^moby)
< dbot> accost bro what the heck is straight up
< kx> ^e hello bro what the heck is up|^rf(^echo:::^sort:::^leet(3):::^reverse:::^moby)
< dbot> ]-[3110 6|z() wh/-\7 7}-{3 h3<|< 1$ (_)|'
< kx> ^e hello bro what the heck is up|^rf(^echo:::^sort:::^leet(3):::^reverse:::^moby)
< dbot> bro heck hello is the up what


the reason why randfunc and future-such functions are deprecated:

03:02 <@ng> ^e |^^(^rand caps moby leet)) hi
03:02 < xxx> HI
03:02 <@ng> ^e |^^(^rand caps moby leet)) hi
03:02 < xxx> hi
03:02 <@ng> ^e |^^(^rand caps moby leet)) hi
03:02 < xxx> h1


^() solves everything.




mod_rot13.c: ^rot13
-------------------
- rot13's text

< kx> ^e hello rot13 me|^rot13
< dbot> uryyb ebg13 zr






mod_rss.c: ^rss, ^rss(info,add,-1/1,update,get)
-----------------------------------------------
- rss module. grabs updates or entire rss feeds


^rss(info) key |^e
^rss(info) |^e|^multi
^rss(add) key http://valuecom/rss
^rss(-1) <- pull from bottom
^rss(1) pull from top
^rss(update) key or url <--- only post updates



< kx> ^rss wired.danger|^e
< dbot> [wired.danger] Danger Room (http://adarq.org/t/0f) [wired.danger] After U.S. Says Sorry, Pakistan Reopens Border
        (http://adarq.org/t/0z6) [wired.danger] Pentagon’s Brain-Powered Video-games Might Treat PTSD (http://adarq.org/t/0z7)
        [wired.danger] Feds Look to Fight Leaks With ‘Fog of Disinformation’ (http://adarq.org/t/0z8) [wired.danger] Belarus
        Dictator Says He’s Totally Not Building a Dynasty (http://adarq.org/t/0z9)
< kx> ^rss wired.danger|^e|^multi
< dbot> [wired.danger] Danger Room (http://adarq.org/t/0f)
< dbot> [wired.danger] After U.S. Says Sorry, Pakistan Reopens Border (http://adarq.org/t/0z6)
< dbot> [wired.danger] Pentagon’s Brain-Powered Video-games Might Treat PTSD (http://adarq.org/t/0z7)
< dbot> [wired.danger] Belarus Dictator Says He’s Totally Not Building a Dynasty (http://adarq.org/t/0z9)
< dbot> [wired.danger] Gallery: The Tattooed, Undead Planes of the Air Force ‘Boneyard’ (http://adarq.org/t/0wo)
< kx> ^a rssupdate|^e|^multi
< dbot> [cnn.topstories] 1.1 million lack AC in heat wave (http://adarq.org/t/108)
< dbot> /tmp/mod_rss-138211449305855.xml/tmp/mod_rss-13821791394073.xml/tmp/mod_rss-13821768696260.xml[wired.science] Newly
        Discovered Particle Appears to Be Long-Awaited Higgs Boson (http://adarq.org/t/109)
< dbot> [wsj.us] Physicists Discover Higgs-Like Particle (http://adarq.org/t/10a)
< dbot> [wsj.us] Poker Pro Wins $17 Million (http://adarq.org/t/10b)
< dbot> [wsj.us-business] Lotte Shopping Preferred Bidder for Himart Stake (http://adarq.org/t/10c)
< dbot> [wsj.us-business] Renault India Introduces Duster SUV (http://adarq.org/t/10d)
< dbot> [wsj.world] New Particle Looks Like Higgs Boson (http://adarq.org/t/10e)
< dbot> [wsj.world] Myanmar Vice President Resigns (http://adarq.org/t/10f)
< dbot> [wsj.world] Police in Standoff in Germany (http://adarq.org/t/10g)
< dbot> [wsj.world] China State Grid Signs Contract With Venezuela (http://adarq.org/t/10h)
< kx> ^rss(info)|^e
< dbot> bugtraq cert cnn.crime cnn.topstories cnn.world dataloss mmajunkie politifact1 politifact2 whatreallyhappened
        wired.danger wired.dayintech wired.politics wired.science wired.threat wsj.digits wsj.smartmoney wsj.tech wsj.us
        wsj.us-business wsj.world
< kx> ^rss(info) wired.danger|^e
< dbot> wired.danger http://www.wired.com/dangerroom/feed/




mod_save.c: ^rss <-- temporarily broken
---------------------------------------
- formerly would save a currnet-state of a configuration file to a local file





mod_shake1.c: ^shake1
---------------------
- generates a random Shakespearean insult

< kx> ^e kx is a |^:|^shake1
< dbot> kx is a lumpish spur-galled miscreant



mod_sig.c: ^sig, ^sig <num>
---------------------------
- generates <num> sig, be careful with num=9 !@!#




mod_smf.c: ^smf, ^smf(journal-topic,journal-subforum)
-----------------------------------------------------
- unfortunately i only have it programmed for my own forum right now.. easy to add in custom smf forums though.





mod_sort.c: ^sort, ^sort(forc:backc:forw:backw:randc:randw)
-----------------------------------------------------------
- sort module

< kx> a okdokd r9w j-02j9 d mof poj pofjpqw4i12ir`=0j 9 oisnd oin oi|^e|^sort
< dbot> 9 a d j-02j9 mof oi oin oisnd okdokd pofjpqw4i12ir`=0j poj r9w
< kx> a okdokd r9w j-02j9 d mof poj pofjpqw4i12ir`=0j 9 oisnd oin oi|^e|^sort(forc)
< dbot>            -001224999=`addddffiiiiijjjjjkkmnnoooooooopppqrrsww
< kx> a okdokd r9w j-02j9 d mof poj pofjpqw4i12ir`=0j 9 oisnd oin oi|^e|^sort(forw)
< dbot> 9 a d j-02j9 mof oi oin oisnd okdokd pofjpqw4i12ir`=0j poj r9w
< kx> a okdokd r9w j-02j9 d mof poj pofjpqw4i12ir`=0j 9 oisnd oin oi|^e|^sort(backc)
< dbot> wwsrrqpppoooooooonnmkkjjjjjiiiiiffdddda`=999422100-
< kx> a okdokd r9w j-02j9 d mof poj pofjpqw4i12ir`=0j 9 oisnd oin oi|^e|^sort(backw)
< dbot> r9w poj pofjpqw4i12ir`=0j okdokd oisnd oin oi mof j-02j9 d a 9
< kx> a okdokd r9w j-02j9 d mof poj pofjpqw4i12ir`=0j 9 oisnd oin oi|^e|^sort(randc)
< dbot> iomn nipo dsio 9 j0`=rqi21ikp4wojfj jop fo d 92j0 -w9r dodko a
< kx> a okdokd r9w j-02j9 d mof poj pofjpqw4i12ir`=0j 9 oisnd oin oi|^e|^sort(randw)
< dbot> oi oin a 9 oisnd mof pofjpqw4i12ir`=0j poj d j-02j9 r9w okdokd





mod_sp.c: ^sp, ^sp(on:off)
--------------------------
- Stanford lexical parser test code


< kx> ^sp hello my name is andrew and i love bot programming.|^e
< dbot> [hello/UH, my/PRP$, name/NN, is/VBZ, andrew/JJ, and/CC, i/FW, love/VBP, bot/NN, programming/NN, ./.]:::::[dep(andrew-5,
        hello-1), poss(name-3, my-2), nsubj(andrew-5, name-3), cop(andrew-5, is-4), root(ROOT-0, andrew-5), nsubj(love-8, i-7),
        conj_and(andrew-5, love-8), nn(programming-10, bot-9), dobj(love-8, programming-10)]






mod_stack.c: ^stack, ^push, ^pop, ^stack(push:pop:size:clear:list)
------------------------------------------------------------------
- stack data structure

< kx> ^stack(list)|^
< kx> ^push 1
< kx> ^push 2
< kx> ^push 3
< kx> ^stack(list)|^e
< dbot> [top] 3 2 1 [bottom]
< kx> ^pop|^e
< dbot> 3
< kx> ^pop|^e
< dbot> 2
< kx> ^pop|^e
< dbot> 1
< kx> ^pop|^e



mod_string.c: ^strlen, ^space, ^strzero, ^strchr, ^strrchr, ^index, ^rindex, ^memset, ^strstr, ^strcasestr, ^strprepend, ^subchar, ^reverse, ^unique
--------------------------------------------------------------------------------------------------------------------------------
- various string functions

18:03 <@ng> ^e abcdefghijklmnopqrstuvwxyz|^space(1)
18:03 < uas> a b c d e f g h i j k l m n o p q r s t u v w x y z
18:03 <@ng> ^e abcdefghijklmnopqrstuvwxyz|^space(1)|^strlen(b,10)
18:03 < uas> a b c d e
18:03 <@ng> ^e abcdefghijklmnopqrstuvwxyz|^space(1)|^strlen(a,10)
18:03 < uas> f g h i j k l m n o p q r s t u v w x y z
18:04 <@ng> ^e abcdefghijklmnopqrstuvwxyz|^space(1)|^strlen(a,10)|^strchr(a,j)
18:04 < uas> j k l m n o p q r s t u v w x y z
18:04 <@ng> ^e abcdefghijklmnopqrstuvwxyz|^space(1)|^strlen(a,10)|^strchr(a,j)|^space(0)
18:04 < uas> jklmnopqrstuvwxyz
18:04 <@ng> ^e abcdefghijklmnopqrstuvwxyz|^space(1)|^strlen(a,10)|^strchr(a,j)|^space(0)|^strstr(xyz)
18:04 < uas> jklmnopqrstuvwx
18:04 <@ng> ^e abcdefghijklmnopqrstuvwxyz|^space(1)|^strlen(a,10)|^strchr(a,j)|^space(0)|^strstr(a,xyz)
18:04 < uas> xyz

18:32 <@ng> ^e abcdefghijklmnopqrstuvwxyz|^subchar(xy)
18:32 < uas> abcdefghijklmnopqrstuvwyyz

18:36 <@ng> ^e abcdefghijklmnopqrstuvwxyz|^subchar(1x)
18:36 < uas> abcdefghijklmnopqrstuvwxyz
18:36 <@ng> ^e abcdefghijklmnopqrstuvwxyz|^memset(A)
18:36 < uas> AAAAAAAAAAAAAAAAAAAAAAAAAA
18:36 <@ng> ^e abcdefghijklmnopqrstuvwxyz|^memset(A)|^subchar(A.)
18:36 < uas> ..........................






mod_sysvbanner.c: ^sysvbanner
-----------------------------
- banner printing mod

< kx> ^sysvbanner DBOT!!!|^e|^multi
< dbot>
< dbot>  ######  ######  ####### #######   ###     ###     ###
< dbot>  #     # #     # #     #    #      ###     ###     ###
< dbot>  #     # #     # #     #    #      ###     ###     ###
< dbot>  #     # ######  #     #    #       #       #       #
< dbot>  #     # #     # #     #    #
< dbot>  #     # #     # #     #    #      ###     ###     ###
< dbot>  ######  ######  #######    #      ###     ###     ###
< dbot>





mod_tail.c: ^tail, ^tail(clear:run:add:del:list)
------------------------------------------------
- monitor changes to a file

using a timer to monitor changes to system logs:

14:52 <@ng> ^timer(add) 60:ttt:FOREVER:ttt:\|^tail\|^e\|^multi
14:52 <@ng> ^tail(add) /var/log/auth.log
14:52 <@ng> ^tail(list)|^e
14:52 < uas> /var/log/auth.log ondev=65024, ino=4023759, mode=33261, nlink=1, uid=0, gid=4, rdev=0, size=8400, blksize=4096,
             blocks=24, atime=1339912496, mtime=1340043421, ctime=1340043421
14:52 <@ng> ^tail|^e|^multi
14:53 < uas> /var/log/auth.log [Jun 18 14:52:40 life sshd[11866]: Bad protocol version identification 'work dammit............'
             from 127.0.0.1




mod_timer.c: ^timer, ^timer(add:del:list:clear)
-----------------------------------------------
- add or delete timers

^timer(add) INTERVAL:ttt:DURATION:ttt:\^e escaped\^md5 pipeline





mod_tiny.c: ^tiny, ^tiny <url>
------------------------------
- tinyurl a given url

< kx> ^tiny http://aol.com|^e
< dbot> http://adarq.org/t/0t8





mod_tinypaw.c: ^tinypaw, ^tinypaw <url> <keywords>
--------------------------------------------------
- 'tinyurl' a given url using specified keywords

< kx> ^tinypaw http://aol.com okaok okd oko kofk o kfo koppsdlpsdlpskosk okf o|^e
< dbot> http://adarq.org/tp/okaok-okd-oko-kofk-o-kfo-koppsdlpsdlpskosk-okf-o




mod_to.c: ^to, temporarily broken
---------------------------------
- pipes data to a different bot stack, ability to pipe data from one ircd/server to another/multiple clients



mod_uu.c: ^uu, ^uu(enc:dec)
---------------------------
- uuencode/uudecode

< kx> hi im so tired |^e|^uu(enc)
< dbot> O4YUOKoUQqwUR4ZmNKEU
< kx> hi im so tired |^e|^uu(enc)|^uu(dec)
< dbot> hi im so tired
< kx> ^e|^uu(dec) O4YUOKoUQqwUR4ZmNKEU
< dbot> hi im so tired




mod_var.c: ^var, ^var(usleep,bufsz,multi,maxflood,nesting_cur,nesting,allowpm,sigprotect,timer_sleep,timer_sort)
----------------------------------------------------------------------------------------------------------------
- set special vars

< kx> ^var(bufsz)|^e
< dbot> 10024
< kx> ^var(buf100)|^e <-- proving that im tired
< kx> ^var(bufsz)|^e
< dbot> 10024
< kx> ^var(bufsz) 100|^e
< kx> ^var(bufsz)|^e
< dbot> 100




mod_whoami.c: ^whoami
---------------------
- tells you, who you are

< kx> ^e|^whoami
< dbot> kx




root@life:/darqbot-dev# wc *.[c,h] */*.[c,h]|tail -n 1
  69010  162155 1337645 total

-- adarq
