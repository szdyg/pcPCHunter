#pragma once
#include <stdarg.h>

namespace pclog 
{
    enum Level                        // ��־�������
    {
        kStartup    = 0x01,           // ��������
        kShutdown   = 0x02,           // �رռ���
        kInfo       = 0x04,           // ��ͨ��Ϣ����
        kWarning    = 0x08,           // ���漶��
        kTrace      = 0x10,           // ���ټ���
        kError      = 0x20,           // ���󼶱�
        kDebug      = 0x40,           // ���Լ���
        kLevelMask  = 0x7F,           // mask
    };

    enum StorageType                // ��־�������
    {
        kScreen          = 0x01,    // ��Ļ���
        kDiskFile        = 0x02,    // ��־�ļ����
        kStorageTypeMask = 0x0F,    // mask
    };

    enum FieldType                  // �ֶ�����
    {
        kTimestamp      = 0x01,     // ���ʱ���ǩ
        kThreadId       = 0x02,     // ����߳�Id��ǩ
        kFileLine       = 0x04,     // �����Դ�����ļ���������ǩ
        kFunction       = 0x08,     // �����Դ����ĺ�������`
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
    void set_reserve_days(int days);      // ��־����������Ĭ��Ϊ30��

}  // namespace pclog


enum LogLevel  // ��־�������
{
    kStartup    = pclog::kStartup,        // ��������
    kShutdown   = pclog::kShutdown,       // �رռ���
    kInfo       = pclog::kInfo,           // ��ͨ��Ϣ����
    kWarning    = pclog::kWarning,        // ���漶��
    kTrace      = pclog::kTrace,          // ���ټ���
    kError      = pclog::kError,          // ���󼶱�
    kDebug      = pclog::kDebug           // ���Լ���
};

#define LOG_SHUTDOWN(...)         pclog::print(kShutdown, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_STARTUP(...)          pclog::print(kStartup, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_INFO(...)             pclog::print(kInfo, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_WARN(...)             pclog::print(kWarning, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_TRACE(...)            pclog::print(kTrace, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_ERROR(...)            pclog::print(kError, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_DEBUG(...)            pclog::print(kDebug, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LogHex(level, data, size) pclog::hexdump(level, __FILE__, __LINE__, __FUNCTION__, data, size)

