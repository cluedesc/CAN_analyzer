#ifndef APP_CAN_HANDLER_HXX
#define APP_CAN_HANDLER_HXX

namespace app {
    class c_can_handler {
       public:
        c_can_handler();

        ~c_can_handler();

       public:
        bool connect(const std::string& can_interface_name, std::string& error_msg);

        void disconnect();

        bool is_connected() const;

       public:
        ssize_t read_frame(can_frame& frame);

       private:
        std::int32_t m_socket{-1};
    };
} // namespace app

#endif // APP_CAN_HANDLER_HXX