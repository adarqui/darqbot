#ifndef TEXILIB_H
#define TEXILIB_H

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

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bot.h"

/* FIXME?  In text-based trees we should perhaps make sure that
   all of the nodes at a given level have the same height.  See,
   e.g., avlins2.tree for an example of where this might be
   nice. */

/* TODO: In text mode break too-long lines.  Useful for
   vine2tree2.txt. */

/* TODO: Should be able to support k-way trees, perhaps with a special #k
   directive.  Then we can talk about 2-3-4 trees in the RB chapter. */

/* FIXME: Needs to be more or less rewritten to fix the cruft. */

/* FIXME: Arrows between trees not properly aligned with $t.  Example:
   rootins.tree. */

/* FIXME: Handle loopback threads; i.e., a=a(>a). */

/* FIXME: Need to re-encode output font in order to get access to
   minus sign instead of hyphen.  Could use ISOLatin1Encoding I
   suppose. */

/* FIXME: Do real fitting of a circle to a set of boxes:
	_____
     __|_____|__
    |___________|
        |___|

   Possible strategy: find the center of the set of lines, then find
   the 3 box corner points that are farthest away from the center, and
   fit a circle to these three points. */

/* FIXME: Ellipses (...) should be made prettier in PostScript
   output. */

/* A PostScript-formatted binary tree node contains the following,
   from top down:

   - If TOPT_LEADER is set, a leader one radius in height.

   - The node itself.  If TOPT_NO_CIRCLE is unset, this is two radii
     in height.  If TOPT_NO_CIRCLE is set, then it is the actual
     height of the text plus one radius above.

   - A gutter between the node and its children.  This contains any
     lines between the node and its children.  It is missing if the
     node has no children.

   - The node's children, if any, having the height of the taller of
     its children.  The node's children are formatted recursively like
     the node itself.

   From left to right:

   - A gutter to accommodate long left-side labels, in top-level
     trees, of width margin[0].

   - If the node has an outside left thread (denoted >> in source
     form) then a left thread gutter of width PS_THREAD_WIDTH.

   - If the node has a left child, its left child, with width
     link[0]->w.

   - The node itself If TOPT_NO_CIRCLE is unset, this is two radii in
     width.  If TOPT_NO_CIRCLE is set, it is the text's actual width
     plus twice PS_MARGIN (one on its left, one on its right).

   - If the node has a right child, its right child, with width
     link[1]->w.

   - If the node has an outside right thread (denoted >> in source
     form) then a right thread gutter of width PS_THREAD_WIDTH.

   - A gutter to accommodate long right-side labels, in top-level
     trees, of width margin[1].

   A production node contains the following, from left to right:

   - The left side of the production, of width link[0]->w.

   - The production operator, of width 4 * PS_CHAR_WIDTH.

   - The right side of the production, of width link[1]->w. */

/* Recent GNU C versions support declaring certain function
   attributes.  Allow less cool compilers to ignore these
   attributes. */
#ifdef __GNUC__
#define ATTRIBUTE(X) __attribute__ (X)
#else
#define ATTRIBUTE(X)
#endif

/* Font metrics. */
struct font
  {
    struct character *chars;
    unsigned char *map;
    struct kern_pair *kern;
  };

/* Metrics for one character. */
struct character
  {
    int wx;			/* Width. */
    int llx, lly, urx, ury;	/* Bounding box. */
  };

/* Kerning information for a character pair. */
struct kern_pair
  {
    unsigned char c0, c1;	/* Characters. */
    int amt;			/* Kern adjust. */
  };

/*
#include "helvetica.inc"
#define font helv_font
*/

/* Output mode. */
enum output_mode
  {
    MODE_TEXT,			/* Textual output. */
  };

/* Token types. */
enum token_type
  {
    TOKEN_EOF,			/* End of file. */
    TOKEN_LPAREN,		/* `(', `{', `<'. */
    TOKEN_RPAREN,		/* `)', `}', `>'. */
    TOKEN_LEFT,			/* `/'. */
    TOKEN_MIDDLE,		/* `|'. */
    TOKEN_RIGHT,		/* `\'. */
    TOKEN_COMMA,		/* `,'. */
    TOKEN_PRODUCES,		/* `=>'. */
    TOKEN_TWOWAY,		/* `<>'. */
    TOKEN_ADJACENT,		/* `||'. */
    TOKEN_NO_CIRCLE,		/* `^'. */
    TOKEN_LEADER,		/* `*'. */
    TOKEN_SHADE,		/* `%'. */
    TOKEN_THREAD,		/* `>'. */
    TOKEN_NAME,			/* `='. */
    TOKEN_NO_LINE,		/* `!'. */
    TOKEN_QUOTE,		/* `"'. */
    TOKEN_INCORRECT,		/* `**'. */
    TOKEN_JUSTIFICATION,	/* `$'. */
    TOKEN_SHAPE,		/* `@'. */
    TOKEN_LABEL			/* Anything else. */
  };

