#include <app.hxx>

namespace app {
    namespace CustomImGui {
        bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0) {
            char buf[256];

            std::strncpy(buf, str->c_str(), sizeof(buf));

            if (ImGui::InputText(label, buf, sizeof(buf), flags)) {
                *str = buf;

                return true;
            }

            return false;
        }
    } // namespace CustomImGui

    void c_renderer::render(model_t& model, c_app& app) {
        ImGui::Begin("CAN Analyzer");

        {
            {
                if (ImGui::Button("Load Dictionary"))
                    app.load_dict();

                ImGui::SameLine();

                ImGui::Text("Dictionary: %s", model.m_cfg.m_dict_path.c_str());
            }

            ImGui::Separator();

            {
                CustomImGui::InputText("CAN Interface Name", &model.m_cfg.m_can_interface_name);

                if (ImGui::Button("Connect to CAN"))
                    app.connect_can();

                ImGui::SameLine();

                ImGui::Text("Status: %s", app.is_can_connected() ? "Connected" : "Disconnected");
            }

            {
                CustomImGui::InputText("PostgreSQL DSN", &model.m_cfg.m_db_connection_str);

                if (ImGui::Button("Connect to DB"))
                    app.connect_db();

                ImGui::SameLine();

                ImGui::Text("Status: %s", app.is_db_connected() ? "Connected" : "Disconnected");
            }

            ImGui::Separator();

            ImGui::Text("Total Packets Received: %ld", model.m_total_packets_received);
            ImGui::Text("Total Bytes Received: %ld", model.m_total_bytes_received);

            if (!model.m_error_msg.empty()) {
                ImGui::Spacing();
                ImGui::Separator();

                ImGui::TextColored(
                    ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Error: %s", model.m_error_msg.c_str());
            }
            else {
                ImGui::Spacing();
                ImGui::Separator();

                ImGui::Text("Error: No error");
            }
        }

        ImGui::End();

        ImGui::Begin("Tracked Messages");

        {
            if (ImGui::BeginTable("Messages", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
                ImGui::TableSetupColumn("ID (hex)");
                ImGui::TableSetupColumn("Name");
                ImGui::TableSetupColumn("Count");
                ImGui::TableSetupColumn("Last Seen");
                ImGui::TableSetupColumn("Last Data (hex)");

                ImGui::TableHeadersRow();

                for (const auto& [id, message] : model.m_tracked_msgs) {
                    ImGui::TableNextRow();

                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%X", id);

                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%s", message.m_name.c_str());

                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%ld", message.m_msg_count);

                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%s", shared::fmt_time_point(message.m_last_seen).c_str());

                    ImGui::TableSetColumnIndex(4);
                    ImGui::Text("%s", shared::fmt_data_to_hex(message.m_data).c_str());
                }

                ImGui::EndTable();
            }
        }

        ImGui::End();
    }
} // namespace app