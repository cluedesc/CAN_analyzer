#include <app.hxx>

namespace app {
    c_app::c_app() = default;

    c_app::~c_app() {
        cleanup();
    }

    void c_app::run() {
        init();

        update();
    }

    void c_app::override_app_size(int width, int height) {
        m_width = width;
        m_height = height;
    }

    void c_app::init() {
        if (!glfwInit())
            throw std::runtime_error("Error while initializing GLFW");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(m_width, m_height, "CAN Analyzer", nullptr, nullptr);

        if (!m_window) {
            glfwTerminate();

            throw std::runtime_error("Error while creating GLFW window");
        }

        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(1);

        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void c_app::update() {
        while (!glfwWindowShouldClose(m_window)) {
            glfwPollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();

            ImGui::NewFrame();

            {
                m_renderer.render(m_model, *this);

                process_can_frame();
            }

            ImGui::Render();

            std::int32_t w{};
            std::int32_t h{};

            glfwGetFramebufferSize(m_window, &w, &h);

            glViewport(0, 0, w, h);

            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(m_window);
        }
    }

    void c_app::cleanup() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();

        ImGui::DestroyContext();

        if (m_window)
            glfwDestroyWindow(m_window);

        glfwTerminate();
    }

    void c_app::process_can_frame() {
        if (!m_can_handler.is_connected())
            return;

        can_frame frame;

        if (m_can_handler.read_frame(frame) > 0) {
            m_model.m_total_packets_received++;

            m_model.m_total_bytes_received += frame.can_dlc;

            auto dict_it = m_model.m_dict.find(frame.can_id);

            if (dict_it != m_model.m_dict.end()) {
                auto& message = m_model.m_tracked_msgs[frame.can_id];

                {
                    message.m_id = frame.can_id;
                    message.m_name = dict_it->second;
                    message.m_msg_count++;
                    message.m_last_seen = std::chrono::system_clock::now();

                    message.m_data.assign(frame.data, frame.data + frame.can_dlc);
                }

                m_db_handler.upsert_message(message, m_model.m_error_msg);
            }
        }
    }

    void c_app::load_dict() {
        char const* filter_patterns[1] = {"*.json"};
        char const* file_path =
            tinyfd_openFileDialog("Open Dictionary", "", 1, filter_patterns, "JSON Files", 0);

        if (!file_path) {
            m_model.m_error_msg = "No file selected";

            return;
        }

        m_model.m_cfg.m_dict_path = file_path;

        std::ifstream f(file_path);

        if (!f.is_open()) {
            m_model.m_error_msg = std::string("Error while opening file: ") + file_path;

            return;
        }

        try {
            auto data = nlohmann::json::parse(f);

            m_model.m_dict.clear();
            m_model.m_tracked_msgs.clear();

            for (const auto& [key, value] : data.items()) {
                auto id = std::stoull(key, nullptr, 16);

                m_model.m_dict[id] = value.get<std::string>();
            }

            m_model.m_error_msg.clear();
        } catch (const std::exception& e) {
            m_model.m_error_msg = std::string("Error while parsing file: ") + file_path;
        }
    }

    void c_app::connect_can() {
        m_can_handler.connect(m_model.m_cfg.m_can_interface_name, m_model.m_error_msg);
    }

    void c_app::connect_db() {
        m_db_handler.connect(m_model.m_cfg.m_db_connection_str, m_model.m_error_msg);
    }

    bool c_app::is_can_connected() const {
        return m_can_handler.is_connected();
    }

    bool c_app::is_db_connected() const {
        return m_db_handler.is_connected();
    }
} // namespace app