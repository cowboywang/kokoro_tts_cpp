#pragma once

#ifdef LOGDLL_EXPORTS
#define LOG_API __declspec(dllexport)
#else
#define LOG_API __declspec(dllimport)
#endif

LOG_API void SetMainThreadId(int main_thread_id);
LOG_API void TTSLog(const char* fmt, ...);