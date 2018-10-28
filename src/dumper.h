#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <arpa/inet.h>
#include <unistd.h>

void dump_memory_region(FILE* pMemFile, unsigned long start_address, long length, int serverSocket);