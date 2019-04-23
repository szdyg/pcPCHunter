#pragma once
#include <stdarg.h>

namespace pclog 
{
    enum Level                        // 日志输出级别
    {
        kStartup    = 0x01,           // 启动级别
        kShutdown   = 0x02,           // 关闭级别
        kInfo       = 0x04,           // 普通信息级别
        kWarning    = 0x08,           // 警告级别
        kTrace      = 0x10,           // 跟踪级别
        kError      = 0x20,           // 错误级别
        kDebug      = 0x40,           // 调试级别
        kLevelMask  = 0x7F,           // mask
    };

    enum StorageType                // 日志输出类型
    {
        kScreen          = 0x01,    // 屏幕输出
        kDiskFile        = 0x02,    // 日志文件输出
        kStorageTypeMask = 0x0F,    // mask
    };

    enum FieldType                  // 字段类型
    {
        kTimestamp      = 0x01,     // 输出时间标签
        kThreadId       = 0x02,     // 输出线程Id标签
        kFileLine       = 0x04,     // 输出在源代码文件和行数标签
        kFunction       = 0x08,     // 输出在源代码的函数名称`
        kFieldTypeMask  = 0x0F,     // mask
    };

    void print_v(unsigned int level, const wchar_t*  filename, unsigned int line, const char*  function, const wchar_t*  format, va_list ap);
    void print(unsigned int level, const wchar_t*  filename, unsigned int line, const char*  function, const wchar_t*  format, ...);
    void hexdump(unsigned int level, const wchar_t*  filename, unsigned int line, const char*  function, const void*  data, int size);

    void set_directory(const wchar_t* path);
    void set_prefix_name(const wchar_t* prefix_name);
    void set_level(unsigned int level);
    void set_storage_type(unsigned int storage_type);
    void set_field_type(unsigned int field_type);
    void set_reserve_days(int days);      // 日志保留天数，默认为30天

}  // namespace pclog


enum LogLevel  // 日志输出级别
{
    kStartup    = pclog::kStartup,        // 启动级别
    kShutdown   = pclog::kShutdown,       // 关闭级别
    kInfo       = pclog::kInfo,           // 普通信息级别
    kWarning    = pclog::kWarning,        // 警告级别
    kTrace      = pclog::kTrace,          // 跟踪级别
    kError      = pclog::kError,          // 错误级别
    kDebug      = pclog::kDebug           // 调试级别
};

#define LOG_SHUTDOWN(...)         pclog::print(kShutdown, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_STARTUP(...)          pclog::print(kStartup, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_INFO(...)             pclog::print(kInfo, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_WARN(...)             pclog::print(kWarning, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_TRACE(...)            pclog::print(kTrace, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_ERROR(...)            pclog::print(kError, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_DEBUG(...)            pclog::print(kDebug, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LogHex(level, data, size) pclog::hexdump(level, __FILE__, __LINE__, __FUNCTION__, data, size)

