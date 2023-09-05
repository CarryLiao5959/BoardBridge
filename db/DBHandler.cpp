#include "DBHandler.h"
using namespace bb::db;
#include "Logger.h"
#include "Singleton.h"
#include "TimeHandler.h"
using namespace bb::util;

#include <iostream>
#include <fstream>
#include <pqxx/pqxx>
#include <nlohmann/json.hpp>

DBHandler::DBHandler() : 
    m_filename(""),
    m_dbname("bb"),
    m_user("postgres"),
    m_password(""),
    m_table("")
{}

DBHandler::~DBHandler(){}

int DBHandler::get_json_data(const string filename){
    m_filename = filename;

    // 读取 JSON 文件
    std::ifstream json_file(m_filename);
    if (!json_file.is_open()) {
        log_error("Failed to open the JSON file.");
        return 1;
    }

    std::string json_data((std::istreambuf_iterator<char>(json_file)), std::istreambuf_iterator<char>());
    json_file.close();
    m_json_data=json_data;

    return 0;
}

void DBHandler::save_sys_to_db(){
    nlohmann::json j = nlohmann::json::parse(m_json_data);

    // Tasks
    int running = j["Tasks"]["running"];
    int sleeping = j["Tasks"]["sleeping"];
    int stopped = j["Tasks"]["stopped"];
    int total_tasks = j["Tasks"]["total"];
    int zombie = j["Tasks"]["zombie"];

    // CPU
    float cpu_hi = j["cpu"]["hi"];
    float cpu_id = j["cpu"]["id"];
    float cpu_ni = j["cpu"]["ni"];
    float cpu_si = j["cpu"]["si"];
    float cpu_st = j["cpu"]["st"];
    float cpu_sy = j["cpu"]["sy"];
    float cpu_us = j["cpu"]["us"];
    float cpu_wa = j["cpu"]["wa"];

    // Load Average
    float load_avg_1m = j["load_average"][0];
    float load_avg_5m = j["load_average"][1];
    float load_avg_15m = j["load_average"][2];

    // Memory
    float mem_buff_cache = j["memory"]["buff/cache"];
    float mem_free = j["memory"]["free"];
    float mem_total = j["memory"]["total"];
    float mem_used = j["memory"]["used"];

    // Swap
    float swap_avail_mem = j["swap"]["avail Mem"];
    float swap_free = j["swap"]["free"];
    float swap_total = j["swap"]["total"];
    float swap_used = j["swap"]["used"];

    // System info
    std::string system_time = j["system_time"];
    system_time=Singleton<TimeHandler>::instance()->get_today()+" "+system_time;
    std::string uptime = j["uptime"];
    int users = j["users"];

    std::string sql = "INSERT INTO system_status ("
                 "running, sleeping, stopped, total_tasks, zombie, "
                 "cpu_hi, cpu_id, cpu_ni, cpu_si, cpu_st, cpu_sy, cpu_us, cpu_wa, "
                 "load_avg_1m, load_avg_5m, load_avg_15m, "
                 "mem_buff_cache, mem_free, mem_total, mem_used, "
                 "swap_avail_mem, swap_free, swap_total, swap_used, "
                 "system_time, uptime, users) VALUES ("
                 + std::to_string(running) + ","
                 + std::to_string(sleeping) + ","
                 + std::to_string(stopped) + ","
                 + std::to_string(total_tasks) + ","
                 + std::to_string(zombie) + ","
                 + std::to_string(cpu_hi) + ","
                 + std::to_string(cpu_id) + ","
                 + std::to_string(cpu_ni) + ","
                 + std::to_string(cpu_si) + ","
                 + std::to_string(cpu_st) + ","
                 + std::to_string(cpu_sy) + ","
                 + std::to_string(cpu_us) + ","
                 + std::to_string(cpu_wa) + ","
                 + std::to_string(load_avg_1m) + ","
                 + std::to_string(load_avg_5m) + ","
                 + std::to_string(load_avg_15m) + ","
                 + std::to_string(mem_buff_cache) + ","
                 + std::to_string(mem_free) + ","
                 + std::to_string(mem_total) + ","
                 + std::to_string(mem_used) + ","
                 + std::to_string(swap_avail_mem) + ","
                 + std::to_string(swap_free) + ","
                 + std::to_string(swap_total) + ","
                 + std::to_string(swap_used) + ","
                 + "'" + system_time + "',"
                 + "'" + uptime + "',"
                 + std::to_string(users) + ");";
    
    try {
        pqxx::connection conn("dbname=" + m_dbname + " user=" + m_user + " password=" + m_password + " hostaddr=127.0.0.1 port=5433");
        if (!conn.is_open()) {
            log_error("Can't open database");
            return;
        }
        log_info("connection db success");

        pqxx::work work(conn);
        work.exec(sql);
        log_info("exec sql success");
        work.commit();
        log_info("commit sql success");

        log_info("Data inserted into the database.");
        conn.disconnect();
    } catch (const std::exception &e) {
        log_error("%s", e.what());
    }
}