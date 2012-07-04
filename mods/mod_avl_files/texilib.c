/* texitree - draws PostScript and plain text tree diagrams
   Copyright (C) 2002, 2004 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to: Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 
   02110-1301 USA. */

#include "texilib.h"
#include "helvetica.inc"
#define font helv_font

/*
int main(int argc, char *argv[]) {
texitree_str(argv[1]);
return 0;
}
*/

char * texitree_str(char * string) 
{
FILE * memstream;

  struct token token;
  enum output_mode mode = 0;

  struct tree **input = NULL;
  int *radius = NULL;
  int m, n;

  int width, height;
  int page_width, page_height;
  

  int i, y;

if(setjmp(texilib_jmpenv)> 0) goto cleanup;

if(!string) return NULL;

//bot_debug(NULL, "texitree_str: string=%s, EOF=%i\n", string, EOF);

      mode = MODE_TEXT;
      page_width = 72;
      page_height = 66;
/* XXX 
      if (argc == 4 && !strcmp (argv[2], "-w"))
	page_width = atoi (argv[3]);
*/

memstream = fmemopen(string, strlen(string)+1, "r");
if(!memstream) return NULL;

/*
string[strlen(string)] = EOF;
*/


  lex_init (&token);
  lex_get (memstream, &token);

  n = m = 0;
  while (token.type != TOKEN_EOF)
    {
      if (n >= m)
	{
	  m = 2 * n + 8;
	  input = xrealloc (input, sizeof *input * m);
	  radius = xrealloc (radius, sizeof *radius * m);
	}

      input[n++] = tree_parse (memstream,&token, mode);
    }

  height = 0;
  for (i = 0; i < n; i++)
    {
	text_measure_tree (input[i]);

      height += input[i]->h;
    }

  width = 0;
  for (i = 0; i < n; i++)
    width = max (width, input[i]->w);
  if (width > page_width)
    page_width = width;

  y = mode = 0;

  for (i = 0; i < n; i++)
    {
      int x0 = page_width / 2 - input[i]->w / 2;

	{
	  struct canvas canvas, subcanvas;

	  canvas_create (&canvas, x0 + input[i]->w, input[i]->h);
	  canvas_subdivide (&subcanvas, &canvas,
			    x0, 0, x0 + input[i]->w, input[i]->h);
	  text_draw_tree (input[i], &subcanvas);
	  canvas_print (&canvas);
	  canvas_free (&canvas);

	  if (i < n - 1)
	    putchar ('\n');
	}

      tree_free (input[i]);
    }

  if (mode == MODE_TEXT)
    fputs (" \n", stdout);

cleanup:

  free (input);
  free (radius);

  return EXIT_SUCCESS;
}

/* Initialize lexeme *TOKEN. */
void
lex_init (struct token *token)
{
  token->ln = 1;
  token->eof = 0;
}

/* Stuffs C back into the input buffer if appropriate. */
void
lex_putback (FILE * memstream, int c)
{
  if (c != EOF)
    ungetc (c, memstream);
}

