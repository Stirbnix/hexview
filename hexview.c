/*******************************************************************************
 * File  : hexview.c
 * Author: Andreas Gollsch <a.gollsch@freenet.de>
 * Date  : 13.04.2018
 * Brief : A simple hexview tool
 ******************************************************************************/

#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>

#define CHUNK_SIZE  (16)

static char *prog_name;

void print_usage()
{
  fprintf(stderr, "Usage  : %s <option> | <file name>\n", prog_name);
  fprintf(stderr, "Example: %s any-file\n", prog_name);
  fprintf(stderr, "Options:\n");
  fprintf(stderr, " -h --help    show this message\n");
  fprintf(stderr, " -v --version show version\n");
  fprintf(stderr, " -            use stdin\n");
  exit(EXIT_FAILURE);
}

void print_help()
{
  printf("%s Copyright (C) 2018 Andreas Gollsch <a.gollsch@freenet.de>\n",
         prog_name);
  printf("This program comes with ABSOLUTELY NO WARRANTY.\n");
  printf("This is free software, and you are welcome to redistribute it\n");
  printf("under certain conditions.\n");
  print_usage();
}

void print_version()
{
  printf("%s version 1.1\n", prog_name);
  exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
  unsigned char buf[CHUNK_SIZE];
  int fd, b_read, addr = 0;

  prog_name = basename(argv[0]);
  if(argc < 2)
  {
    fprintf(stderr, "%s: too few arguments\n", prog_name);
    print_usage();
  }
  if(*argv[1] == '-')
  {
    if(!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version"))
      print_version();
    else if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
      print_help();
    else if(!strcmp(argv[1], "-"))
      fd = 0;
    else
    {
      fprintf(stderr, "%s: Unknown option %s\n", prog_name, argv[1]);
      exit(EXIT_FAILURE);
    }
  }
  else if((fd = open(argv[1], O_RDONLY)) == -1)
  {
    fprintf(stderr, "%s: Can't open file %s\n", prog_name, argv[1]);
    exit(EXIT_FAILURE);
  }
  while((b_read = read(fd, buf, CHUNK_SIZE)) > 0)
  {
    printf("%06X  ", addr);
    for(int i = 0; i < b_read; i++)
      printf("%02X ", buf[i]);
    printf(" ");
    for(int i = 0; i < (CHUNK_SIZE - b_read); i++)
      printf("   ");
    for(int i = 0; i < b_read; i++)
    {
      if(isprint(buf[i]))
        printf("%c", buf[i]);
      else
        printf(".");
    }
    printf("\n");
    addr += b_read;
  }
  printf("%06X\n", addr);
  close(fd);

  return EXIT_SUCCESS;
}
