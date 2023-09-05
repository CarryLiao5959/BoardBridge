#include <string>

using namespace std;

namespace bb {
namespace util {
class TimeHandler {
public:
  TimeHandler();
  ~TimeHandler();
  string get_today();

private:
  std::string getCurrentDate();

private:
  string m_today;
};
} // namespace util
} // namespace bb