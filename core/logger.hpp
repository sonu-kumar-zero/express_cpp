#pragma once
#include <fstream>
#include <string>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <filesystem>

namespace fs = std::filesystem;

struct Logger
{
private:
    std::ofstream file_;
    std::mutex mtx_;

    std::string timestamp()
    {
        using namespace std::chrono;
        auto now = system_clock::now();
        auto now_time_t = system_clock::to_time_t(now);

        std::tm utc_tm = *std::gmtime(&now_time_t);

        utc_tm.tm_hour += 5;
        utc_tm.tm_min += 30;

        std::mktime(&utc_tm);

        std::stringstream ss;
        ss << std::put_time(&utc_tm, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

public:
    Logger(const std::string &filePath = "")
    {
        std::string finalPath = filePath;

        if (finalPath.empty())
        {
            std::string logDir = "logs";

            if (!fs::exists(logDir))
            {
                fs::create_directories(logDir);
            }

            finalPath = logDir + "/server.log";
        }

        file_.open(finalPath, std::ios::app);

        if (!file_.is_open())
        {
            throw std::runtime_error("Failed to open log file:" + filePath);
        }
    }

    ~Logger()
    {
        if (file_.is_open())
            file_.close();
    }

    void log(const std::string &message)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        file_ << "[" << timestamp() << "]" << message << std::endl;
        file_.flush();
    }
};
