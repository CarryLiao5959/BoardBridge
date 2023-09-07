#include <string>

using namespace std;

namespace bb
{
namespace db
{
class DBHandler{

public:
    DBHandler();
    ~DBHandler();

    int get_json_data(const string filename);
    void try_insert(const string& sql);
    void save_sys_to_db();
    void save_rproc_to_db();

private:
    string m_filename;
    string m_dbname;
    string m_user;
    string m_password;
    string m_table;
    string m_json_data;
};

}

}