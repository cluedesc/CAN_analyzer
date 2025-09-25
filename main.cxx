#include <app.hxx>

int main() {
    try {
        app::c_app app{};

        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;

        return 1;
    }

    return 0;
}