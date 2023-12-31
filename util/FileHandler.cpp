#include "FileHandler.h"
#include "Logger.h"
using namespace bb::util;

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

FileHandler::FileHandler() {}
FileHandler::FileHandler(const string filename) :m_filename(filename) {}
FileHandler::~FileHandler() {}

void FileHandler::set_filename(const string filename){
    m_filename=filename;
}

size_t FileHandler::read_file(char* buf) {
    log_debug("read_file");

    ifstream file(m_filename, ifstream::binary);
    log_debug("filename: %s", m_filename.c_str());
    if (!file) {
        log_error("open file %s fail: %s", m_filename.c_str(), strerror(errno));
        return -1;
    }
    // Go to the end of the file
    file.seekg(0, std::ios::end);
    // Get the current position (which is the file size)
    std::streampos fileSize = file.tellg();

    // Go back to the start of the file and read its content
    file.seekg(0, std::ios::beg);
    file.read(buf, fileSize);
    log_debug("%s", buf);

    file.close();
    return fileSize;
}

FILE* FileHandler::open_pipe(const char* cmd, const char* type) {
    log_debug("open_pipe");
    FILE* fp;
    fp = popen(cmd, type);
    if (fp == NULL) {
        log_error("open pipe");
        return NULL;
    }
    return fp;
}

size_t FileHandler::read_pipe(FILE* fp) {
    log_debug("read_pipe");
    memset(m_buf, 0, buf_size);
    if (feof(fp)) {
        return 0;
    }
    int nbytes = fread(m_buf, 1, buf_size - 1, fp);
    if (ferror(fp)) {
        log_error("read failed|errno=%d,errstr=%s", errno, strerror(errno));
        return -1;
    }
    log_debug("buf=[%s],len=[%d]", m_buf, nbytes);
    return nbytes;
}

size_t FileHandler::read_pipe_without_first_line(FILE* fp) {
    log_debug("read_pipe");
    memset(m_buf, 0, buf_size);
    if (feof(fp)) {
        return 0;
    }
    // read the first line
    if (!fgets(m_buf, buf_size, fp)) {
        log_error("fgets failed|errno=%d,errstr=%s", errno, strerror(errno));
        return -1;
    }
    // length of first line
    int line_len = strlen(m_buf);
    // read the rest line
    memset(m_buf, 0, buf_size);
    int nbytes = fread(m_buf, 1, buf_size - line_len - 1, fp);
    log_debug("m_buf=[%s],len=[%d]", m_buf, line_len + nbytes);
    if (ferror(fp)) {
        log_error("read failed|errno=%d,errstr=%s", errno, strerror(errno));
        return -1;
    }
    return nbytes;
}