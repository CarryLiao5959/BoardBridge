#include "Logger.h"
using namespace bb::util;

#include <time.h>
#include <string.h>
#include <stdexcept>
#include <iostream>
#include <stdarg.h>

const char *Logger::s_level[LEVEL_COUNT] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL",
};

Logger *Logger::m_logger = nullptr;

Logger *Logger::instance() {
    if (m_logger == NULL) {
        m_logger = new Logger();
    }
    return m_logger;
}

Logger::Logger() : m_level(DEBUG), m_max_line(4096), m_line(0) {
}

Logger::~Logger() {
    close_fstream();
}

void Logger::open_ofstream(const string &filename, ios_base::openmode mode) {
    m_mutex.lock();

    m_fout.open(filename, mode);
    m_filename = filename;
    if (m_fout.fail()) {
        throw logic_error("[Logger] log: Could not open log file " + m_filename);
    }
    m_fout << "-----------------------------------------------------------------------\n";

    m_mutex.unlock();
    count_line();
}

int Logger::count_line() {
    m_mutex.lock();

    m_fin.open(m_filename);
    if (m_fin.fail()) {
        throw logic_error("[Logger] log: Could not open log file " + m_filename);
    }
    string line;
    int lineCount = 0;
    while (getline(m_fin, line)) {
        ++lineCount;
    }
    m_line = lineCount;

    m_mutex.unlock();
    return m_line;
}

void Logger::close_fstream() {
    m_fout.close();
    m_fin.close();
}

void Logger::log(Level level, const char *file, int line, const char *fmt, ...) {
    if (m_level > level) {
        return;
    }

    if (m_fout.fail()) {
        throw logic_error("[Logger] log: Could not open log file " + m_filename);
    }

    time_t ticks = time(NULL);
    struct tm *ptm = localtime(&ticks);
    // ptm->tm_hour += 8; // add 8 hours to Beijing time
    mktime(ptm);
    char timestamp[32];
    memset(timestamp, 0, 32);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", ptm);

    const char *myfmt = "%s %s %s: %d ";
    int size = snprintf(NULL, 0, myfmt, timestamp, s_level[level], file, line);
    m_mutex.lock();
    if (size > 0) {
        char *buffer = new char[size + 1];
        snprintf(buffer, size + 1, myfmt, timestamp, s_level[level], file, line);
        buffer[size] = '\0';
        m_fout << buffer;
        delete[] buffer;
    }

    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    size = vsnprintf(NULL, 0, fmt, arg_ptr);
    va_end(arg_ptr);
    if (size > 0) {
        char *content = new char[size + 1];
        va_start(arg_ptr, fmt);
        size = vsnprintf(content, size + 1, fmt, arg_ptr);
        va_end(arg_ptr);
        m_fout << content;
        m_line++;
        delete[] content;
    }

    m_fout << endl;
    m_fout.flush();
    m_mutex.unlock();

    if (m_line >= m_max_line && m_max_line > 0) {
        rotate();
    }
}

void Logger::rotate() {
    close_fstream();

    time_t ticks = time(NULL);
    struct tm *ptm = localtime(&ticks);
    // ptm->tm_hour += 8; // add 8 hours to Beijing time
    mktime(ptm);
    char timestamp[32];
    memset(timestamp, 0, 32);
    strftime(timestamp, sizeof(timestamp), " %Y-%m-%d %H:%M:%S", ptm);

    string filename = m_filename + timestamp;
    if (rename(m_filename.c_str(), filename.c_str()) != 0) {
        throw std::logic_error("rename log name failed: " + string(strerror(errno)));
    }
    open_ofstream(m_filename);
}