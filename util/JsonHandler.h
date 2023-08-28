#pragma once

#include <string>
using namespace std;

namespace bb {
namespace util {

class JsonHandler {
  public:
    JsonHandler();
    JsonHandler(string directory);
    ~JsonHandler();

    void set_filename(string filename);

    void get_proc_json(string data);
    void get_sys_json(string data);

  private:
    string m_directory;
    string m_filename;
};

} // namespace util
} // namespace bb