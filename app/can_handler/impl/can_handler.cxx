#include <app.hxx>

namespace app {
    c_can_handler::c_can_handler() = default;

    c_can_handler::~c_can_handler() {
        disconnect();
    }

    bool c_can_handler::connect(const std::string& can_interface_name, std::string& error_msg) {
        disconnect();

        m_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);

        if (m_socket < 0) {
            error_msg = "Error while opening CAN socket";

            return false;
        }

        auto flags = fcntl(m_socket, F_GETFL, 0);

        if (flags < 0) {
            error_msg = std::string("fcntl(F_GETFL) failed: ") + std::strerror(errno);

            disconnect();

            return false;
        }

        if (fcntl(m_socket, F_SETFL, flags | O_NONBLOCK) < 0) {
            error_msg = std::string("fcntl(F_SETFL) failed: ") + std::strerror(errno);

            disconnect();

            return false;
        }

        struct ifreq ifr;

        std::memset(&ifr, 0, sizeof(ifr));

        strncpy(ifr.ifr_name, can_interface_name.c_str(), IFNAMSIZ - 1);

        ifr.ifr_name[IFNAMSIZ - 1] = '\0';

        if (ioctl(m_socket, SIOCGIFINDEX, &ifr) < 0) {
            error_msg = "Error while getting CAN interface index";

            disconnect();

            return false;
        }

        sockaddr_can addr{};

        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;

        if (bind(m_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
            error_msg = "Error while binding CAN socket";

            disconnect();

            return false;
        }

        error_msg.clear();

        return true;
    }

    void c_can_handler::disconnect() {
        if (is_connected()) {
            close(m_socket);

            m_socket = -1;
        }
    }

    bool c_can_handler::is_connected() const {
        return m_socket >= 0;
    }

    ssize_t c_can_handler::read_frame(can_frame& frame) {
        if (!is_connected())
            return 0;

        auto n = recv(m_socket, &frame, sizeof(frame), 0);

        if (n < 0)
            return -1;

        return n;
    }
} // namespace app