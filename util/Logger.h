#pragma once

#include "Mutex.h"
using namespace bb::thread;

#include <fstream>
#include <string>
using namespace std;

namespace bb {
namespace util {

#define log_debug(format, ...) \
    Logger::instance()->log(Logger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define log_info(format, ...) \
    Logger::instance()->log(Logger::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define log_warn(format, ...) \
    Logger::instance()->log(Logger::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define log_error(format, ...) \
    Logger::instance()->log(Logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define log_fatal(format, ...) \
    Logger::instance()->log(Logger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__);

class Logger {
  public:
    enum Level {
        DEBUG = 0,
        INFO,
        WARN,
        ERROR,
        FATAL,
        LEVEL_COUNT,
    };

    static Logger *instance();

    void log(Level level, const char *file, int line, const char *fmt, ...);

    void open_ofstream(const string &filename, ios_base::openmode mode = ios::app);
    void close_fstream();

    void set_level(Level level) { m_level = level; }
    int count_line();
    void set_max_line(int bytes) { m_max_line = bytes; }

  private:
    Logger();
    ~Logger();

    void rotate();

  private:
    static Logger *m_logger;
    static const char *s_level[LEVEL_COUNT];

    string m_filename;
    ofstream m_fout;
    ifstream m_fin;

    Level m_level;
    int m_max_line;
    int m_line;

    Mutex m_mutex;
};



} // namespace util
} // namespace bb