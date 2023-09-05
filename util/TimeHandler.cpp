#include "TimeHandler.h"
using namespace bb::util;

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

TimeHandler::TimeHandler() {
}

TimeHandler::~TimeHandler() {
}

string TimeHandler::get_today() {
    if (m_today != "") {
        return m_today;
    }
    return getCurrentDate();
}

std::string TimeHandler::getCurrentDate() {
    std::time_t t = std::time(nullptr);
    std::tm* localTime = std::localtime(&t);

    std::ostringstream oss;
    oss << std::setfill('0')
        << (localTime->tm_year + 1900) << '-'
        << std::setw(2) << (localTime->tm_mon + 1) << '-'
        << std::setw(2) << localTime->tm_mday;
    m_today = oss.str();
    return m_today;
}
