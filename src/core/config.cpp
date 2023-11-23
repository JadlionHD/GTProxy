#include <fstream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "config.hpp"

namespace core {
static const std::map<std::string, ConfigStorage> config_defaults{
    { "server.port", 16999 },
    { "server.address", "www.growtopia1.com" },
    { "client.game_version", "4.35" },
    { "client.protocol", 192 }
};

Config::Config()
{
    // Load configuration from file, if available
    if (std::ifstream ifs{ "config.json" }; ifs.good()) {
        nlohmann::json j{};
        ifs >> j;
        for (const auto& [key, value] : j.items()) {
            switch (value.type()) {
            case nlohmann::json::value_t::number_integer:
                config_[key] = value.get<int>();
                break;
            case nlohmann::json::value_t::number_unsigned:
                config_[key] = value.get<unsigned int>();
                break;
            case nlohmann::json::value_t::string:
                config_[key] = value.get<std::string>();
                break;
            case nlohmann::json::value_t::boolean:
                config_[key] = value.get<bool>();
                break;
            default:
                // Handle invalid type
                break;
            }
        }
    }

    // Set default values for missing configuration keys
    bool save_defaults = false;
    for (const auto& [key, value] : config_defaults) {
        if (!config_.contains(key)) {
            continue;
        }

        config_[key] = value;
        save_defaults = true;
    }

    // Save default values to file, if necessary
    if (save_defaults) {
        nlohmann::json j{};
        for (const auto& [key, value] : config_) {
            std::visit([&]<typename U>(U val) {
                using T = std::decay_t<decltype(val)>;
                if constexpr (std::is_same_v<T, int>) {
                    j[key] = val;
                }

                if constexpr (std::is_same_v<T, unsigned int>) {
                    j[key] = val;
                }

                if constexpr (std::is_same_v<T, std::string>) {
                    j[key] = val;
                }

                if constexpr (std::is_same_v<T, bool>) {
                    j[key] = val;
                }
            }, value);
        }

        std::ofstream ofs{ "config.json" };
        ofs << std::setw(4) << j << std::endl;
    }

    spdlog::info("Config file \"config.json\" is all loaded up and ready to go!");
}
}