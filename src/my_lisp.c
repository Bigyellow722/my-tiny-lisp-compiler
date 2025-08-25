#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>

#include "lexer.h"

#define _PROJ_NAME_ "my-lisp"
#define _VERSION_ "0.1.0"

#define LINE_BUFFER_SIZE 1024
/* global variables in getopt */
/* extern char *optarg; */
/* extern int optind, opterr, optopt; */

/* int getopt_long(int argc, char *argv[], */
/*                   const char *optstring, */
/*                   const struct option *longopts, int *longindex); */

/* struct option { */
/*   const char *name; */
/*   int         has_arg; */
/*   int        *flag; */
/*   int         val; */
/* }; */

static struct option opts[] = {
  {"help", no_argument, 0, 'h'},
  {"version", no_argument, 0, 'V'},
  {0,0,0,0}
};

void print_version()
{
  printf("%s%s\n", _PROJ_NAME_, _VERSION_);
}

void usage(const char* name)
{
  printf("usage: %s [option] [file]\n", name);
  printf("options:\n");
  printf("\t-v, --version\t show the version\n");
  printf("\t-h, --help\t show the usage\n");
}

int main(int argc, char **argv) {
  int c;
  while ((c = getopt_long(argc, argv, "hV", opts, NULL)) != -1) {
    switch (c) {
    case 'V':
      print_version();
      return 0;
    case 'h':
    default:
      usage(argv[0]);
      return 0;
    }
  }
  if (optind >= argc) {
    printf("lisp> \n");
    return 0;
  }
  printf("%d: %s\n", optind, argv[optind]);

  ssize_t line_sz = 0;
  lexer* g_lexer = lexer_make(argv[optind]);
  if (!g_lexer) {
    fprintf(stderr, "failed to create the global lexer\n");
    lexer_free(&g_lexer);
    return 0;
  }
  while ((line_sz = getline(&g_lexer->line_buf, &g_lexer->line_caps, g_lexer->src)) != -1) {
    printf("size: %d %s", (int)line_sz, g_lexer->line_buf);
  }

  lexer_free(&g_lexer);
  return 0;
}
