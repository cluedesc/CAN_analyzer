#include <app.hxx>

int main(int argc, char* argv[]) {
    int width{-1};
    int height{-1};

    for (auto i{1}; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--width" && i + 1 < argc) {
            width = std::stoi(argv[++i]);
        } else if (arg == "--height" && i + 1 < argc) {
            height = std::stoi(argv[++i]);
        } else
            std::cerr << "Unknown argument: " << arg << std::endl;
    }

    try {
        app::c_app app{};

        if (width != -1 && height != -1)
            app.override_app_size(width, height);

        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;

        return 1;
    }

    return 0;
}