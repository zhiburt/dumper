#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <string.h>

void dump_memory_region(FILE* pMemFile, unsigned long start_address, long length);
void writeTo(char* page, int pageLength);
char* getRegionName(unsigned int pid,unsigned long start_address, long length);
char* getLastWord(const char* line);

//hexdumper
void dumpCanonical(char* page, int pageLength);
int CompareTwoBytes(const char* page, int start1, int start2);