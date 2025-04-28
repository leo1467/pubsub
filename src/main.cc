#include <string>

#include "IniTools.hpp"

int main(int argc, char *argv[]) {
    const std::string tbasePath = "../tbase/BusClnt.ini";
    INI_READER_TOOL::UserReader iniReader;
    iniReader.Update_user_from_ini(tbasePath);
    iniReader.Print_user();
    return 0;
}