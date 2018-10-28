#include "dumper.h"

//we read here some region by pages
void dump_memory_region(FILE* pMemFile, unsigned long start_address, long length, int serverSocket)
{
    unsigned long address;
    int pageLength = getpagesize(); // int pageLength = 4096;
    unsigned char page[pageLength];
    fseeko(pMemFile, start_address, SEEK_SET);//move pointer of file (carriage) to the start of the region 

    for (address=start_address; address < start_address + length; address += pageLength)
    {
        fread(&page, 1, pageLength, pMemFile);
        if (serverSocket == -1)
        {
            // write to stdout
            fwrite(&page, 1, pageLength, stdout);
        }
        else
        {
            send(serverSocket, &page, pageLength, 0);
        }
    }
}