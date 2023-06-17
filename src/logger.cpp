#include "pch.hpp"
#include "logger.hpp"

namespace ion
{
	Logger::Message::Message(Logger::Level level, std::string&& msg) :
		level(level),
		msg(std::forward<std::string>(msg)),
		time(std::chrono::system_clock::now())
	{
	}

	void Logger::Message::log(std::fstream& file) const
	{
		auto t = std::chrono::system_clock::to_time_t(time);
		auto s = std::localtime(&t);
		char buf[29] = { '\0' };
		std::strftime(buf, 21, "%Y-%m-%dT %H:%M:%S", s);
		const char* color = colors_[static_cast<std::size_t>(level)];

		switch (level)
		{
			case Level::INFO:
				printf("%s %s%s%s %s\n", buf, color, "[INFO]", resetColor_, msg.data());
				file << std::format("{} {} {}\n", buf, "[INFO]", msg.data());
				break;
			case Level::DEBUG:
				printf("%s %s%s%s %s\n", buf, color, "[DEBUG]", resetColor_, msg.data());
				file << std::format("{} {} {}\n", buf, "[DEBUG]", msg.data());
				break;
			case Level::WARNING:
				printf("%s %s%s%s %s\n", buf, color, "[WARN]", resetColor_, msg.data());
				file << std::format("{} {} {}\n", buf, "[WARN]", msg.data());
				break;
			case Level::FATAL:
				printf("%s %s%s%s %s\n", buf, color, "[FATAL]", resetColor_, msg.data());
				file << std::format("{} {} {}\n", buf, "[FATAL]", msg.data());
				break;
			case Level::ERROR:
				printf("%s %s%s%s %s\n", buf, color, "[ERROR]", resetColor_, msg.data());
				file << std::format("{} {} {}\n", buf, "[ERROR]", msg.data());
				break;
			default:
				break;
		}
	}

	std::optional<Logger> Logger::instance_;

	std::array<const char*, 5> Logger::colors_ = {
		"\033[34m", "\033[30m", "\033[33m", "\033[31m", "\033[31m",
	};

	const char* Logger::resetColor_ = "\033[39m\033[49m";

	Logger& Logger::init(std::string_view path)
	{
		namespace Fs = std::filesystem;

		if (instance_.has_value())
			throw std::runtime_error("Logger is already initialized");

		auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		auto s = std::localtime(&t);
		char buf[29] = { '\0' };

		std::strftime(buf, 21, "%Y-%m-%d", s);

		Fs::path fsPath = Fs::path(path);

		if (Fs::is_regular_file(fsPath))
		{
			std::string msg = std::format("Cannot use a file as a log directory! Tried to use {}!", fsPath.string());
			throw std::runtime_error(msg.c_str());
		}

		if (!Fs::exists(fsPath))
			Fs::create_directory(fsPath);

		Fs::path dir = fsPath / buf;

		if (Fs::is_directory(fsPath))
		{
			std::size_t version = 0;

			Fs::path check = dir.string() + ".log";

			while (Fs::exists(check))
			{
				version++;
				check = dir.string() + " (" + std::to_string(version) + ").log";
			}

			std::string p = check.string();
			return instance_.emplace(p, CtorTag{});
		}

		return instance_.emplace(path, CtorTag{});
	}

	Logger& Logger::get()
	{
		assert(instance_.has_value());
		return instance_.value();
	}

	void Logger::dispose()
	{
		assert(instance_.has_value());
		instance_.reset();
	}

	Logger::Logger(std::string_view path, [[maybe_unused]] CtorTag tag) :
		running_(true),
		file_(path.data(), std::ios_base::ate | std::ios_base::out),
		queue_(),
		cv_(),
		mutex_(),
		thread_([&] { threadHandler(); })
	{
		debug("Created log file at", path);
		while (!isReady_.load(std::memory_order::acquire)) continue;
	}

	Logger::~Logger()
	{
		running_.store(false, std::memory_order::release);

		cv_.notify_one();
		thread_.join();

		while (queue_.size() > 0)
		{
			auto msg = queue_.front();
			msg.log(file_);
			queue_.pop();
		}
	}

	void Logger::threadHandler()
	{
		isReady_.store(true, std::memory_order::release);

		while (running_.load(std::memory_order::acquire))
		{
			if (queue_.size() == 0)
			{
				std::unique_lock lock(mutex_);
				cv_.wait(lock, [&] { return !running_.load(std::memory_order::acquire) || queue_.size() > 0; });
			}

			while (queue_.size() > 0)
			{
				std::unique_lock lock(mutex_);
				queue_.front().log(file_);
				queue_.pop();
			}
		}
	}
} // namespace ion