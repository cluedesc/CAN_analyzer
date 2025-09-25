#ifndef SHARED_UTILS_HXX
#define SHARED_UTILS_HXX

namespace shared {
    std::string fmt_data_to_hex(const std::vector<std::uint8_t>& data);

    std::string fmt_time_point(const std::chrono::system_clock::time_point& time_point);
} // namespace shared

#endif // SHARED_UTILS_HXX