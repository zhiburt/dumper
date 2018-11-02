#include <unistd.h>
#include <dirent.h>
#include <sys/types.h> // for opendir(), readdir(), closedir()
#include <sys/stat.h> // for stat()

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>

#define PROC_DIRECTORY "/proc/"

std::string getProcessName(const std::string& fileContent);
std::string getProcessState(const std::string& fileContent);

int IsNumeric(const char* ccharptr_CharacterList)
{
    for ( ; *ccharptr_CharacterList; ccharptr_CharacterList++)
        if (*ccharptr_CharacterList < '0' || *ccharptr_CharacterList > '9')
            return 0; // false
    return 1; // true
}

int main()
{
    struct dirent* dirEntity = NULL ;
    DIR* dir_proc = NULL ;

    dir_proc = opendir(PROC_DIRECTORY) ;
    if (dir_proc == NULL)
    {
        perror("Couldn't open the " PROC_DIRECTORY " directory") ;
        return (pid_t) -2 ;
    }

    while ((dirEntity = readdir(dir_proc)) != 0) {
        if (dirEntity->d_type == DT_DIR) {
            if (IsNumeric(dirEntity->d_name)) {
                std::string path = std::string(PROC_DIRECTORY) + dirEntity->d_name + "/status";

                std::ifstream cmdLineFile(path.c_str());
                std::stringstream fileContent;
                std::string statusLine;

                if (cmdLineFile.is_open()) {
                    while (std::getline(cmdLineFile, statusLine)){
                        fileContent << statusLine << std::endl;
                    }
                }
                
                std::string nameProcess = getProcessName(fileContent.str());
                std::string statusProcess = getProcessState(fileContent.str());


                std::cout << "pid: " << dirEntity->d_name << " " << nameProcess << " state:  " << statusProcess << std::endl;

                fileContent.clear();
                cmdLineFile.close();
            }
        }
    }
    closedir(dir_proc) ;
    return 0;
}

std::string getProcessName(const std::string& fileContent){
    //[\\:\\(\\.\\+\\w\\d\\)/_-]
    std::regex regular("Name:[\\s\\t]*(.+)\\n");
    std::cmatch math;
    if (std::regex_search (fileContent.c_str(), math, regular)) {
        return math[1];
    }
    return "invalid name";
}

std::string getProcessState(const std::string& fileContent){
    //State:	S (sleeping)
    std::regex regular("State:[\\s\\t]*\\b([\\w]+)\\b.*(\\(.+\\))");
    std::cmatch math;
    while (std::regex_search (fileContent.c_str(), math, regular)) {
        return math[1].str() + " " + math[2].str();
    }
    std::cout << fileContent << std::endl;

    return "invalid state";
}