#include "stdafx.h"
#include <mutex>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <direct.h>
#include <share.h>
#include "log.h"

namespace pclog
{
    namespace ostime
    {
        const wchar_t kSystemSlash = L'\\';
        struct SystemTime
        {
            int year;
            int month;
            int day;
            int hour;
            int minute;
            int second;
            int milli_second;
        };
        SystemTime get_local_time()
        {
            SYSTEMTIME now;
            ::GetLocalTime(&now);
            return SystemTime{ now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute,now.wSecond,now.wMilliseconds };
        }

        SystemTime get_local_time(time_t seconds)
        {
            struct tm tm_now;
            localtime_s(&tm_now, &seconds);
            return SystemTime{ tm_now.tm_year + 1900,tm_now.tm_mon + 1,tm_now.tm_mday,tm_now.tm_hour,tm_now.tm_min,tm_now.tm_sec,0 };
        }

        bool make_directory(const wchar_t* directory)
        {
            return CreateDirectoryW(directory, NULL);
        }


        uint32_t get_thread_id()
        {
            return ::GetCurrentThreadId();
        }

        int vsnprintf(wchar_t* buffer, size_t size, const wchar_t* format, va_list ap)
        {
            return ::_vsnwprintf_s(buffer, size, _TRUNCATE, format, ap);
        }

    }

    namespace detail
    {

        const int  kMaxBufferSize = 4 * 1024;
        const WCHAR kHeaderTimestamp[] = L"[%02u:%02u:%02u.%03u]|";
        const WCHAR kHeaderThreadId[] = L"[%06u]|";
        const WCHAR kHeaderFileAndLine[] = L"[%s:%d]|";
        const WCHAR kHeaderFunction[] = L"[%ws]|";
        const WCHAR kHeaderLogLevel[] = L"[%ws]|";
        const WCHAR kLogFilenameSuffix[] = L"%04u%02u%02u.log";
        const int  kLogFilenameSuffixSize = 16;


        class Log
        {
        public:
            static Log& instance();
            void set_directory(const wchar_t* path);
            void set_prefix_name(const wchar_t* prefix_name);
            void set_level(unsigned int level);
            void set_storage_type(unsigned int storage_type);
            void set_field_type(unsigned int field_type);
            void set_reserve_days(int days);
            void print(unsigned int level, const wchar_t* filename, unsigned int line, const char* function, const wchar_t* format, va_list ap);
            void hexdump(unsigned int level, const wchar_t* filename, unsigned int line, const char* function, const void* data, int size);

        private:
            Log();
            ~Log();
            void write(unsigned int level, const wchar_t* message, int size, const ostime::SystemTime& system_time);
            void write_to_screen(unsigned int level, const wchar_t *message, int size);
            void write_to_stderror(unsigned int level, const wchar_t *message, int size);
            void wirte_to_file(const wchar_t* message, const ostime::SystemTime& system_time);
            void clear_log_file(unsigned int reserve_day);
            int format_line_head(wchar_t* buffer, int size, unsigned int level, const wchar_t*  filename, unsigned int line, const char*  function, const ostime::SystemTime& system_time);
            const wchar_t* level_string(unsigned int level);
            const wchar_t* trim_filename(const wchar_t* filename);
            int sprintf_filename_suffix(wchar_t* buffer, int buffer_size, const ostime::SystemTime& system_time);
            int sprintf_timestamp(unsigned int head_type, wchar_t* buffer, int buffer_size, const ostime::SystemTime& system_time);
            int sprintf_thread_id(unsigned int head_type, wchar_t* buffer, int buffer_size, unsigned int thread_id);
            int sprintf_file_and_line(unsigned int head_type, wchar_t* buffer, int buffer_size, const wchar_t* filename, unsigned int line);
            int sprintf_level(unsigned int head_type, wchar_t* buffer, int buffer_size, unsigned int level);
            int sprintf_function(unsigned int head_type, wchar_t* buffer, int buffer_size, const char* function);
            std::wstring get_log_filename(const wchar_t* suffix);
            std::wstring get_log_filename(time_t log_time);