/* Obtains a lexeme from standard input and stores it into *TOKEN. */
void
lex_get (FILE * memstream, struct token *token)
{
  int c;

  /* Whitespace is not significant.
     Comments are introduced by #. */
  for (;;)
    {
      do {
	c = getchar_mem (memstream);
	if (c == '\n')
	  token->ln++;
      } while (isspace ((unsigned char) c));

      if (c != '#')
	break;

      do {
	c = getchar_mem (memstream);
      }	while (c != '\n' && c != EOF);
      if (c == '\n')
	token->ln++;
    }

  /* Parse a token. */
  token->c = c;
  switch (c)
    {
    case EOF:
      token->type = TOKEN_EOF;
      if (token->eof == 1)
	error (token, "unexpected end of file");
      token->eof = 1;
      break;

    case '(':
      token->type = TOKEN_LPAREN;
      break;

    case ')':
      token->type = TOKEN_RPAREN;
      break;

    case '/':
      token->type = TOKEN_LEFT;
      break;

    case '\\':
      token->type = TOKEN_RIGHT;
      break;

    case ',':
      token->type = TOKEN_COMMA;
      break;

    case '<':
      c = getchar_mem (memstream);
      if (c == '>')
	token->type = TOKEN_TWOWAY;
      break;

    case '=':
      /* = followed by > is =>.
	 Otherwise, = is a character by itself. */
      c = getchar_mem (memstream);
      if (c == '>')
	token->type = TOKEN_PRODUCES;
      else
	{
	  lex_putback (memstream, c);
	  token->type = TOKEN_NAME;
	}
      break;

    case '^':
      token->type = TOKEN_NO_CIRCLE;
      break;

    case '*':
      /* ** is TOKEN_INCORRECT; * by itself is TOKEN_LEADER. */
      c = getchar_mem (memstream);
      if (c == '*')
	token->type = TOKEN_INCORRECT;
      else
	{
	  lex_putback (memstream, c);
	  token->type = TOKEN_LEADER;
	}
      break;

    case '%':
      token->type = TOKEN_SHADE;
      break;

    case '|':
      c = getchar_mem (memstream);
      if (c == '|')
	token->type = TOKEN_ADJACENT;
      else
	{
	  lex_putback (memstream, c);
	  token->type = TOKEN_MIDDLE;
	}
      break;

    case '>':
      token->type = TOKEN_THREAD;
      break;

    case '!':
      token->type = TOKEN_NO_LINE;
      break;

    case '"':
      token->type = TOKEN_QUOTE;
      break;

    case '$':
      token->type = TOKEN_JUSTIFICATION;
      break;

    case '@':
      token->type = TOKEN_SHAPE;
      break;

    default:
      if (isprint (c))
	token->type = TOKEN_LABEL;
      else
{
// WTF
printf("bad char: %c %i\n", c, c);
//	error (token, "bad character in input");
}
      break;
    }
}

/* If *TOKEN is of type TYPE, reads the next token into *TOKEN and
   returns nonzero.  Otherwise, returns zero and takes no action. */
int
lex_match (FILE *memstream, struct token *token, enum token_type type)
{
  if (token->type == type)
    {
      lex_get (memstream,token);
      return 1;
    }
  else
    return 0;
}

/* Add character C to string pointed to by S,
   allocating space for it as well. */
void
add_char (char **s, int c)
{
  size_t len = 0;
  if (*s != NULL)
    len = strlen (*s);

  *s = xrealloc (*s, len + 2);
  (*s)[len] = c;
  (*s)[len + 1] = '\0';
}

/* Parses a single-character or quoted string name into S. */
void
lex_get_string (FILE * memstream, struct token *token, char **s)
{
  if (*s != NULL)
    error (token, "duplicate node or thread name");

  if (token->type == TOKEN_QUOTE)
    {
      lex_get (memstream,token);
      do
	{
	  if (token->type != TOKEN_LABEL)
	    error (token, "non-label character inside quoted string");
	  add_char (s, token->c);

	  lex_get (memstream,token);
	}
      while (token->type != TOKEN_QUOTE);
    }
  else if (token->type == TOKEN_LABEL)
    add_char (s, token->c);
  else
    error (token, "node or thread name expected");
}

struct tree *parse_production (FILE * memstream, struct token *token,
				      struct tree *tree1,
				      enum token_type,
				      enum output_mode);
void parse_tree_label (FILE * memstream, struct token *token, struct tree *tree,
			      enum output_mode mode);

/* Parses and returns a tree structure starting at lexeme *TOKEN.
   Tree structures are specified as follows:

     - (Optional) `*' indicates that a vertical line should be drawn
       down to the node to indicate that the tree is part of a larger
       tree.  '^' indicates that the circle normally drawn around the
       node for PostScript output should be omitted.

     - (Required) A label.  Note that, in labels, the character &
       represents newline.  For PostScript output, `/' precedes
       left-side labels, `\' precedes right-side labels, and `|'
       precedes middle labels; unprefixed label text is put in the
       middle.  For text output, `/', `\', and `|' are ignored.

       Additionally, `%' followed by `r', `g', or `b' can be included
       for shading as a red, gray, or black node, respectively.  In
       text output, the `r', `g', or `b' is copied literally to the
       output.

     - (Optional) A pair of parentheses containing a pair of tree
       structures delimited by a comma (the node's left and right
       subtrees), a single tree structure (the node's left subtree),
       or a comma followed by a single tree structure (the node's
       right subtree).  Subtree(s) not specified are considered to be
       empty.

     - (Optional) A =>, ||, or <> token followed by another tree
     structure, specifying a transformation or production rule. */
