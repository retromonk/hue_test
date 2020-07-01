#include "hue_controller.h"

#include <CLI/CLI.hpp>

#include <exception>

int main(int argc, char** argv) {
    CLI::App app{"Hue Controller"};
    std::string hostname;
    app.add_option("-n, --hostname", hostname, "IP address of the host")->required();
    int port = 80;
    app.add_option("-p, --port", port, "Port of the host");
    CLI11_PARSE(app, argc, argv);
 
    hue::HueController hueController(hostname, port);
    try {
        hueController.registerApplication();
    } catch (const std::string& msg) {
        std::cerr << "Failed to register the application with the device: " << msg << std::endl;
        exit(-1);
    }  

    hueController.signal_lights_changed.connect([](const std::string& output) {
        std::cout << output << std::endl;
    });

    hueController.startPollingLights();

    return 0;    
}