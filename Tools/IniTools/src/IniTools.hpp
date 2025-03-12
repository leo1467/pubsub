#ifndef _INI_TOOLS_HPP_
#define _INI_TOOLS_HPP_

#include <cstring>
#include <iostream>
#include <unordered_map>
#include <string>

#include "ini.h"
#include "INIReader.h"

#define my_strcmp(s, n) strcmp (s, n) == 0

namespace INI_READER_TOOL
{

struct Ini_type_clnt_t { explicit Ini_type_clnt_t() = default; };
struct Ini_type_serv_t { explicit Ini_type_serv_t() = default; };

constexpr Ini_type_clnt_t clntType{};
constexpr Ini_type_serv_t servType{};

class UserInfo {
public:
    std::string Addr;
    std::string Port;
    std::string Type;
};

class UserReader final {
public:
    void Print_user() {
        for (const auto &ele : userMap) {
            std::cout << ele.first << std::endl;
            std::cout << ele.second.Addr << std::endl;
            std::cout << ele.second.Port << std::endl;
            std::cout << ele.second.Type << std::endl;
        }
    }

    int Update_user_from_ini(const std::string &tbasePath) {
        int rc = 0;
        rc = ini_parse(tbasePath.c_str(), handler, nullptr);
        if (rc < 0) {
            std::cout << "Cant load " << tbasePath << std::endl;
        }
        return rc;
    }

    bool Is_user_in_map(const std::string &username) {
        return userMap.find(username) != userMap.end();
    }

    const UserInfo *Get_user_info(const std::string &username) {
        if (!Is_user_in_map(username)) {
            return nullptr;
        }
        return &userMap.find(username)->second;
    }

    UserReader() = default;
    ~UserReader() = default;
    // IniReader

private:
    inline static std::unordered_map<std::string, UserInfo> userMap;

    static int handler(void *info, const char *section, const char *name, const char *value) {
        std::string username = section;
        auto iter = userMap.find(username);
        if (iter == userMap.end()) {
            std::cout << "not in usermap, add user:" << username << std::endl;
            userMap.insert(std::make_pair(username, UserInfo()));
        }

        iter = userMap.find(username);
        if (my_strcmp(name, "Addr")) {
            iter->second.Addr = value;
        }
        else if (my_strcmp(name, "Port")) {
            iter->second.Port = value;
        }
        else if (my_strcmp(name, "Type")) {
            iter->second.Type = value;
        }

        return 0;
    }
};

} // namespace INI_READER_TOOL


#endif
