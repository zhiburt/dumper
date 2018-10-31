#include "dumper.h"

//we read here some region by pages
void dump_memory_region(FILE* pMemFile, unsigned long start_address, long length, int realAdress)
{
    unsigned long address;
    int pageLength = getpagesize(); // int pageLength = 4096;
    unsigned char page[pageLength];
    fseeko(pMemFile, start_address, SEEK_SET);//move pointer of file (carriage) to the start of the region 
    
    for (address=start_address; address < start_address + length; address += pageLength)
    {
        fread(&page, 1, pageLength, pMemFile);
        //writeTo(page, pageLength);
        if (realAdress == 0){
            dumpCanonicalWithRealAdresses(page, pageLength, address);
        }else{
            dumpCanonical(page, pageLength);
        }
    }
}

char* dumpMRegion(FILE* pMemFile, unsigned long start_address, long length, int realAdress){
    unsigned long size = ((unsigned long)length * 10);
    char *resp = malloc(size * 2);
    int x = realAdress;
    if (realAdress != 0){
        x = -1;
    }
    unsigned long address;
    int pageLength = getpagesize(); // int pageLength = 4096;
    unsigned char page[pageLength];
    fseeko(pMemFile, start_address, SEEK_SET);//move pointer of file (carriage) to the start of the region 
    
    for (address=start_address; address < start_address + length; address += pageLength)
    {
        fread(&page, 1, pageLength, pMemFile);
        char *can;
        if (x == -1){
            can = dumpCanon(page, pageLength);
        }else{
            can = dumpCanonR(page, pageLength, address);
        }
        resp = strcat(resp, can);

    }
    
    return resp;
}



void writeTo(char* page, int pageLength){
    fwrite(page, 1, pageLength, stdout);
}

char* getRegionName(unsigned int pid,unsigned long start_address, long length){
    char mapsFilename[1024];
    sprintf(mapsFilename, "/proc/%d/maps", pid);
    FILE* pMapsFile = fopen(mapsFilename, "r");

    int sizeLine = 256;
    char line[sizeLine];
    while (fgets(line, sizeLine, pMapsFile) != NULL)
    {
        unsigned long sa;
        unsigned long ea;
        getLongAddress(line, &sa, &ea);
        if (start_address == sa && ea == start_address + length){
            fclose(pMapsFile);
            char* last = NULL;
            last = getLastWord(line);
            return last;
        }
    }
    fclose(pMapsFile);
    return "invalid_";
}

char* getRName(const char *str){
    char* last = NULL;
    last = getLastWord(str);
    return last;
}

char* getLastWord(const char* line){
    char *last_tok = malloc(sizeof(char) * 100);
    int i = 0;
        char *tok = strtok(line, " \t\n");
        while (tok) {
            strcpy(last_tok, tok);
            tok = strtok(NULL, " \t\n");
            i++;
        }
        if (i <= 5){
            // free(last_tok);
            return NULL;
        }

    return last_tok;
}

void getLongAddress(const char* line, unsigned long *x1, unsigned long *x2){
    sscanf(line, "%016lx-%016lx\n", x1, x2);
}


void getShortAddress(const char* line, unsigned long *x1, unsigned long *x2){
    sscanf(line, "%08lx-%08lx\n", x1, x2);
}


//DUMP

void dumpCanonical(char* page, int pageLength){
    char ascii[17]; // last column
	size_t i, j;
	ascii[16] = '\0'; 
    int iter = 0x00000000; // iterator first column
    int last_star_or_not = 1; // bool flag
	for (i = 0; i < pageLength; ++i) {

        if (i % 16 == 0){
            if (CompareTwoBytes(page, i - 16, i) == 0 && i >= 16){ // check on first row for the some out like hexdump -C (cannonical)
                iter += 16;

                if (last_star_or_not != 0){
                    printf("*\n");
                }
            
                i += 15; // continue ++;
                
                last_star_or_not = 0;  
                continue;

            }

            printf("%08x  ", iter);
            iter += 16;

            last_star_or_not = 1; 
        }
        
        printf("%02X ", ((unsigned char*)page)[i]);
		if (((unsigned char*)page)[i] >= ' ' && ((unsigned char*)page)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)page)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == pageLength) {
			printf(" ");
			if ((i+1) % 16 == 0) {
				printf("|%s|\n", ascii);
			} else if (i+1 == pageLength) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					printf(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					printf("   ");
				}
				printf("|%s|\n", ascii);
			}
		}
    }
}

void dumpCanonicalWithRealAdresses(char* page, int pageLength, unsigned long start){
    char ascii[17]; // last column
	size_t i, j;
	ascii[16] = '\0'; 
    int iter = start; // iterator first column
    int last_star_or_not = 1; // bool flag
	for (i = 0; i < pageLength; ++i) {

        if (i % 16 == 0){
            if (CompareTwoBytes(page, i - 16, i) == 0 && i >= 16){ // check on first row for the some out like hexdump -C (cannonical)
                iter += 16;

                if (last_star_or_not != 0){
                    printf("*\n");
                }
            
                i += 15; // continue ++;
                
                last_star_or_not = 0;  
                continue;

            }

            printf("%08x  ", iter);
            iter += 16;

            last_star_or_not = 1; 
        }
        
        printf("%02X ", ((unsigned char*)page)[i]);
		if (((unsigned char*)page)[i] >= ' ' && ((unsigned char*)page)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)page)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == pageLength) {
			printf(" ");
			if ((i+1) % 16 == 0) {
				printf("|%s|\n", ascii);
			} else if (i+1 == pageLength) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					printf(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					printf("   ");
				}
				printf("|%s|\n", ascii);
			}
		}
    }
}

