#include "ConstPara.h"
#include<string>

using namespace std;

class FileHandler {
public:
    FileHandler();
    FileHandler(const string filename);
    ~FileHandler();

    size_t read_file(char* buf);
    FILE* open_pipe(const char* type = "r");
    size_t read_pipe(FILE* fp);
    size_t read_pipe_without_first_line(FILE* fp);

private:
    string m_filename;
    char m_buf[buf_size];
};