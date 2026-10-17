#pragma once

#include <cstddef>
#include <iostream>
#include <mutex>
#include <ostream>
#include <string>

enum class LogLevel {
	Trace = 0,
	Info = 1,
	Warning = 2,
	Error = 3,
	Fatal = 4
};

class Logger final {
public:
	static Logger& Instance() {
		static Logger instance;
		return instance;
	}

	void SetLevel(LogLevel level) { level_ = level; }
	void SetOutputStream(std::ostream& stream) { stream_ = &stream; }

	void Log(LogLevel level, const std::string& message) {
		if (level < level_) return;

		const char* label = "INFO";
		switch (level) {
		case LogLevel::Trace: label = "TRACE"; break;
		case LogLevel::Info: label = "INFO"; break;
		case LogLevel::Warning: label = "WARN"; break;
		case LogLevel::Error: label = "ERROR"; break;
		case LogLevel::Fatal: label = "FATAL"; break;
		}

		std::lock_guard<std::mutex> guard(mutex_);
		(*stream_) << "[" << label << "] " << message << '\n';
	}

	void Trace(const std::string& message) { Log(LogLevel::Trace, message); }
	void Info(const std::string& message) { Log(LogLevel::Info, message); }
	void Warning(const std::string& message) { Log(LogLevel::Warning, message); }
	void Error(const std::string& message) { Log(LogLevel::Error, message); }
	void Fatal(const std::string& message) { Log(LogLevel::Fatal, message); }

private:
	Logger() = default;

	std::ostream* stream_{ &std::cerr };
	LogLevel level_{ LogLevel::Info };
	std::mutex mutex_;
};

inline Logger& GetLogger() { return Logger::Instance(); }
inline void SetLogLevel(LogLevel level) { GetLogger().SetLevel(level); }
inline void SetLogOutput(std::ostream& stream) { GetLogger().SetOutputStream(stream); }
inline void LogTrace(const std::string& message) { GetLogger().Trace(message); }
inline void LogInfo(const std::string& message) { GetLogger().Info(message); }
inline void LogWarning(const std::string& message) { GetLogger().Warning(message); }
inline void LogError(const std::string& message) { GetLogger().Error(message); }
inline void LogFatal(const std::string& message) { GetLogger().Fatal(message); }
