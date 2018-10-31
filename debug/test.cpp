#include <iostream>
#include <fstream>

using namespace std;

extern "C" void writeTo(char* page, int pageLength);//prototype
extern "C" char* dumpMRegion(FILE* pMemFile, unsigned long start_address, long length, int realAdress);
extern "C" char* getDumpCannS(int pid);


int main(int argc, char* argv[]){
    std::ofstream out("out_test.txt"); //откроем файл для вывод
    std::streambuf *coutbuf = std::cout.rdbuf(); //запомним старый буфер
    std::cout.rdbuf(out.rdbuf()); //и теперь все будет в файл out.txt!

    char* s = getDumpCannS(3163);
    std::cout << s << std::endl;

    return EXIT_SUCCESS;
}