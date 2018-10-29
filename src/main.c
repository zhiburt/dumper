#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/ptrace.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "./dumper.h"

int main(int argc, char **argv) {

    if (argc == 2)
    {
        int pid = atoi(argv[1]);
        long ptraceResult = ptrace(PTRACE_ATTACH, pid, NULL, NULL);
        if (ptraceResult < 0)
        {
            printf("Unable to attach to the pid specified\n");
            return EXIT_FAILURE;
        }
        // wait(NULL);
        waitpid(pid, NULL, 0);

        char mapsFilename[1024];
        sprintf(mapsFilename, "/proc/%s/maps", argv[1]);
        FILE* pMapsFile = fopen(mapsFilename, "r");
        char memFilename[1024];
        sprintf(memFilename, "/proc/%s/mem", argv[1]);
        FILE* pMemFile = fopen(memFilename, "r");
        
        int sizeLine = 256;
        char line[sizeLine];
        while (fgets(line, sizeLine, pMapsFile) != NULL)
        {

            unsigned long start_address;
            unsigned long end_address;
            // sscanf(line, "%08lx-%08lx\n", &start_address, &end_address);
            // sscanf(line, "%016lx-%016lx\n", &start_address, &end_address);
            getLongAddress(line, &start_address, &end_address);
            char *name = NULL;
            name = getRegionName(pid, start_address, end_address - start_address);
            if (name != NULL){
                printf("\nregion = %s\n", name);
                free(name);
            }else{
                printf("\nregion = %s\n", "anonymous region");
            }

            dump_memory_region(pMemFile, start_address, end_address - start_address);
            
        }
        fclose(pMapsFile);
        fclose(pMemFile);
        
        ptrace(PTRACE_CONT, pid, NULL, NULL);
        ptrace(PTRACE_DETACH, pid, NULL, NULL);
    }
    else
    {
        printf("%s <pid>\n", argv[0]);
        printf("%s <pid> <ip-address> <port>\n", argv[0]);
        exit(0);
    }
}