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
#ifndef MOD_SP_H
#define MOD_SP_H

#include "bot.h"
#include <curl/curl.h>


enum sp_opts
{
  OPT_TAGS = 0x01,
  OPT_DEPS = 0x02,
  OPT_TREE = 0x04,
  OPT_INFO = 0x08,
};

module_t mod_sp_info;

bot_t *sp_init (dlist_t *, bot_t *);
bot_t *sp_fini (dlist_t *, bot_t *);
bot_t *sp_help (dlist_t *, bot_t *);
bot_t *sp_run (dlist_t *, bot_t *);

void sp_fill_info (bot_t *);
int sp_run_on (bot_t *);
int sp_run_off (bot_t *);


typedef struct sp_info
{
  int initialized;

  pid_t ison;

  FILE *fp;

  int argc;
  char **argv;
  char *dir;

  int pipefd[2];
  int pipefd_b[2];

  int should_on;

}
sp_info_t;

sp_info_t sp_info;



char *sp_change_string (bot_t *, char *, int);
bot_t *sp_run_info (bot_t *, char *);









typedef struct penn_meanings
{
  char *abrv;
  char *meaning;
  char *example;
} penn_meanings_t;


penn_meanings_t penn_meanings[100] = {
  {"$", "dollar", "$ -$ --$ A$ C$ HK$ M$ NZ$ S$ U.S.$ US$"},
  {"``", "opening quotation mark", "``"},

  {"''", "closing quotation mark", "''"},
  {"(", "opening parenthesis", "( [ {"},
  {")", "closing parenthesis", ") ] }"},
  {",", "comma", ","},
  {"--", "dash", "--"},
  {".", "sentence terminator", ". ! ?"},
  {":", "colon or ellipsis", ": ; ..."},
  {"CC", "conjunction, coordinating",
   "& 'n and both but either et for less minus neither nor or plus so therefore times v. versus vs. whether yet"},
  {"CD", "numeral, cardinal",
   "mid-1890 nine-thirty forty-two one-tenth ten million 0.5 one forty-seven 1987 twenty '79 zero two 78-degrees eighty-four IX '60s .025 fifteen 271,124 dozen quintillion DM2,000 ..."},
  {"DT", "determiner",
   "all an another any both del each either every half la many much nary neither no some such that the them these this those"},
  {"EX", "existential there", "there"},
  {"FW", "foreign word",
   "gemeinschaft hund ich jeux habeas Haementeria Herr K'ang-si vous lutihaw alai je jour objets salutaris fille quibusdam pas trop Monte terram fiche oui corporis ..."},
  {"IN", "preposition or conjunction, subordinating",
   "astride among uppon whether out inside pro despite on by throughout below within for towards near behind atop around if like until below next into if beside ..."},
  {"JJ", "adjective or numeral, ordinal",
   "third ill-mannered pre-war regrettable oiled calamitous first separable ectoplasmic battery-powered participatory fourth still-to-be-named multilingual multi-disciplinary ..."},
  {"JJR", "adjective, comparitive",
   "bleaker braver breezier briefer brighter brisker broader bumper busier calmer cheaper choosier cleaner clearer closer colder commoner costlier cozier creamier crunchier cuter ..."},
  {"JJS",
   "calmest cheapest choicest classiest cleanest clearest closest commonest corniest costliest crassest creepiest crudest cutest darkest deadliest dearest deepest densest dinkiest ..."},
  {"LS", "list item marker",
   "A A. B B. C C. D E F First G H I J K One SP-44001 SP-44002 SP-44005 SP-44007 Second Third Three Two * a b c d first five four one six three two"},
  {"MD", "modal auxiliary",
   "can cannot could couldn't dare may might must need ought shall should shouldn't will would"},
  {"NN", "noun, common, singular or mass",
   "common-carrier cabbage knuckle-duster Casino afghan shed thermostat investment slide humour falloff slick wind hyena override subhumanity machinist ..."},
  {"NNP", "noun, proper, singular",
   "Motown Venneboerger Czestochwa Ranzer Conchita Trumplane Christos Oceanside Escobar Kreisler Sawyer Cougar Yvette Ervin ODI Darryl CTCA Shannon A.K.C. Meltex Liverpool ..."},
  {"NNPS", "noun, proper, plural",
   "Americans Americas Amharas Amityvilles Amusements Anarcho-Syndicalists Andalusians Andes Andruses Angels Animals Anthony Antilles Antiques Apache Apaches Apocrypha ..."},
  {"NNS", "noun, common, plural",
   "undergraduates scotches bric-a-brac products bodyguards facets coasts divestitures storehouses designs clubs fragrances averages subjectivists apprehensions muses factory-jobs ..."},
  {"PDT", "pre-determiner", "all both half many quite such sure this"},
  {"POS", "genitive marker", "' 's"},
  {"PRP", "pronoun, personal",
   "hers herself him himself hisself it itself me myself one oneself ours ourselves ownself self she thee theirs them themselves they thou thy us"},
  {"PRP$", "pronoun, possessive", "her his mine my our ours their thy your"},
  {"RB", "adverb",
   "occasionally unabatingly maddeningly adventurously professedly stirringly prominently technologically magisterially predominately swiftly fiscally pitilessly ..."},
  {"RBR", "adverb, comparative",
   "further gloomier grander graver greater grimmer harder harsher healthier heavier higher however larger later leaner lengthier less-perfectly lesser lonelier longer louder lower more ..."},
  {"RBS", "adverb, superlative",
   "best biggest bluntest earliest farthest first furthest hardest heartiest highest largest least less most nearest second tightest worst"},
  {"RP", "particle",
   "| aboard about across along apart around aside at away back before behind by crop down ever fast for forth from go high i.e. in into just later low more off on open out over per pie raising start teeth that through under unto up up-pp upon whole with you"},
  {"SYM", "symbol",
   "% & ' '' ''. ) ). * + ,. < = > @ A[fj] U.S U.S.S.R * ** ***"},
  {"TO", "\"to\" as preposition or infinitive to", "to"},
  {"UH", "interjection",
   "Goodbye Goody Gosh Wow Jeepers Jee-sus Hubba Hey Kee-reist Oops amen huh howdy uh dammit whammo shucks heck anyways whodunnit honey golly man baby diddle hush sonuvabitch ..."},
  {"VB", "verb, base form",
   "ask assemble assess assign assume atone attention avoid bake balkanize bank begin behold believe bend benefit bevel beware bless boil bomb boost brace break bring broil brush build ..."},
  {"VBD", "verb, past tense",
   "dipped pleaded swiped regummed soaked tidied convened halted registered cushioned exacted snubbed strode aimed adopted belied figgered speculated wore appreciated contemplated ..."},
  {"VBG", "verb, present participle or gerund",
   "telegraphing stirring focusing angering judging stalling lactating hankerin' alleging veering capping approaching traveling besieging encrypting interrupting erasing wincing ..."},
  {"VBN", "verb, past participle",
   "multihulled dilapidated aerosolized chaired languished panelized used experimented flourished imitated reunifed factored condensed sheared unsettled primed dubbed desired ..."},
  {"VBP", "verb, present tense, not 3rd, person singular",
   "predominate wrap resort sue twist spill cure lengthen brush terminateappear tend stray glisten obtain comprise detest tease attract emphasize mold postpone sever return wag ..."},
  {"VBZ", "verb, present tense, 3rd person, singular",
   "bases reconstructs marks mixes displeases seals carps weaves snatches slumps stretches authorizes smolders pictures emerges stockpiles seduces fizzes uses bolsters slaps speaks pleads ..."},
  {"WDT", "WH-determiner", "that what whatever which whichever"},
  {"WP", "WH-pronoun",
   "that what whatever whatsoever which who whom whosoever"},
  {"WP$", "WH-pronoun, possessive", "whose"},
  {"WRB", "Wh-adverb",
   "how however whence whenever where whereby whereever wherein whereof why"},
  {NULL, NULL, NULL}
};


size_t sp_curl_write (void *, size_t, size_t, void *);
void __sp_init__ (void) __attribute__ ((constructor));

#endif