        private:
            std::mutex lock_;
            unsigned int storage_type_ = kStorageTypeMask;
            unsigned int field_type_ = kFieldTypeMask;
            unsigned int level_ = kLevelMask;
            unsigned int reserve_days_ = 30;  // 日志保留天数，默认30天
            int filename_depth_ = 1;
            std::wstring log_directory_ = L"./log";
            std::wstring prefix_name_ = L"N_A";
            wchar_t old_suffix_[kLogFilenameSuffixSize];
            wchar_t new_suffix_[kLogFilenameSuffixSize];
            FILE* file_ = NULL;

        };

        Log::Log()
        {
        }

        Log::~Log()
        {
        }

        Log& Log::instance()
        {
            static Log log;
            return log;
        }

        void Log::set_directory(const wchar_t* path)
        {
            log_directory_.assign(path);
            if (*log_directory_.rbegin() != ostime::kSystemSlash)
            {
                log_directory_.push_back(ostime::kSystemSlash);
            }
            ostime::make_directory(log_directory_.data());
        }

        void Log::set_prefix_name(const wchar_t* prefix_name)
        {
            prefix_name_ = prefix_name;
        }

        void Log::set_level(unsigned int level)
        {
            level_ = level;
        }

        void Log::set_storage_type(unsigned int storage_type)
        {
            storage_type_ = storage_type;
        }

        void Log::set_field_type(unsigned int field_type)
        {
            field_type_ = field_type;
        }

        void Log::set_reserve_days(int days)
        {
            reserve_days_ = days;
        }

        void Log::print(unsigned int level, const wchar_t* filename, unsigned int line, const char*  function, const wchar_t*  format, va_list ap)
        {
            if ((level_ & level) == 0) return;
            // 日志时间
            ostime::SystemTime system_time = ostime::get_local_time();

            // 初始化行头
            wchar_t buffer[kMaxBufferSize + 2];  // 结尾2个字符'\n\0'
            int pos = format_line_head(buffer, kMaxBufferSize, level, filename, line, function, system_time);

            // 格式化打印数据
            pos += ostime::vsnprintf(buffer + pos, kMaxBufferSize - pos, format, ap);
            if (pos > kMaxBufferSize) pos = kMaxBufferSize - 1;
            buffer[pos++] = L'\n';
            buffer[pos++] = 0;

            // 打印输出
            write(level, buffer, pos, system_time);
        }

        void Log::hexdump(unsigned int level, const wchar_t*  filename, unsigned int line, const char* function, const void* data, int size)
        {
            if (size <= 0)
                return;
            if ((level_ & level) == 0)
                return;

            ostime::SystemTime system_time = ostime::get_local_time();

            std::vector<wchar_t> buffer(size * 5);
            uint32_t pos = format_line_head(buffer.data(), buffer.size(), level, filename, line, function, system_time);

            buffer[pos++] = L'\n';

            for (int i = 0; i < size; i++)
            {
                if (pos + 3 > buffer.size())
                    break;
                pos += _snwprintf_s(buffer.data() + pos, buffer.size() - pos, (buffer.size() - pos) / sizeof(wchar_t), L"%02x", *((unsigned char*)data + i));
                if (((i + 1) % 16) == 0 && pos < buffer.size())
                {
                    buffer[pos++] = L'\n';
                    continue;
                }
                buffer[pos++] = L' ';
            }

            if (buffer[pos - 1] != L'\n')
                buffer[pos++] = L'\n';
            buffer[pos++] = 0;

            // 打印输出
            write(level, buffer.data(), pos, system_time);
        }

        void Log::write(unsigned int level, const wchar_t* message, int size, const ostime::SystemTime& system_time)
        {
            std::lock_guard<std::mutex> guard(lock_);

            if (storage_type_ & kScreen)
                write_to_screen(level, message, size);
            if (storage_type_ & kDiskFile)
                wirte_to_file(message, system_time);
        }

