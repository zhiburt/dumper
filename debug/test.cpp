#include <iostream>
#include <fstream>

using namespace std;

extern "C" void writeTo(char* page, int pageLength);//prototype
extern "C" char* dumpMRegion(FILE* pMemFile, unsigned long start_address, long length, int realAdress);
extern "C" char* getDumpCann(int pid, int addrrType, int scnTp);

int main(int argc, char* argv[]){
    std::ofstream out("out_test.txt"); //откроем файл для вывод
    std::streambuf *coutbuf = std::cout.rdbuf(); //запомним старый буфер
    std::cout.rdbuf(out.rdbuf()); //и теперь все будет в файл out.txt!


    char* s = getDumpCann(13698, 1, 1);
    std::cout << s << std::endl;

    std::cout.rdbuf(coutbuf);

    std::cout << "DONE" << std::endl;

    return EXIT_SUCCESS;
}