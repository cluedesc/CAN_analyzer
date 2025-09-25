#ifndef APP_RENDERER_HXX
#define APP_RENDERER_HXX

namespace app {
    class c_app;

    struct model_t;

    class c_renderer {
       public:
        void render(model_t& model, c_app& app);
    };
} // namespace app

#endif // APP_RENDERER_HXX