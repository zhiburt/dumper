#include "dumper.h"

//we read here some region by pages
void dump_memory_region(FILE* pMemFile, unsigned long start_address, long length)
{
    unsigned long address;
    int pageLength = getpagesize(); // int pageLength = 4096;
    unsigned char page[pageLength];
    fseeko(pMemFile, start_address, SEEK_SET);//move pointer of file (carriage) to the start of the region 

    for (address=start_address; address < start_address + length; address += pageLength)
    {
        fread(&page, 1, pageLength, pMemFile);
        //writeTo(page, pageLength);
        dumpCanonical(page, pageLength);
    }
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
        sscanf(line, "%08lx-%08lx\n", &sa, &ea);
        if (start_address == sa && ea == start_address + length){
            fclose(pMapsFile);
            char* last = NULL;
            last = getLastWord(line);
            return last;
        }
    }
    fclose(pMapsFile);
    return "invalid";
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

int CompareTwoBytes(const char* page, int s1, int s2){
    for(int i = 0; i < 16; i++, s1++, s2++){
        if (page[s1] != page[s2]){
            return 1;
        }
    }

    return 0;
}