/* Current token. */
struct token
  {
    int ln;			/* Tracks input line number. */
    enum token_type type;	/* Token type. */
    int c;			/* Character corresponding to token. */
    int eof;			/* 1=Reached end of file. */
  };

/* Tree types. */
enum tree_type
  {
    TREE_BINARY,		/* Ordinary binary tree. */
    TREE_PRODUCTION,		/* Two binary trees separated by =>. */
    TREE_SHIFT			/* Binary tree shifted left or right. */
  };

/* Binary tree options. */
enum tree_options
  {
    TOPT_LEADER = 001,		/* Line leads in from top. */
    TOPT_NO_CIRCLE = 002,	/* Omit circle around node. */
    TOPT_THREAD = 004,		/* This is a thread, not a real node. */
    TOPT_NO_LINE = 010		/* Don't draw line from parent node. */
  };

/* Label positions. */
enum label_position
  {
    LP_MIDDLE,			/* Inside the node circle. */
    LP_LEFT,			/* (PostScript only) Left of node circle. */
    LP_RIGHT,			/* (PostScript only) Right of node circle. */
    LP_COUNT
  };

/* A binary tree or production. */
struct tree
  {
    enum tree_type type;	/* Tree type. */
    int w, h;			/* Width, height in characters or units. */
    double x, y;		/* (PostScript only) Absolute x,y of circle. */

    /* TREE_BINARY or TREE_SHIFT. */
    int ofs;			/* Relative x coordinate of label's center. */

    /* TREE_BINARY. */
    enum tree_options options;	/* Options. */
    char *label[LP_COUNT];	/* Labels for the root of this tree. */
    struct tree *link[2];	/* Links to subtrees. */
    struct tree *thread[2];	/* Thread links. */
    int shade;			/* Percentage shading, 0=black, 100=white. */
    char *name;			/* Node name. */
    int margin[2];		/* Left and right margins for label space. */
    int justify;		/* Justification: 't', 'm', 'b'. */
    int shape;			/* 'c' for circle or 's' for square. */

    /* TREE_PRODUCTION. */
    enum token_type prod_type;	/* TOKEN_PRODUCES, TOKEN_ADJACENT,
				   TOKEN_TWOWAY. */
  };

/* A text art canvas. */
struct canvas
  {
    int w;			/* Width in characters. */
    int ll;			/* Line length (may be greater than width). */
    int h;			/* Height in characters. */
    unsigned char *buf;		/* Content. */
  };

char * texitree_str(char *);
int getchar_mem(FILE *) ;
dlist_t * g_dl_texilib;
jmp_buf texilib_jmpenv;

void lex_init (struct token *);
void lex_get (FILE *, struct token *);
int lex_match (FILE *, struct token *token, enum token_type type);
void lex_get_string (FILE *, struct token *token, char **s);

struct tree *tree_parse (FILE *, struct token *, enum output_mode);
void tree_free (struct tree *tree);
//utatic void tree_print (FILE *, struct tree *);

void text_measure_tree (struct tree *);
void text_draw_tree (const struct tree *, struct canvas *);
void text_measure_text (const char *text, int *w, int *h);
struct tree *text_shift_tree (struct tree *old, int direction);
int text_trivial (const struct tree *tree);
int text_caret (const struct tree *tree);
void text_draw_line (struct canvas *, int x0, int x1, int y);
void text_draw_text (struct canvas *canvas, int x, int y, int w,
			    const char *text);
void text_draw_tree_justified (const struct tree *tree,
				      struct canvas *canvas, int x);

void canvas_create (struct canvas *, int w, int h);
void canvas_free (struct canvas *);
int canvas_get (const struct canvas *canvas, int x, int y);
 void canvas_set (struct canvas *, int x, int y, int c);
void canvas_subdivide (struct canvas *dst, const struct canvas *src,
			      int x0, int y0, int x1, int y1);
void canvas_print (const struct canvas *);

 void error (struct token *, const char *, ...)
     ATTRIBUTE ((format (printf, 2, 3)));
void *xmalloc (size_t);
void *xrealloc (void *ptr, size_t size);
int max (int a, int b);


#endif
