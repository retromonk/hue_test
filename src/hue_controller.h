#include <memory>
#include <functional>

#include <sigc++/sigc++.h>

namespace hue
{
class IHueController
{
    public:
        virtual ~IHueController() = default;
        virtual void registerApplication() = 0;
        virtual void startPollingLights() = 0;

        sigc::signal<void(const std::string& status)> signal_lights_changed;
};

class HueController : public IHueController
{
    public:
        HueController(const std::string& hostname, int port);
        void registerApplication() override;
        void startPollingLights() override;
    private:
        std::unique_ptr<IHueController> impl_;
};
}