#include <iostream>
#include <cstring>
#include <unordered_map>
#include <string>
#include "ini.h"

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

typedef struct {
    const char *addr;
    const char *port;
    const char *type;
} configuration;

namespace USER_MAP {
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
    std::cout << section << ", " << name << ", " << value << std::endl;
    // if (MATCH("TEST1", "Addr")) {
    //     std::cout << value << std::endl;
    // }
    // else if (MATCH("TEST1", "Port")) {
    //     std::cout << value << std::endl;
    // }
    // else if (MATCH("TEST1", "Type")) {
    //     std::cout << value << std::endl;
    // }
    std::string username = section;
    auto &userMap = USER_MAP::UserMap::GetInstance();
    if (userMap.find(username) == userMap.end()) {
        std::cout << "not in usermap, add user:" << username << std::endl;
        userMap.insert(std::make_pair(username, (configuration *)user));
    }
    std::cout << userMap.size() << std::endl;
    return 0;
}

int main(int argc, char *argv[]) {
    std::string tbasePath = "../tbase/BusClnt.ini";
    configuration conf = {};
    int rc = 0;
    rc = ini_parse(tbasePath.c_str(), handler, &conf);
    if (rc < 0) {
        std::cout << "Cant load " << tbasePath << std::endl;
    }
    std::cout << USER_MAP::UserMap::GetInstance().size() << std::endl;
    return 0;
}