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
/* darqbot 1.0 (2012)
  multi purpose chat & knowledge bot
  -- adarqui ::: adarq.org
 */
#include "mod_geoip.h"

void __geoip_init__(void)
{

	strlcpy_buf(mod_geoip_info.name, "mod_geoip");
	strlcpy_buf(mod_geoip_info.trigger, "^geoip");

	mod_geoip_info.init = geoip_init;
	mod_geoip_info.fini = geoip_fini;
	mod_geoip_info.help = geoip_help;
	mod_geoip_info.run = geoip_run;

	mod_geoip_info.output = NULL;
	mod_geoip_info.input = NULL;

	debug(NULL, "__geoip_init__: Loaded mod_geoip\n");

	return;
}

bot_t *geoip_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "geoip_init: Entered\n");
	return NULL;
}

bot_t *geoip_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "geoip_fini: Entered\n");
	return NULL;
}

bot_t *geoip_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "geoip_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^geoip <ip>";

	return NULL;
}

bot_t *geoip_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt;

	debug(bot, "geoip_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "geoip_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (_bot_shouldreturn(bot))
		return NULL;

	opt = 0;

	MOD_OPTIONS_TOP_HALF;
	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF;
	l_new_str = geoip_change_string(l_str_ptr, opt);
	MOD_PARSE_BOTTOM_HALF;

	return bot;
}

char *geoip_change_string(char *string, int opt)
{
	char *str = NULL;

	GeoIP *gi;
	GeoIPRecord *gir;

	char **ret;
	char *time_zone;

	gi = NULL;
	gir = NULL;
	time_zone = NULL;
	ret = NULL;

	char *sep_ptr;

	if (!string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	_strstrip_chars(string, " !@#$%^&*()_+=';\"[]{}<>,?");

	gi = GeoIP_open("/custom_builds/share/GeoIP/GeoLiteCity.dat",
			GEOIP_STANDARD);
	if (!gi)
		return NULL;

	gir = (GeoIPRecord *) GeoIP_record_by_name(gi, string);
	if (gir) {
		ret = GeoIP_range_by_ip(gi, (const char *)string);
		time_zone =
		    GeoIP_time_zone_by_country_and_region(gir->country_code,
							  gir->region);
		str =
		    str_unite("%s:%s:%s:%s:%s:%s:%s:%s:%i:%f:%f:%d:%d:%s:%s:%s",
			      string, _mk_NA(gir->continent_code),
			      _mk_NA(gir->country_code),
			      _mk_NA(gir->country_name), _mk_NA(gir->region),
			      _mk_NA(GeoIP_region_name_by_code
				     (gir->country_code, gir->region)),
			      _mk_NA(gir->city), _mk_NA(gir->postal_code),
			      gir->charset, gir->latitude, gir->longitude, 0,
			      gir->area_code, _mk_NA(time_zone), ret[0],
			      ret[1]);
		GeoIP_range_by_ip_delete(ret);
		GeoIPRecord_delete(gir);
	}
	GeoIP_delete(gi);

	return str;
}

static const char *_mk_NA(const char *p)
{
	return p ? p : "N/A";
}
