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
#include "mod_xml.h"

void __xml_init__(void)
{
	strlcpy_buf(mod_xml_info.name, "mod_xml");
	strlcpy_buf(mod_xml_info.trigger, "^xml");

	mod_xml_info.init = xml_init;
	mod_xml_info.fini = xml_fini;
	mod_xml_info.help = xml_help;
	mod_xml_info.run = xml_run;

	mod_xml_info.output = NULL;
	mod_xml_info.input = NULL;

	debug(NULL, "__xml_init__: Loaded mod_xml\n");

	return;
}

bot_t *xml_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "xml_init: Entered\n");
	return NULL;
}

bot_t *xml_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "xml_fini: Entered\n");
	return NULL;
}

bot_t *xml_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "xml_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^xml ::: xml's input or argument";

	return NULL;
}

bot_t *xml_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr, *opt_str = NULL;
	int opt;

	debug(bot, "xml_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "xml_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	opt = 0;

	MOD_OPTIONS_TOP_HALF;
	opt_str = dl_options_ptr;
	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF;
	l_new_str = xml_change_string(l_str_ptr, opt_str, opt);
	MOD_PARSE_BOTTOM_HALF;

	return bot;
}

char *xml_change_string(char *string, char *opt_str, int opt)
{
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;

	dlist_t *dl_text = NULL, *dl_match_strings = NULL;

	char *str = NULL;
	char *sep_ptr;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	if (!sNULL(string))
		return NULL;

	doc = xmlReadMemory(string, strlen(string), NULL, NULL, 0);

	if (doc == NULL) {
		return NULL;
	}

	/*Get the root element node */
	root_element = xmlDocGetRootElement(doc);

	if (sNULL(opt_str)) {
		dl_match_strings =
		    tokenize(NULL, opt_str,
			     TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE, ",");
	}

	xml_op_traverse(&dl_text, root_element,
			dl_match_strings != NULL ? &dl_match_strings : NULL);

	/*free the document */
	xmlFreeDoc(doc);

	/*
	 *Free the global variables that may
	 *have been allocated by the parser.
	 */
	xmlCleanupParser();

	if (dl_text) {
		str = dlist_to_str_fix(dl_text, " ", NULL);
		dlist_fini(&dl_text, free);
	}

	if (dl_match_strings)
		tokenize_destroy(NULL, &dl_match_strings);

	return str;
}

void
xml_op_traverse(dlist_t ** dl_text, xmlNode * a_node,
		dlist_t ** dl_match_strings)
{
	char *str_ptr = NULL;
	xmlNode *cur_node = NULL;

	if (!dl_text)
		return;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		str_ptr = NULL;
		if (cur_node->type == XML_ELEMENT_NODE) {
			if (dl_match_strings) {
				str_ptr =
				    (char *)dlist_traverse(dl_match_strings,
							   xml_compare,
							   (void *)
							   cur_node->name);
			}

			if (str_ptr && cur_node->children) {
				dl_str_unite(dl_text, "%s=%s\n", cur_node->name,
					     cur_node->children->content);
			} else if (!dl_match_strings) {
				str_ptr =
				    (char *)dlist_traverse(dl_text, xml_compare,
							   (void *)
							   cur_node->name);
				if (!str_ptr) {
					dl_str_unite(dl_text, "%s",
						     cur_node->name);
				}
			}

		}

		xml_op_traverse(dl_text, cur_node->children, dl_match_strings);

	}

	return;
}

void *xml_compare(void *pa, void *pb)
{
	dlist_t *db = NULL;
	void *v = NULL;
	char *a = NULL, *b = NULL;

	if (!pa || !pb)
		return NULL;

	db = (dlist_t *) pb;

	a = (char *)(pa);
	b = (char *)dlist_data(db);

	if (!strcasecmp(a, b))
		return a;

	return v;
}
