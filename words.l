%{
        /* An example program from lex & yacc by John R. Levine. I added a
         * minimalistic query feature. */

    enum {
        LOOKUP = 0,             /* Default is to just lookup */
        VERB,
        ADJ,
        ADV,
        NOUN,
        PREP,
        PRON,
        CONJ,
        LIST
    };

    int status;

    /* Datastructure for word */
    typedef struct wordtab {
        int             type;
        char            *word;
        struct wordtab  *next;
    } wordtab_t;

    /* Last created structure is the first */
    wordtab_t *head = NULL;
    char *type = NULL;

    int lookup_word(char *word);
    int add_word(int type, char *word);
    void list_types(char *type);
%}
%%

[\t .]+         ;                   /* Do nothing for tabs */
\n              { status = LOOKUP; }
\?              { status = LIST;   }

^verb           { status = VERB;   }
^adj            { status = ADJ;    }
^adv            { status = ADV;    }
^noun           { status = NOUN;   }
^prep           { status = PREP;   }
^pron           { status = PRON;   }
^conj           { status = CONJ;   }

[a-zA-Z]+       {
    if (status == LOOKUP)  {
        switch(lookup_word(yytext)) {
            case VERB:
                printf("%s is a verb.\n", yytext);
                break;
            case ADJ:
                printf("%s is an adjective.\n", yytext);
                break;
            case ADV:
                printf("%s is an adverb.\n", yytext);
                break;
            case NOUN:
                printf("%s is a noun.\n", yytext);
                break;
            case PREP:
                printf("%s is a preposition.\n", yytext);
                break;
            case PRON:
                printf("%s is a pronoun.\n", yytext);
                break;
            case CONJ:
                printf("%s is a conjugate.\n", yytext);
                break;
            default:
                printf("%s not recorded in any of the category.\n", yytext);
                break;
        }
    } else if (status == LIST) {
        list_types(yytext);
    } else {
        add_word(status, yytext);
    }
}

%%

int
main()
{
    yylex();
}

void *malloc();

int
add_word(int type, char *word)
{
    wordtab_t *new_word;

    if (lookup_word(word) != LOOKUP) {
        printf("Word already exists in the list.\n");
        return(0);              /* This return value is useless */
    }

    /* Word doesn't exist let's add it
     *          typedef struct wordtab {
     *                  int             type;
     *                  char            *word;
     *                  struct wordtab  *next;
     *          } wordtab_t;
     */

    new_word = (wordtab_t *)malloc(sizeof(wordtab_t));
    new_word->type = type;
    new_word->word = (char *)malloc(sizeof(word)+1);
    strcpy(new_word->word, word);
    new_word->next = head;
    head = new_word;

    return 0;
}

int
lookup_word(char *word)
{
    wordtab_t *lword = head;

    for (; lword; lword = lword->next) {
        if (strcmp(word, lword->word) == 0)
            return (lword->type);
    }

    return LOOKUP;
}

void
list_types(char *type)
{
    wordtab_t *list = head;
    int listype = 0;

        /* Build a table of known types. This is quite errorprone as this has
         * to be in sync with the global enum */

    if (strcmp(type, "verb") == 0)
        listype = 1;
    else if (strcmp(type, "adj") == 0)
        listype = 2;
    else if (strcmp(type, "adv") == 0)
        listype = 3;
    else if (strcmp(type, "noun") == 0)
        listype = 4;
    else if (strcmp(type, "prep") == 0)
        listype = 5;
    else if (strcmp(type, "pron") == 0)
        listype = 6;
    else if (strcmp(type, "conj") == 0)
        listype = 7;
    else {
        printf("Unknown type\n");
        return;
    }

    for (; list; list = list->next) {
        if (list->type == listype)
            printf("%s  ", list->word);
    }
    printf("\n");
}
