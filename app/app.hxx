#ifndef APP_HXX
#define APP_HXX

#include "shared/shared.hxx"

#include <message/message.hxx>

#include "can_handler/can_handler.hxx"

#include "database_handler/database_handler.hxx"

#include "renderer/renderer.hxx"

struct GLFWwindow;

namespace app {
    struct cfg_t {
        std::string m_dict_path{};

        std::string m_can_interface_name{"vcan0"};

        std::string m_db_connection_str{
            "dbname=test user=test password=test hostaddr=127.0.0.1 port=5432"};
    };

    struct model_t {
        cfg_t m_cfg{};

        std::map<std::uint32_t, std::string> m_dict{};

        std::map<std::uint32_t, app::message_t> m_tracked_msgs{};

        std::int64_t m_total_packets_received{};

        std::int64_t m_total_bytes_received{};

        std::string m_error_msg{};
    };

    class c_app {
       public:
        c_app();

        ~c_app();

       public:
        void run();

        void load_dict();

        void connect_can();

        bool is_can_connected() const;

        void connect_db();

        bool is_db_connected() const;

       private:
        void init();

        void update();

        void cleanup();

        void process_can_frame();

       private:
        GLFWwindow* m_window{};

        model_t m_model{};

        c_can_handler m_can_handler{};

        c_database_handler m_db_handler{};

        c_renderer m_renderer{};
    };
} // namespace app

#endif // APP_HXX