struct tree *
tree_parse (FILE  * memstream, struct token *token, enum output_mode mode)
{
  struct tree *tree = xmalloc (sizeof *tree);
  tree->type = TREE_BINARY;
  tree->options = 0;
  tree->thread[0] = tree->thread[1] = NULL;
  tree->margin[0] = tree->margin[1] = 0;
  tree->name = NULL;

  parse_tree_label (memstream, token, tree, mode);

  tree->link[0] = tree->link[1] = NULL;
  if (lex_match (memstream,token, TOKEN_LPAREN))
    {
      if (token->type != TOKEN_COMMA)
	tree->link[0] = tree_parse (memstream,token, mode);
      if (lex_match (memstream,token, TOKEN_COMMA))
	tree->link[1] = tree_parse (memstream,token, mode);
      if (!lex_match (memstream,token, TOKEN_RPAREN))
	error (token, "expecting right paren");
    }

  if (strlen (tree->label[LP_MIDDLE]) == 0
      && tree->label[LP_LEFT] == NULL
      && tree->label[LP_RIGHT] == NULL
      && tree->link[0] == NULL
      && tree->link[1] == NULL)
    error (token, "node has no label and no children");

  if (lex_match (memstream,token, TOKEN_TWOWAY))
    return parse_production (memstream,token, tree, TOKEN_TWOWAY, mode);
  else if (lex_match (memstream,token, TOKEN_PRODUCES))
    return parse_production (memstream,token, tree, TOKEN_PRODUCES, mode);
  else if (lex_match (memstream,token, TOKEN_ADJACENT))
    return parse_production (memstream,token, tree, TOKEN_ADJACENT, mode);
  else
    return tree;
}

/* Parses and returns a tree label string.  */
void
parse_tree_label (FILE  * memstream, struct token *token, struct tree *tree,
		  enum output_mode mode)
{
  enum label_position pos;
  size_t count;

  {
    int i;

    for (i = 0; i < LP_COUNT; i++)
      tree->label[i] = NULL;
  }
  tree->shade = 100;
  tree->justify = 'm';
  tree->shape = 'c';

  pos = LP_MIDDLE;
  for (count = 0;; lex_get (memstream,token), count++)
{
printf("grr: %i: %c %i [count=%i]\n", token->type, token->c, token->c, count);
    switch (token->type)
      {
      case TOKEN_INCORRECT:
	add_char (&tree->label[LP_MIDDLE], '*');
	add_char (&tree->label[LP_MIDDLE], '*');
	tree->options |= TOPT_NO_CIRCLE;
	break;
	
      case TOKEN_LEFT:
	break;

      case TOKEN_RIGHT:
	break;

      case TOKEN_MIDDLE:
	pos = LP_MIDDLE;
	break;

      case TOKEN_LABEL:
        if (token->c == '\'')
          {
            /* We used to print ' as a traditional mathematical
               prime symbol, but that was too easy to overlook.
               So now we substitute an asterisk, which is much
               more obvious. */
            add_char (&tree->label[pos], '*');
          }
        else if (token->c == ';')
          {
            if (mode == MODE_TEXT)
              add_char (&tree->label[pos], ',');
          }
        else
          add_char (&tree->label[pos], token->c);
	break;

        break;

      case TOKEN_SHADE: 
        {
          char *arg = NULL;
          lex_get (memstream,token);
          lex_get_string (memstream,token, &arg);
            {
              add_char (&tree->label[pos], '&');
              add_char (&tree->label[pos], '<');
              for (; *arg != '\0'; arg++)
                add_char (&tree->label[pos], *arg); 
              add_char (&tree->label[pos], '>');
            }
        }
        break;

      case TOKEN_JUSTIFICATION:
	lex_get (memstream,token);
	if (token->c == 't' || token->c == 'm' || token->c == 'b')
	  tree->justify = token->c;
	else
	  error (token, "expecting `t', `m', or `b'");
	break;

      case TOKEN_SHAPE:
	lex_get (memstream,token);
	if (token->c == 'c' || token->c == 's')
	  tree->shape = token->c;
	else
	  error (token, "expecting `c', `s'");
	if (tree->shape == 's' && mode == MODE_TEXT)
	  add_char (&tree->label[LP_MIDDLE], '[');
	break;

      case TOKEN_NO_CIRCLE:
	tree->options |= TOPT_NO_CIRCLE;
	break;

      case TOKEN_NO_LINE:
	tree->options |= TOPT_NO_LINE;
	break;

      case TOKEN_LEADER:
	tree->options |= TOPT_LEADER;
	break;

      case TOKEN_NAME:
	lex_get (memstream,token);
	lex_get_string (memstream,token, &tree->name);
	break;

      case TOKEN_THREAD:
	lex_get (memstream,token);
	if (token->type != TOKEN_THREAD)
	  lex_get_string (memstream,token, &tree->name);

	add_char (&tree->label[LP_MIDDLE], '[');
	if (token->c != '>')
	  {
	    const char *p;
	    for (p = tree->name; *p != '\0'; p++)
	      add_char (&tree->label[LP_MIDDLE], *p);
	  }
	add_char (&tree->label[LP_MIDDLE], ']');
	break;

      default:
	if (count == 0)
	  error (token, "expecting label");
	if (tree->shape == 's' && mode == MODE_TEXT)
	  add_char (&tree->label[LP_MIDDLE], ']');
	if (tree->label[LP_MIDDLE] == NULL)
	  add_char (&tree->label[LP_MIDDLE], '\0');
	return;
      }
}

}

