#pragma once

namespace bb {
namespace util {

template <typename T>
class Singleton {
  public:
    template <typename... Args>
    static T *instance(Args... args) {
        if (m_instance == nullptr) {
            m_instance = new T(std::forward<Args>(args)...);
        }
        return m_instance;
    }

    static void destroy() {
        delete m_instance;
        m_instance = nullptr;
    }

  private:
    Singleton() {}
    ~Singleton() {}
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;

  private:
    /*
    @brief   全局唯一的实例
    @details 在程序的生命周期中只被创建和初始化一次
    @note    该静态变量的存储位置在程序的数据段
    */
    static T *m_instance;
};

template <typename T>
T *Singleton<T>::m_instance = nullptr;

} // namespace util
} // namespace bb