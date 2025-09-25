#include <app.hxx>

namespace app {
    c_database_handler::c_database_handler() = default;

    c_database_handler::~c_database_handler() {
        disconnect();
    }

    bool c_database_handler::connect(const std::string& db_connection_str, std::string& error_msg) {
        try {
            disconnect();

            m_connection = std::make_unique<pqxx::connection>(db_connection_str);

            if (m_connection->is_open()) {
                error_msg.clear();

                return true;
            }
        } catch (const std::exception& e) {
            error_msg = std::string("Error while connecting to database: ") + e.what();
        }

        return false;
    }

    void c_database_handler::disconnect() {
        m_connection.reset();
    }

    bool c_database_handler::is_connected() const {
        return m_connection && m_connection->is_open();
    }

    bool c_database_handler::upsert_message(const message_t& message, std::string& error_msg) {
        if (!is_connected()) {
            error_msg = "Not connected to database";

            return false;
        }

        try {
            pqxx::work txn(*m_connection);

            std::string sql_str = R"(
                INSERT INTO can_messages (id, name, last_seen, message_count, last_data)
                VALUES ($1, $2, NOW(), 1, $3)
                ON CONFLICT (id) DO UPDATE SET
                    last_seen = EXCLUDED.last_seen,
                    message_count = can_messages.message_count + 1,
                    last_data = EXCLUDED.last_data;
            )";

            pqxx::binarystring bstr(message.m_data.data(), message.m_data.size());

            txn.exec_params(sql_str, message.m_id, message.m_name, bstr);

            txn.commit();

            error_msg.clear();

            return true;
        } catch (const std::exception& e) {
            error_msg = std::string("Error while upserting message: ") + e.what();

            disconnect();

            return false;
        }
    }
} // namespace app