/* Parses the latter part of and returns a `production' consisting of
   TREE1 followed by PROD_TYPE followed by another tree. */
struct tree *
parse_production (FILE * memstream, struct token *token,
		  struct tree *tree1, enum token_type prod_type,
		  enum output_mode mode)
{
  struct tree *tree2 = tree_parse (memstream,token, mode);
  struct tree *production = xmalloc (sizeof *production);
  production->type = TREE_PRODUCTION;
  production->prod_type = prod_type;
  production->link[0] = tree1;
  production->link[1] = tree2;
  production->name = NULL;
  return production;
}

/* Frees TREE and any subtrees. */
void
tree_free (struct tree *tree)
{
  if (tree->link[0] != NULL)
    tree_free (tree->link[0]);
  if (tree->link[1] != NULL)
    tree_free (tree->link[1]);

  free (tree->name);
  if (tree->type == TREE_BINARY)
    {
      int i;

      for (i = 0; i < LP_COUNT; i++)
	free (tree->label[i]);
    }

  free (tree);
}

/* Prints the structure of *TREE to stderr in a format acceptable to
   tree_parse().  For debugging purposes.  */

/* Calculates the size of *TREE and all its subtrees. */
void
text_measure_tree (struct tree *tree)
{
  switch (tree->type)
    {
    case TREE_BINARY:
      if (tree->link[0] != NULL)
	text_measure_tree (tree->link[0]);
      if (tree->link[1] != NULL)
	text_measure_tree (tree->link[1]);

      if (text_caret (tree))
	{
	  tree->w = 3;
	  tree->h = 3;
	  tree->ofs = 1;
	}
      else
	{
	  int subtree_height = 0;

	  text_measure_text (tree->label[LP_MIDDLE], &tree->w, &tree->h);

	  if (tree->link[0] != NULL)
	    {
	      assert (tree->link[0]->ofs >= 0
		      && tree->link[0]->ofs < tree->link[0]->w);
	      if (tree->link[0]->ofs == tree->link[0]->w - 1)
		tree->link[0] = text_shift_tree (tree->link[0], -1);
	      tree->w += tree->ofs = tree->link[0]->w;
	      subtree_height = max (subtree_height, 1 + tree->link[0]->h);
	    }
	  else
	    tree->ofs = 0;

	  if (tree->link[1] != NULL)
	    {
	      assert (tree->link[1]->ofs >= 0
		      && tree->link[1]->ofs < tree->link[1]->w);
	      if (tree->link[1]->ofs == 0)
		tree->link[1] = text_shift_tree (tree->link[1], +1);
	      tree->w += tree->link[1]->w;
	      subtree_height = max (subtree_height, 1 + tree->link[1]->h);
	    }

	  tree->h += subtree_height;
	}

      if (tree->options & TOPT_LEADER)
	tree->h++;
      break;

    case TREE_PRODUCTION:
      {
	text_measure_tree (tree->link[0]);
	text_measure_tree (tree->link[1]);
	tree->w = tree->link[0]->w + 4 + tree->link[1]->w;
	tree->h = max (tree->link[0]->h, tree->link[1]->h);
      }
      break;

    default:
      assert (0);
    }
}