char *dumpCanonR(char* page, unsigned long pageLength, unsigned long start){
    int size = pageLength * 10;
    char* resp = malloc(size);

    char* l = malloc(pageLength / 4);

    char ascii[17]; // last column
	size_t i, j;
	ascii[16] = '\0'; 
    
    int iter = start; // iterator first column

    int last_star_or_not = 1; // bool flag
	for (i = 0; i < pageLength; ++i) {

        if (i % 16 == 0){
            if (CompareTwoBytes(page, i - 16, i) == 0 && i >= 16){ // check on first row for the some out like hexdump -C (cannonical)
                iter += 16;

                if (last_star_or_not != 0){
                    resp = strcat(resp, "*\n");

                }
            
                i += 15; // continue ++;
                
                last_star_or_not = 0;  
                continue;

            }
            sprintf(l,"%08x  ", iter);
            resp = strcat(resp, l);

            iter += 16;

            last_star_or_not = 1; 
        }
        
        sprintf(l,"%02X ", ((unsigned char*)page)[i]);
        resp = strcat(resp, l);
		
        if (((unsigned char*)page)[i] >= ' ' && ((unsigned char*)page)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)page)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == pageLength) {
            resp = strcat(resp, " ");
		
			if ((i+1) % 16 == 0) {
				sprintf(l, "|%s|\n", ascii);
                resp = strcat(resp, l);
            } else if (i+1 == pageLength) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
                    resp = strcat(resp, " ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
                    resp = strcat(resp, "   ");
				}
				sprintf(l, "|%s|\n", ascii);
                resp = strcat(resp, l);
			}
		}
    
    }
    return resp;
}

char *dumpCanon(char* page, int pageLength){
    int size = pageLength * 10;
    char* resp = malloc(size);

    char* l = malloc(pageLength / 4);

    char ascii[17]; // last column
	size_t i, j;
	ascii[16] = '\0'; 
    
    int iter = 0; // iterator first column

    int last_star_or_not = 1; // bool flag
	for (i = 0; i < pageLength; ++i) {

        if (i % 16 == 0){
            if (CompareTwoBytes(page, i - 16, i) == 0 && i >= 16){ // check on first row for the some out like hexdump -C (cannonical)
                iter += 16;

                if (last_star_or_not != 0){
                    resp = strcat(resp, "*\n");

                }
            
                i += 15; // continue ++;
                
                last_star_or_not = 0;  
                continue;

            }
            sprintf(l,"%08x  ", iter);
            resp = strcat(resp, l);

            iter += 16;

            last_star_or_not = 1; 
        }
        
        sprintf(l,"%02X ", ((unsigned char*)page)[i]);
        resp = strcat(resp, l);
		
        if (((unsigned char*)page)[i] >= ' ' && ((unsigned char*)page)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)page)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == pageLength) {
            resp = strcat(resp, " ");
		
			if ((i+1) % 16 == 0) {
				sprintf(l, "|%s|\n", ascii);
                resp = strcat(resp, l);
            } else if (i+1 == pageLength) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
                    resp = strcat(resp, " ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
                    resp = strcat(resp, "   ");
				}
				sprintf(l, "|%s|\n", ascii);
                resp = strcat(resp, l);
			}
		}
    
    }
    return resp;
}


int CompareTwoBytes(const char* page, int s1, int s2){
    for(int i = 0; i < 16; i++, s1++, s2++){
        if (page[s1] != page[s2]){
            return 1;
        }
    }

    return 0;
}




//high test


char* getDumpCannFR(int pid){

}


char* getDumpCannSR(int pid){

}

char* getDumpCannF(int pid){

}

char* getDumpCannS(int pid){
        long ptraceResult = ptrace(PTRACE_ATTACH, pid, NULL, NULL);
        if (ptraceResult < 0)
        {
            // printf("Unable to attach to the pid specified\n");
            return NULL;
        }
        // wait(NULL);
        waitpid(pid, NULL, 0);

        char mapsFilename[1024];
        sprintf(mapsFilename, "/proc/%d/maps", pid);
        FILE* pMapsFile = fopen(mapsFilename, "r");
        char memFilename[1024];
        sprintf(memFilename, "/proc/%d/mem", pid);
        FILE* pMemFile = fopen(memFilename, "r");
        
        int size = getSize(pMapsFile);
        char *resp = malloc(size * 2);
        char *temp_ = 0;

        int sizeLine = 256;
        char line[sizeLine];
        while (fgets(line, sizeLine, pMapsFile) != NULL)
        {

            unsigned long start_address;
            unsigned long end_address;
            char *name = NULL;
            int typeAdd = -1;
            getShortAddress(line, &start_address, &end_address);
            name = getRName(line);
            if (name != NULL){

                sprintf(temp_, "\nregion = %s\n", name);
                resp = strcat(resp, temp_);
                free(name);
            }else{

                sprintf(temp_, "\nregion = %s\n", "anonymous region");
                resp = strcat(resp, temp_);
            }

            char* rs = dumpMRegion(pMemFile, start_address, end_address - start_address, typeAdd);
            resp = strcat(resp, rs);
        }
        fclose(pMapsFile);
        fclose(pMemFile);
        
        ptrace(PTRACE_CONT, pid, NULL, NULL);
        ptrace(PTRACE_DETACH, pid, NULL, NULL);

        return resp;
}


unsigned long getSize(FILE* pMapsFile){
        unsigned long size = 0;
        int sizeLine = 256;
        char line[sizeLine];
        while (fgets(line, sizeLine, pMapsFile) != NULL)
        {
            unsigned long start_address;
            unsigned long end_address;
            char *name = NULL;
            int typeAdd = -1;
            getLongAddress(line, &start_address, &end_address);
            size += (end_address - start_address);
        }

        return size;
}