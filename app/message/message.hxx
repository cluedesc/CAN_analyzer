#ifndef APP_MESSAGE_HXX
#define APP_MESSAGE_HXX

namespace app {
    struct message_t {
        std::uint32_t m_id{};

        std::string m_name{};

        std::int64_t m_msg_count{};

        std::vector<std::uint8_t> m_data{};

        std::chrono::system_clock::time_point m_last_seen{};
    };
} // namespace app

#endif // APP_MESSAGE_HXX