/* Returns the first printable character in text string S. */
char
text_first_char (const char *s) 
{
  while (*s == '&' || *s == '_')
    s++;
  return *s;
}

/* Draws TREE into CANVAS. */
void
text_draw_tree (const struct tree *tree, struct canvas *canvas)
{
  switch (tree->type)
    {
    case TREE_BINARY:
      {
	struct canvas c;
	int x, tx, ty;

	if (tree->options & TOPT_LEADER)
	  canvas_subdivide (&c, canvas, 0, 1, canvas->w, canvas->h);
	else
	  c = *canvas;

	x = 0;
	if (text_caret (tree))
	  {
	    canvas_set (&c, 1, 0, text_first_char (tree->label[LP_MIDDLE]));
	    canvas_set (&c, 1, 1, '^');
	    canvas_set (&c, 0, 2,
                        text_first_char (tree->link[0]->label[LP_MIDDLE]));
	    canvas_set (&c, 2, 2,
                        text_first_char (tree->link[1]->label[LP_MIDDLE]));

	    if (tree->options & TOPT_LEADER)
	      canvas_set (canvas, 1, 0, '|');

	    return;
	  }

	text_measure_text (tree->label[LP_MIDDLE], &tx, &ty);
	if (tree->link[0] != NULL)
	  {
	    struct canvas subcanvas;

	    canvas_subdivide (&subcanvas, &c, x, ty + 1,
			      x + tree->link[0]->w, ty + 1 + tree->link[0]->h);
	    text_draw_tree (tree->link[0], &subcanvas);

	    x += tree->link[0]->w;
	    if (!(tree->link[0]->options & TOPT_NO_LINE))
	      text_draw_line (&c, tree->link[0]->ofs + 1, x, ty);
	  }

	if (tree->options & TOPT_LEADER)
	  canvas_set (canvas, x + tx / 2, 0, '|');

	text_draw_text (&c, x, 0, tx, tree->label[LP_MIDDLE]);
	x += tx;

	if (tree->link[1] != NULL)
	  {
	    struct canvas subcanvas;

	    canvas_subdivide (&subcanvas, &c, x, ty + 1,
			      x + tree->link[1]->w, ty + 1 + tree->link[1]->h);
	    text_draw_tree (tree->link[1], &subcanvas);

	    if (!(tree->link[1]->options & TOPT_NO_LINE))
	      text_draw_line (&c, x + tree->link[1]->ofs - 1, x - 1, ty);
	  }
      }
      break;

    case TREE_PRODUCTION:
      {
	text_draw_tree_justified (tree->link[0], canvas, 0);

	if (tree->prod_type == TOKEN_PRODUCES)
	  {
	    canvas_set (canvas, tree->link[0]->w + 1, tree->h / 2, '=');
	    canvas_set (canvas, tree->link[0]->w + 2, tree->h / 2, '>');
	  }

	text_draw_tree_justified (tree->link[1], canvas, tree->link[0]->w + 4);
      }
      break;

    case TREE_SHIFT:
      {
	struct canvas subcanvas;
	int shift;

	shift = tree->ofs - tree->link[0]->ofs;
	canvas_subdivide (&subcanvas, canvas,
			  shift, 0, shift + tree->link[0]->w, tree->h);

	text_draw_tree (tree->link[0], &subcanvas);
      }
      break;

    default:
      assert (0);
    }
}

/* Sets *W to the width and *H to the height of TEXT. */
void
text_measure_text (const char *text, int *w, int *h)
{
  int ll = 0;

  *w = 0;
  *h = 1;

  while (*text == '&' || *text == '_')
    text++;

  for (;;)
    switch (*text++)
      {
      case '\0':
	return;

      case '&':
	(*h)++;
	ll = 0;
	break;

      case '_':
        break;

      default:
        *w = max (++ll, *w);
	break;
      }
}

/* Returns a new tree the same as tree OLD but shifted to the left or
   the right by MOVE characters and filled with blanks on the opposite
   side.  If MOVE is negative OLD is shifted left, otherwise right. */
