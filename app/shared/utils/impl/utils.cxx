#include <app.hxx>

namespace shared {
    std::string fmt_data_to_hex(const std::vector<std::uint8_t>& data) {
        std::stringstream ss;

        for (const auto& byte : data)
            ss << std::setw(2) << static_cast<int>(byte) << ' ';

        return ss.str();
    }

    std::string fmt_time_point(const std::chrono::system_clock::time_point& time_point) {
        auto time = std::chrono::system_clock::to_time_t(time_point);
        auto tm = *std::localtime(&time);

        std::stringstream ss;

        ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

        return ss.str();
    }
} // namespace shared