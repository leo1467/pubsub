#include <iostream>
#include <cstring>
#include <unordered_map>
#include <string>
#include "ini.h"
#include "INIReader.h"

#define my_strcmp(s, n) strcmp (s, n) == 0

typedef struct {
    std::string Addr;
    std::string Port;
    std::string Type;
} configuration;

namespace USER_MAP
{
class UserMap {
public:
    static std::unordered_map<std::string, configuration *> &GetInstance()
    {
        static std::unordered_map<std::string, configuration *> userMap;
        return userMap;
    }
};
}; // namespace USER_MAP

int handler(void *user, const char *section, const char *name, const char *value) {
    std::string username = section;
    auto &userMap = USER_MAP::UserMap::GetInstance();
    auto iter = userMap.find(username);
    if (iter == userMap.end()) {
        std::cout << "not in usermap, add user:" << username << std::endl;
        userMap.insert(std::make_pair(username, (configuration *)user));
    }

    iter = userMap.find(username);
    if (my_strcmp(name, "Addr")) {
        iter->second->Addr = value;
    }
    else if (my_strcmp(name, "Port")) {
        iter->second->Port = value;
    }
    else if (my_strcmp(name, "Type")) {
        iter->second->Type = value;
    }

    return 0;
}

void print_user() {
    for (const auto &ele : USER_MAP::UserMap::GetInstance()) {
        std::cout << ele.first << std::endl;
        std::cout << ele.second->Addr << std::endl;
        std::cout << ele.second->Port << std::endl;
        std::cout << ele.second->Type << std::endl;
    }
}

int read_user_from_ini(const std::string &tbasePath) {
    int rc = 0;
    configuration conf;
    rc = ini_parse(tbasePath.c_str(), handler, &conf);
    if (rc < 0) {
        std::cout << "Cant load " << tbasePath << std::endl;
    }
    print_user();
    return rc;
}


namespace INI_READER_TOOL
{
class IniReader : public INIReader {
public:
    IniReader(const std::string &tbasePath) : INIReader(tbasePath) {
    }
};
} // namespace IniReader


int main(int argc, char *argv[]) {
    const std::string tbasePath = "../tbase/BusClnt.ini";
    std::cout << "c version" << std::endl;
    read_user_from_ini(tbasePath);
    std::cout << "===========================================" << std::endl;
    std::cout << std::endl;
    std::cout << "cpp version" << std::endl;
    INI_READER_TOOL::IniReader iniReader(tbasePath);
    std::cout << "===========================================" << std::endl;
    return 0;
}