struct tree *
text_shift_tree (struct tree *old, int move)
{
  struct tree *new = xmalloc (sizeof *new);
  new->type = TREE_SHIFT;
  new->w = old->w + abs (move);
  new->h = old->h;
  new->ofs = old->ofs + (move > 0 ? move : 0);
  new->options = old->options;
  new->name = NULL;
  assert (new->ofs >= 0 && new->ofs < new->w);
  new->link[0] = old;
  new->link[1] = NULL;

  return new;
}

/* Returns nonzero iff TREE is a node with no children and a
   single-character label. */
int
text_trivial (const struct tree *tree)
{
  return (tree->w == 1
	  && tree->link[0] == NULL
	  && tree->link[1] == NULL);
}

/* Returns nonzero if TREE can be properly rendered with a caret (^),
   like so:

    a
    ^
   b c

   This is true iff TREE's two children are both leaves and all three
   nodes have single-character labels. */
int
text_caret (const struct tree *tree)
{
  return (tree->type == TREE_BINARY
	  && strlen (tree->label[LP_MIDDLE]) == 1
	  && tree->link[0] != NULL && text_trivial (tree->link[0])
	  && tree->link[1] != NULL && text_trivial (tree->link[1]));
}

/* Draws a line on CANVAS at vertical location Y horizontally from X0
   to X1, exclusive. */
void
text_draw_line (struct canvas *canvas, int x0, int x1, int y)
{
  if (x1 == x0 + 1)
    canvas_set (canvas, x0, y, '/');
  else if (x1 == x0 - 1)
    canvas_set (canvas, x0, y, '\\');
  else if (x1 > x0)
    {
      int x;

      for (x = x0; x < x1; x++)
	{
	  static const char lines[] = "_.-'";
	  size_t frac = ((double) x - x0) / (x1 - x0 - 1) * 3;
	  assert (frac < sizeof lines);
	  canvas_set (canvas, x, y, lines[frac]);
	}
    }
  else
    {
      int x;

      for (x = x0; x > x1; x--)
	{
	  static const char lines[] = "_.-`";
	  size_t frac = ((double) x0 - x) / (x0 - x1 - 1) * 3;
	  assert (frac < sizeof lines);
	  canvas_set (canvas, x, y, lines[frac]);
	}
    }
}

/* Returns the number of printing characters in line of text TEXT. */
size_t
text_line_len (const char *text)
{
  size_t len = 0;
  
  for (; *text != '\0' && *text != '&'; text++)
    if (*text != '_')
      len++;
  return len;
}

/* Draws TEXT inside CANVAS starting vertically at Y0, centered
   horizontally between X0 and X0+W. */
void
text_draw_text (struct canvas *canvas, int x0, int y0, int w, const char *text)
{
  int y = y0;

  while (*text == '&' || *text == '_')
    text++;

  while (*text != '\0')
    {
      int line_len = text_line_len (text);
      int x = x0 + w / 2 - line_len / 2;
      for (; *text != '\0' && *text != '&'; text++)
	if (*text != '_')
	  canvas_set (canvas, x++, y, *text);

      if (*text == '&' || *text == '_')
	{
	  y++;
	  while (*text == '&' || *text == '_')
	    text++;
	}
    }
}

/* Draws TREE vertically justified in CANVAS at horizontal offset X. */
void
text_draw_tree_justified (const struct tree *tree,
                          struct canvas *canvas, int x)
{
  struct canvas subcanvas;
  int ofs;

  if (tree->type == TREE_BINARY)
    {
      if (tree->justify == 'm')
	ofs = (canvas->h - tree->h) / 2;
      else if (tree->justify == 'b')
	ofs = canvas->h - tree->h;
      else
	ofs = 0;
    }
  else
    ofs = 0;

  canvas_subdivide (&subcanvas, canvas, x, ofs, x + tree->w, ofs + tree->h);
  text_draw_tree (tree, &subcanvas);
}

/* Sets up *CANVAS as a new canvas W characters wide and H characters
   tall. */
void
canvas_create (struct canvas *canvas, int w, int h)
{
  canvas->w = canvas->ll = w;
  canvas->h = h;
  canvas->buf = xmalloc (w * h);
  memset (canvas->buf, ' ', w * h);
}

