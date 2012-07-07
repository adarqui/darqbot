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
#include "mod_me.h"

void __me_init__(void)
{

	strlcpy_buf(mod_me_info.name, "mod_me");
	strlcpy_buf(mod_me_info.trigger, "^me");

	mod_me_info.init = me_init;
	mod_me_info.fini = me_fini;
	mod_me_info.help = me_help;
	mod_me_info.run = me_run;

	mod_me_info.output = NULL;
	mod_me_info.input = NULL;

	debug(NULL, "__me_init__: Loaded mod_me\n");

	return;
}

bot_t *me_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "me_init: Entered\n");
	return NULL;
}

bot_t *me_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "me_fini: Entered\n");
	return NULL;
}

bot_t *me_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "me_help: Entered\n");
	char *str = NULL;

	if (!bot)
		return NULL;

	bot->dl_module_help = "^me";

	str =
	    str_unite
	    ("Supported constants are (names that should be used are given in parenthesis): e (e), log2(e) (log2e), log10(e) (log10e),\n"
	     "ln(2) (ln2), ln(10) (ln10), pi (pi), pi / 2 (pi_2), pi / 4 (pi_4), 1 / pi (1_pi), 2 / pi (2_pi), 2 / sqrt(pi) (2_sqrtpi),\n"
	     "sqrt(2) (sqrt) and sqrt(1 / 2) (sqrt1_2).\n\n"
	     "Variable name is any combination of alphanumericals and _ characters beginning with a non-digit that is not elementary function name.\n\n"
	     "Supported elementary functions are (names that should be used are given in parenthesis): exponential (exp), logarithmic\n"
	     "(log), square root (sqrt), sine (sin), cosine (cos), tangent (tan), cotangent (cot), secant (sec), cosecant (csc), inverse\n"
	     "sine (asin), inverse cosine (acos), inverse tangent (atan), inverse cotangent (acot), inverse secant (asec), inverse\n"
	     "cosecant (acsc), hyperbolic sine (sinh), cosine (cosh), hyperbolic tangent (tanh), hyperbolic cotangent (coth), hyperbolic\n"
	     "secant (sech), hyperbolic cosecant (csch), hyperbolic inverse sine (asinh), hyperbolic inverse cosine (acosh), hyperbolic\n"
	     "inverse tangent (atanh), hyperbolic inverse cotangent (acoth), hyperbolic inverse secant (asech), hyperbolic inverse\n"
	     "cosecant (acsch), absolute value (abs), Heaviside step function (step) with value 1 defined for x = 0, Dirac delta\n"
	     "function with infinity (delta) and not-a-number (nandelta) values defined for x = 0, and error function (erf).\n\n"
	     "Supported unary operation is unary minus ('-').\n\n"
	     "Supported binary operations are addition ('+'), subtraction ('+'), multiplication ('*'), division multiplication ('/') and exponentiation ('^').\n");

	strlcat_bot(bot->txt_data_out, str);

	free(str);

	return NULL;
}

bot_t *me_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt = 0;
	me_t me;

	debug(bot, "me_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	debug(bot,
	      "me_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	stat_inc(bot, bot->trig_called);

	if (bot_shouldreturn(bot))
		return NULL;

	memset(&me, 0, sizeof(me));

	MOD_OPTIONS_TOP_HALF;
	me_parse_options(dl_options_ptr, &me);
	MOD_OPTIONS_BOTTOM_HALF;

	bot->shouldsend = 1;

	MOD_PARSE_TOP_HALF_NODL;
	l_new_str = me_change_string(l_str_ptr, opt, &me);
	MOD_PARSE_BOTTOM_HALF_NODL;

	return bot;
}

char *me_change_string(char *string, int opt, me_t * me)
{
	void *f = NULL, *f_prim = NULL;
	char *sep_ptr;
	char *str = NULL;

	char **names = NULL;
	int count = 0;

	char *f_prim_str = NULL;

	char *vars_str = NULL;

	double res = 0, res_prim = 0;

	debug(NULL, "me_change_string: Entered: %s\n", string);

	if (!me || !string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	if (!strlen(string))
		return NULL;

	f = evaluator_create(string);
	if (!f)
		return NULL;

	evaluator_get_variables(f, &names, &count);

	if (!names)
		goto cleanup;

	vars_str =
	    array_to_str((void **)names, count, (char *(*)(void *))strdup);
	if (!vars_str)
		goto cleanup;

	f_prim = evaluator_derivative_x(f);

	if (!f_prim)
		goto cleanup;

	f_prim_str = evaluator_get_string(f_prim);

	res = evaluator_evaluate_x_y_z(f, me->x, me->y, me->z);
	res_prim = evaluator_evaluate_x_y_z(f_prim, me->x, me->y, me->z);

	str =
	    str_unite("f(x,y,z)=%f ... vars={%s} ::: f'(x)=%s=%f", res,
		      vars_str, f_prim_str, res_prim);

 cleanup:
	if (f_prim)
		evaluator_destroy(f_prim);

	if (f)
		evaluator_destroy(f);

	if (vars_str)
		free(vars_str);

	return str;
}

void me_parse_options(char *str, me_t * me)
{
	dlist_t *dl = NULL, *dptr;

	debug(NULL, "me_parse_options: Entered\n");
	if (!str || !me)
		return;

	dl = tokenize(NULL, str, TOKENIZE_NORMAL | TOKENIZE_MATCHANY, " ,");
	if (!dl)
		return;

	dlist_fornext(dl, dptr) {
		str = (char *)dlist_data(dptr);
		if (!str)
			continue;

		if (!strncasecmp_len(str, "x=")) {
			me->x = atof(&str[2]);
			me->x_set = 1;
		} else if (!strncasecmp_len(str, "y=")) {
			me->y = atof(&str[2]);
			me->y_set = 1;
		} else if (!strncasecmp_len(str, "z=")) {
			me->z = atof(&str[2]);
			me->z_set = 1;
		}
	}

	tokenize_destroy(NULL, &dl);

	return;
}