        void Log::write_to_screen(unsigned int level, const wchar_t *message, int size)
        {
            if (level == kError || level == kWarning)
            {
                write_to_stderror(level, message, size);
            }
            else
            {
                fwrite(message, size - 1, 1, stderr);
            }
        }

        void Log::write_to_stderror(unsigned int level, const wchar_t *message, int size)
        {
            const HANDLE stderr_handle = GetStdHandle(STD_ERROR_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO buffer_info;
            GetConsoleScreenBufferInfo(stderr_handle, &buffer_info);
            const WORD old_color_attrs = buffer_info.wAttributes;
            WORD color = 0;

            if (level == kError)
            {
                color = FOREGROUND_RED;
            }
            else if (level == kWarning)
            {
                color = FOREGROUND_GREEN | FOREGROUND_RED;
            }
            fflush(stderr);
            SetConsoleTextAttribute(stderr_handle, color | FOREGROUND_INTENSITY);
            fwrite(message, size - 1, 1, stderr);
            fflush(stderr);

            SetConsoleTextAttribute(stderr_handle, old_color_attrs);
        }

        void Log::wirte_to_file(const wchar_t* message, const ostime::SystemTime& system_time)
        {
            sprintf_filename_suffix(new_suffix_, sizeof(new_suffix_), system_time);
            if (wcscmp(old_suffix_, new_suffix_) != 0)
            {
                if (file_ != NULL) fclose(file_);
                clear_log_file(reserve_days_);
                std::wstring path = get_log_filename(new_suffix_);
                file_ = _wfsopen(path.c_str(), L"a+", _SH_DENYWR);
                memcpy(old_suffix_, new_suffix_, sizeof(old_suffix_));
            }
            if (file_ != NULL)
            {
                fwprintf(file_, L"%ws", message);
                fflush(file_);
            }
        }

        void Log::clear_log_file(unsigned int reserve_day)
        {
            time_t seconds = time(NULL) - reserve_day * 86400;
            for (seconds -= 24 * 3600; ; seconds -= 24 * 3600)
            {
                std::wstring filename = get_log_filename(seconds);
                if (::_wremove(filename.c_str()) != 0)
                {
                    break;
                }
            }
        }

        int Log::format_line_head(wchar_t* buffer, int size, unsigned int level, const wchar_t*  filename, unsigned int line, const char*  function, const ostime::SystemTime& system_time)
        {
            int pos = 0;
            pos += sprintf_timestamp(field_type_, buffer + pos, size - pos, system_time);
            if (pos >= size)
                return size;
            pos += sprintf_thread_id(field_type_, buffer + pos, size - pos, ostime::get_thread_id());
            if (pos >= size)
                return size;
            pos += sprintf_level(field_type_, buffer + pos, size - pos, level);
            if (pos >= size)
                return size;
            pos += sprintf_file_and_line(field_type_, buffer + pos, size - pos, filename, line);
            if (pos >= size)
                return size;
            pos += sprintf_function(field_type_, buffer + pos, size - pos, function);
            if (pos >= size)
                return size;
            return pos;
        }

        const wchar_t* Log::level_string(unsigned int level)
        {
            static const wchar_t* level_strings[] =
            {
                L"Startup",
                L"Shutdown",
                L"Info",
                L"Warning",
                L"Trace",
                L"Error",
                L"Debug",
                L"Unknown"
            };

            switch (level)
            {
            case kStartup:
                return level_strings[0];
            case kShutdown:
                return level_strings[1];
            case kInfo:
                return level_strings[2];
            case kWarning:
                return level_strings[3];
            case kTrace:
                return level_strings[4];
            case kError:
                return level_strings[5];
            case kDebug:
                return level_strings[6];
            }

            return level_strings[7];
        }

        int Log::sprintf_filename_suffix(wchar_t* buffer, int buffer_size, const ostime::SystemTime& system_time)
        {
            return _snwprintf_s(buffer, buffer_size, buffer_size / sizeof(wchar_t), kLogFilenameSuffix, system_time.year, system_time.month, system_time.day);
        }

        int Log::sprintf_timestamp(unsigned int head_type, wchar_t* buffer, int buffer_size, const ostime::SystemTime& system_time)
        {
            if (head_type & kTimestamp)
            {
                return _snwprintf_s(buffer, buffer_size, buffer_size / sizeof(wchar_t), kHeaderTimestamp, system_time.hour, system_time.minute, system_time.second, system_time.milli_second);
            }
            return 0;
        }

        int Log::sprintf_thread_id(unsigned int head_type, wchar_t* buffer, int buffer_size, unsigned int thread_id)
        {
            if (head_type & kThreadId)
            {
                return _snwprintf_s(buffer, buffer_size, buffer_size / sizeof(wchar_t), kHeaderThreadId, thread_id);
            }
            else
            {
                return 0;
            }
        }

        const wchar_t* Log::trim_filename(const wchar_t* filename)
        {
            const wchar_t* r = filename + wcslen(filename) - 1;
            int slash_count = 0;
            while (r-- != filename)
            {
                if (*r == ostime::kSystemSlash)
                    slash_count++;
                if (slash_count == filename_depth_ + 1)
                    return ++r;
            }
            return filename;
        }

        int Log::sprintf_file_and_line(unsigned int head_type, wchar_t* buffer, int buffer_size, const wchar_t* filename, unsigned int line)
        {
            if (head_type & kFileLine)
            {
                return _snwprintf_s(buffer, buffer_size, buffer_size / sizeof(wchar_t), kHeaderFileAndLine, trim_filename(filename), line);
            }
            return 0;
        }

        int Log::sprintf_level(unsigned int head_type, wchar_t* buffer, int buffer_size, unsigned int level)
        {
            return _snwprintf_s(buffer, buffer_size, buffer_size / sizeof(wchar_t), kHeaderLogLevel, level_string(level));
        }

        int Log::sprintf_function(unsigned int head_type, wchar_t* buffer, int buffer_size, const char* function)
        {
            if (head_type & kFunction)
            {
                return _snwprintf_s(buffer, buffer_size, buffer_size / sizeof(wchar_t), kHeaderFunction, function);
            }
            return 0;
        }

        std::wstring Log::get_log_filename(const wchar_t* suffix)
        {
            return log_directory_ + prefix_name_ + suffix;
        }

        std::wstring Log::get_log_filename(time_t seconds)
        {
            ostime::SystemTime system_time = ostime::get_local_time(seconds);
            wchar_t suffix[kLogFilenameSuffixSize];
            sprintf_filename_suffix(suffix, sizeof(suffix), system_time);
            return get_log_filename(suffix);
        }

    }

