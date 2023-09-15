#include "JsonHandler.h"

#include "Logger.h"
using namespace bb::util;

#include <fstream>
#include <iostream>
#include <iomanip>

#define JSON_USE_IMPLICIT_CONVERSIONS 0
#define JSON_ORDERED_MAP 1
#include <nlohmann/json.hpp>

using json = nlohmann::json;

JsonHandler::JsonHandler() {
}

JsonHandler::JsonHandler(std::string directory) {
    m_directory = directory;
}

JsonHandler::~JsonHandler() {
}

void JsonHandler::set_filename(std::string filename) {
    m_filename = filename;
}

string trim(string s){
    int start, end;
    int n = s.size();
    for (int i = 0; i < n; i++) {
        if(s[i]!=' '){
            start = i;
            break;
        }
    }
    for (int i = n-1; i >= 0; i--) {
        if (s[i] != ' ') {
            end = i;
            break;
        }
    }
    return s.substr(start, end - start + 1);
}

void JsonHandler::get_sys_json(string data) {
    /*
    top - 10:35:00 up  1:09,  0 users,  load average: 0.00, 0.00, 0.00
    Tasks:  26 total,   1 running,  25 sleeping,   0 stopped,   0 zombie
    %Cpu(s):  0.0 us,  0.0 sy,  0.0 ni, 99.3 id,  0.3 wa,  0.0 hi,  0.3 si,  0.0 st
    MiB Mem :  12528.9 total,  10508.1 free,    908.2 used,   1112.7 buff/cache
    MiB Swap:   4096.0 total,   4096.0 free,      0.0 used.  11380.8 avail Mem
    */
    json j;

    std::istringstream ss(data);
    std::string line;

    size_t start=0, end=0;

    // 第一行解析
    log_debug("1");
    // top - 10:35:00 up  1:09,  0 users,  load average: 0.00, 0.00, 0.00
    std::getline(ss, line);
    start = line.find('-')+1;
    end = line.find("up");
    j["system_time"] = trim(line.substr(start, end - start));
    log_debug("system_time %s", j["system_time"]);
    start = end + 2;
    end = line.find(',', start);
    j["uptime"] = trim(line.substr(start, end - start));
    log_debug("system_time %s", j["uptime"]);
    start = end + 1;
    end = line.find("users");
    j["users"] = std::stoi(trim(line.substr(start, end - start)));
    log_debug("system_time %d", j["users"]);

    std::vector<float> loads;
    size_t loadStart = line.rfind(":") + 1;
    std::stringstream loadss(line.substr(loadStart));
    std::string load;
    while (std::getline(loadss, load, ',')) {
        loads.push_back(std::stof(trim(load)));
    }
    j["load_average"] = loads;

    // Tasks行
    log_debug("2");
    // Tasks:  26 total,   1 running,  25 sleeping,   0 stopped,   0 zombie
    std::getline(ss, line);
    start = line.find(':', 0) + 1;
    end = line.find(',', start) - 5;
    j["Tasks"]["total"] = std::stoi(trim(line.substr(start, end - start)));
    start = line.find(',', end) + 1;
    end = line.find(',', start) - 7;
    j["Tasks"]["running"] = std::stoi(trim(line.substr(start, end - start)));
    start = line.find(',', end) + 1;
    end = line.find(',', start) - 8;
    j["Tasks"]["sleeping"] = std::stoi(trim(line.substr(start, end - start)));
    start = line.find(',', end) + 1;
    end = line.find(',', start) - 7;
    j["Tasks"]["stopped"] = std::stoi(trim(line.substr(start, end - start)));
    start = line.find(',', end) + 1;
    end = line.find(',', start) - 6;
    j["Tasks"]["zombie"] = std::stoi(trim(line.substr(start, end - start)));

    // CPU行
    log_debug("3");
    // %Cpu(s):  0.0 us,  0.0 sy,  0.0 ni, 99.3 id,  0.3 wa,  0.0 hi,  0.3 si,  0.0 st
    std::getline(ss, line);
    start = line.find(':', 0) + 1;
    end = line.find("us");
    j["cpu"]["us"] = std::stof(trim(line.substr(start, end - start)));
    log_debug("start %d end %d %s", start, end, line.substr(start, end - start).c_str());
    log_debug("us");

    start = line.find(',', end) + 1;
    end = line.find("sy");
    j["cpu"]["sy"] = std::stof(trim(line.substr(start, end - start)));
    log_debug("start %d end %d %s", start, end, line.substr(start, end - start).c_str());
    log_debug("sy");

    start = line.find(',', end) + 1;
    end = line.find("ni");
    j["cpu"]["ni"] = std::stof(trim(line.substr(start, end - start)));
    log_debug("start %d end %d %s", start, end, line.substr(start, end - start).c_str());
    log_debug("ni");

    start = line.find(',', end) + 1;
    end = line.find("id");
    j["cpu"]["id"] = std::stof(trim(line.substr(start, end - start)));
    log_debug("start %d end %d %s", start, end, line.substr(start, end - start).c_str());
    log_debug("id");

    start = line.find(',', end) + 1;
    end = line.find("wa");
    j["cpu"]["wa"] = std::stof(trim(line.substr(start, end - start)));
    log_debug("start %d end %d %s", start, end, line.substr(start, end - start).c_str());
    log_debug("wa");

    start = line.find(',', end) + 1;
    end = line.find("hi");
    j["cpu"]["hi"] = std::stof(trim(line.substr(start, end - start)));
    log_debug("start %d end %d %s", start, end, line.substr(start, end - start).c_str());
    log_debug("hi");

    start = line.find(',', end) + 1;
    end = line.find("si");
    // end = start + 5;
    log_debug("start %d end %d %s", start, end, line.substr(start, end - start).c_str());
    j["cpu"]["si"] = std::stof(trim(line.substr(start, end - start)));
    log_debug("si");

    start = line.find(',', end) + 1;
    end = line.find("st");
    // end = start + 5;
    log_debug("start %d end %d %s", start, end, line.substr(start, end - start).c_str());
    j["cpu"]["st"] = std::stof(trim(line.substr(start, end - start)));
    log_debug("st");

    // Mem行
    log_debug("4");
    // MiB Mem :  12528.9 total,  10508.1 free,    908.2 used,   1112.7 buff/cache
    std::getline(ss, line);
    start = line.find(':', 0) + 1;
    end = line.find("total");
    j["memory"]["total"] = std::stof(trim(line.substr(start, end - start)));
    start = line.find(',', end) + 1;
    end = line.find("free");
    j["memory"]["free"] = std::stof(trim(line.substr(start, end - start)));
    start = line.find(',', end) + 1;
    end = line.find("used");
    j["memory"]["used"] = std::stof(trim(line.substr(start, end - start)));
    start = line.find(',', end) + 1;
    end = line.find("buff");
    j["memory"]["buff/cache"] = std::stof(trim(line.substr(start, end - start)));

    // Swap行 
    log_debug("5");
    // MiB Swap : 4096.0 total, 4096.0 free, 0.0 used.11380.8 avail Mem
    std::getline(ss, line);
    start = line.find(':', 0) + 1;
    end = line.find("total");
    j["swap"]["total"] = std::stof(trim(line.substr(start, end - start)));
    start = line.find(',', end) + 1;
    end = line.find("free");
    j["swap"]["free"] = std::stof(trim(line.substr(start, end - start)));
    start = line.find(',', end) + 1;
    end = line.find("used");
    j["swap"]["used"] = std::stof(trim(line.substr(start, end - start)));
    start = line.find('.', end) + 1;
    end = line.find("avail");
    j["swap"]["avail Mem"] = std::stof(trim(line.substr(start, end - start)));

    // 输出JSON
    std::ofstream file(m_directory + m_filename);
    file << j.dump(4) << endl;
    file.close();
    log_debug("Data saved to %s", m_filename.c_str());
}

