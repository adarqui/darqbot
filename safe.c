/*
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
*/
#include "bot.h"

int
safe_close (int fd)
{

debug(NULL, "safe_close: fd=%i\n", fd);

  if (!fd)
    return 0;

  return (close (fd));
}



     void
     safe_event_set(struct event *ev, int fd, short event, void (*fn)(int, short, void *), void *arg) {

debug(NULL, "safe_event_set: Entered: ev=%p, fd=%i, event=%i\n", ev, fd, event);

return event_set(ev,fd,event,fn,arg);
}



     int
     safe_event_add(struct event *ev, struct timeval *tv) {

debug(NULL, "safe_event_add: Entered: ev=%p\n",ev);

return event_add(ev, tv);
}


     int
     safe_event_del(struct event *ev){

debug(NULL, "safe_event_del: Entered: ev=%p\n",ev);

return event_del(ev);
}


