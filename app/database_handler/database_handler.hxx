#ifndef APP_DATABASE_HANDLER_HXX
#define APP_DATABASE_HANDLER_HXX

namespace app {
    class c_database_handler {
       public:
        c_database_handler();

        ~c_database_handler();

       public:
        bool connect(const std::string& db_connection_str, std::string& error_msg);

        void disconnect();

        bool is_connected() const;

       public:
        bool upsert_message(const message_t& message, std::string& error_msg);

       private:
        std::unique_ptr<pqxx::connection> m_connection{};
    };
} // namespace app

#endif // APP_DATABASE_HANDLER_HXX