/* Creates DST as a subcanvas of SRC occupying the space
   (X0,Y0)-(X1,Y1). */
void
canvas_subdivide (struct canvas *dst, const struct canvas *src,
		  int x0, int y0, int x1, int y1)
{
  assert (dst != NULL && src != NULL);
  assert (x0 >= 0 && x0 < src->w && x1 >= 0 && x1 <= src->w && x0 <= x1);
  assert (y0 >= 0 && y0 < src->h && y1 >= 0 && y1 <= src->h && y0 <= y1);

  dst->w = x1 - x0;
  dst->ll = src->ll;
  dst->h = y1 - y0;
  dst->buf = src->buf + x0 + y0 * src->ll;
}

/* Release storage associated with *CANVAS. */
void
canvas_free (struct canvas *canvas)
{
  free (canvas->buf);
}

/* Sets location (X,Y) in CANVAS to character C. */
void
canvas_set (struct canvas *canvas, int x, int y, int c)
{
  assert (canvas != NULL);
  assert (x >= 0 && x < canvas->w);
  assert (y >= 0 && y < canvas->h);
  assert (c >= 0 && c <= UCHAR_MAX);
  assert (canvas->buf[x + y * canvas->ll] == ' ');

  canvas->buf[x + y * canvas->ll] = c;
}

/* Returns the character at location (X,Y) in CANVAS. */
int
canvas_get (const struct canvas *canvas, int x, int y)
{
  assert (canvas != NULL);
  assert (x >= 0 && x < canvas->w);
  assert (y >= 0 && y < canvas->h);

  return canvas->buf[x + y * canvas->ll];
}

/* Prints the contents of CANVAS to stdout. */
void
canvas_print (const struct canvas *canvas)
{
  int y;

  for (y = 0; y < canvas->h; y++)
    {
      int x, w;

      w = canvas->w;
      while (w > 0 && canvas_get (canvas, w - 1, y) == ' ')
	w--;

for(x=0;x<w;x++) {
int z;

z = canvas_get(canvas,x,y);
dl_str_unite(&g_dl_texilib, "%c", z);


    }
dl_str_unite(&g_dl_texilib, "\n");
}

}


/* Prints error message FORMAT regarding TOKEN to stderr.  FORMAT may
   use printf()-style formatting sequences. */
void
error (struct token *token, const char *format, ...)
{
  va_list args;

  if (token == NULL)
    fputs ("error: ", stderr);
  else if (token->c == EOF)
    fputs ("error at end-of-file: ", stderr);
  else
    fprintf (stderr, "line %d: error at `%c': ", token->ln, token->c);

  va_start (args, format);
  vfprintf (stderr, format, args);
  va_end (args);

  putc ('\n', stderr);

/*
  exit (EXIT_FAILURE);
*/
longjmp(texilib_jmpenv,1);
}

/* Allocates a block of AMT bytes and returns a pointer to the
   block. */
void *
xmalloc (size_t amt)
{
  void *p;

  if (amt == 0)
    return NULL;
  p = malloc (amt);
  if (p == NULL)
    {
      fprintf (stderr, "virtual memory exhausted\n");
/*
      exit (EXIT_FAILURE);
*/
longjmp(texilib_jmpenv,1);
    }
  return p;
}

/* If SIZE is 0, then block PTR is freed and a null pointer is
   returned.
   Otherwise, if PTR is a null pointer, then a new block is allocated
   and returned.
   Otherwise, block PTR is reallocated to be SIZE bytes in size and
   the new location of the block is returned.
   Aborts if unsuccessful. */
 void *
xrealloc (void *ptr, size_t size)
{
  void *vp;
  if (!size)
    {
      if (ptr)
	free (ptr);

      return NULL;
    }

  if (ptr)
    vp = realloc (ptr, size);
  else
    vp = malloc (size);

  if (!vp)
    {
      fprintf (stderr, "virtual memory exhausted\n");
/*
      exit (EXIT_FAILURE);
*/
longjmp(texilib_jmpenv,1);
    }

  return vp;
}

/* Returns the value of the larger of its arguments. */
 int
max (int a, int b)
{
  return a > b ? a : b;
}



int getchar_mem(FILE * memstream) {

int c;

c = fgetc(memstream);

if(c == 0) return EOF;

return c;
}
