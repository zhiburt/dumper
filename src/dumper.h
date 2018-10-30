#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <string.h>

void dump_memory_region(FILE* pMemFile, unsigned long start_address, long length);
void writeTo(char* page, int pageLength);
char* getRegionName(unsigned int pid,unsigned long start_address, long length);
char* getRName(const char *str);
char* getLastWord(const char* line);

//hexdumper
void dumpCanonical(char* page, int pageLength);
void dumpCanonicalWithRealAdresses(char* page, int pageLength, unsigned long start);
int CompareTwoBytes(const char* page, int start1, int start2);
void getLongAddress(const char* line, unsigned long *x1, unsigned long *x2);
void getShortAddress(const char* line, unsigned long *x1, unsigned long *x2);