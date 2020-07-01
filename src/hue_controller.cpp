#include "hue_controller.h"

#include <httplib.h>
#include <nlohmann/json.hpp>

#include <chrono>
#include <thread>

namespace hue
{
class HueControllerImpl : public IHueController {
    public:
        HueControllerImpl(const std::string& hostname, int port) : client_(hostname, port) {
        }

        void registerApplication() override {
            const std::string endpoint = "/api";
            nlohmann::json payload;
            payload["devicetype"] = app_name_;
            auto response = client_.Post(endpoint.c_str(), payload.dump(), "application/json");
            if (response == nullptr || response->status != 200) {
                throw "Failed to connect to remote endpoint";
            }

            auto result = nlohmann::json::parse(response->body);
            if (!result.is_array()) {
                throw response->body;
            }

            auto errorObject = result[0].find("error");
            if (errorObject != result[0].end()) {
                throw errorObject->dump();
            }

            auto successObject = result[0].find("success");
            if (successObject != result[0].end()) {
                successObject->at("username").get_to(username_);
            } else {
                throw "Unknown Error";
            }
        }

        void startPollingLights() override {
            std::string endpoint = "/api/";
            endpoint += username_;
            endpoint += "/lights";
            auto firstTime = true;
            while (true) {
                auto response = client_.Get(endpoint.c_str());
                auto result = nlohmann::json::parse(response->body);
                for (const auto& iter : result.items()) {
                    std::string id;
                    id = iter.key();
                    addOrUpdateLightData(id, iter);
                }

                if (firstTime) {
                    firstTime = false;
                    nlohmann::json result = nlohmann::json::array();
                    for (const auto& iter : light_cache_) {
                        nlohmann::json entry;
                        entry["id"] = iter.first;
                        entry["on"] = iter.second.on;
                        entry["brightness"] = iter.second.brightness;
                        entry["name"] = iter.second.name;
                        result.push_back(entry);
                    }

                    signal_lights_changed(result.dump());
                }

                using namespace std::chrono_literals;
                std::this_thread::sleep_for(2s);
            }
        }

    private:
        struct LightStatus {
            std::string name;
            bool on;
            int brightness;
        };

        std::map<std::string, LightStatus> light_cache_;

        std::string hostname_;
        httplib::Client client_;
        std::string username_;
        const std::string app_name_ = "sample2#hue_app";

        void addOrUpdateLightData(const std::string& id, const nlohmann::json& jsonObj) {
            auto subObj = jsonObj.find(id);
            std::string name;
            subObj->at("name").get_to(name);
            bool on;
            subObj->at("state").at("on").get_to(on);
            int brightness;
            subObj->at("state").at("bri").get_to(brightness);
            brightness = brightness / 254. * 100; 

            auto lightIter = light_cache_.find(id);
            if (lightIter == light_cache_.end()) {
                light_cache_[id] = {
                    name,
                    on,
                    brightness
                };
            } else {
                if (lightIter->second.on != on) {
                    nlohmann::json result;
                    result["id"] = id;
                    result["on"] = on;
                    signal_lights_changed(result.dump());
                    lightIter->second.on = on;
                }
                
                if (lightIter->second.brightness != brightness) {
                    nlohmann::json result;
                    result["id"] = id;
                    result["brightness"] = brightness;
                    signal_lights_changed(result.dump());
                    lightIter->second.brightness = brightness;
                }
            }
        }
};

HueController::HueController(const std::string& hostname, int port) : impl_(std::make_unique<HueControllerImpl>(hostname, port)) {
    impl_->signal_lights_changed.connect(signal_lights_changed);
}

void HueController::registerApplication() {
    impl_->registerApplication();
}

void HueController::startPollingLights() {
    impl_->startPollingLights();
}
}