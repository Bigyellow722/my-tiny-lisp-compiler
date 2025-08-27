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

  FILE* src;
  ssize_t line_sz = 0;
  char* line_buf = NULL;
  size_t line_caps = 0;
  size_t cur_line_num = 0;
  int res = 0;
  lexer* g_lexer = lexer_make();
  if (!g_lexer) {
    fprintf(stderr, "failed to create the global lexer\n");
    return 0;
  }
  src = fopen(argv[optind], "r");
  if (!src) {
    fprintf(stderr, "failed to open %s, error msg:%s\n", argv[optind], strerror(errno));
    lexer_free(&g_lexer);
    return 0;
  }
  while ((line_sz = getline(&line_buf, &line_caps, src)) != -1) {
    cur_line_num++;
    printf("line(%ld) size: %d %s", cur_line_num, (int)line_sz, line_buf);
    res = lexer_tokenize_line(g_lexer, line_buf, line_sz, cur_line_num);
  }

  lexer_dump_tokens(g_lexer);
  free(line_buf);
  lexer_free(&g_lexer);
  return 0;
}