void JsonHandler::get_proc_json(string data) {
    /*
    PID  PPID COMMAND         S STIME
    */
    json j = json::object();

    std::istringstream ss(data);
    size_t start = 0, end = 0;

    std::string line;
    getline(ss, line);
    log_debug("%s", line.c_str());
    string proc_name;
    int cnt = 1;
    while (getline(ss, line)) {
        // log_debug("%s", line.c_str());
        line = trim(line);
        proc_name = "process " + to_string(cnt);
        start = 0;
        end = line.find(' ');
        j[proc_name]["PID"] = stoi(trim(line.substr(start, end - start)));
        line = trim(line.substr(end));
        start = 0;
        end = line.find(' ');
        j[proc_name]["PPID"] = stoi(trim(line.substr(start, end - start)));
        line = trim(line.substr(end));
        start = 0;
        end = line.find(' ');
        j[proc_name]["COMMAND"] = trim(line.substr(start, end - start));
        line = trim(line.substr(end));
        start = 0;
        end = line.find(' ');
        j[proc_name]["STATUS"] = trim(line.substr(start, end - start));
        line = trim(line.substr(end));
        start = 0;
        end = line.size();
        j[proc_name]["STIME"] = trim(line.substr(start, end - start));
        cnt++;
    }

    // 输出JSON
    std::ofstream file(m_directory + m_filename);
    file << j.dump(4) << endl;
    file.close();
    log_debug("Data saved to %s", m_filename.c_str());
}