    void print_v(unsigned int level, const wchar_t*  filename, unsigned int line, const char*  function, const wchar_t*  format, va_list ap)
    {
        detail::Log::instance().print(level, filename, line, function, format, ap);
    }

    void print(unsigned int level, const wchar_t*  filename, unsigned int line, const char*  function, const wchar_t*  format, ...)
    {
        va_list ap;
        va_start(ap, format);
        print_v(level, filename, line, function, format, ap);
        va_end(ap);
    }

    void hexdump(unsigned int level, const wchar_t*  filename, unsigned int line, const char*  function, const void*  data, int size)
    {
        detail::Log::instance().hexdump(level, filename, line, function, data, size);
    }

    void set_directory(const wchar_t* path)
    {
        detail::Log::instance().set_directory(path);
    }

    void set_prefix_name(const wchar_t* prefix_name)
    {
        detail::Log::instance().set_prefix_name(prefix_name);
    }

    void set_level(unsigned int level)
    {
        detail::Log::instance().set_level(level);
    }

    void set_storage_type(unsigned int storage_type)
    {
        detail::Log::instance().set_storage_type(storage_type);
    }

    void set_field_type(unsigned int field_type)
    {
        detail::Log::instance().set_field_type(field_type);
    }

    void set_reserve_days(int days)
    {
        if (days < 1) days = 1;
        detail::Log::instance().set_reserve_days(days);
    }

}

