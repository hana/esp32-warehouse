#include <stdio.h>
#include "warehouse.hpp"

#include <filesystem>
#include <string>
#include <string_view>

#include "./lib/nlohmann/json.hpp"

#include "esp_log.h"
#include "esp_littlefs.h"

void warehouse_t::test() {
    static_assert(__cplusplus >= 202302L);
    init();
    load();
}

void warehouse_t::init(void)  {
    // mount
    constexpr auto TAG = "Warehouse::init";
    const auto base_path = std::string("/") + std::string(CONFIG_WAREHOUSE_PARTITION_LABEL);
    if(!esp_littlefs_mounted(CONFIG_WAREHOUSE_PARTITION_LABEL)) {
        esp_vfs_littlefs_conf_t conf = {
            .base_path = base_path.c_str(),
            .partition_label = "storage",
            .format_if_mount_failed = true,
            .dont_mount = false,        
        };

        if(const esp_err_t ret = esp_vfs_littlefs_register(&conf); ret != ESP_OK) {
            switch(ret) {
                case ESP_FAIL:
                    ESP_LOGE(TAG, "Failed to mount or format filesystem");
                    break;                
                case ESP_ERR_NOT_FOUND:
                    ESP_LOGE(TAG, "Failed to find LittleFS partition");
                    break;
                default:
                    ESP_LOGE(TAG, "Failed to initialize LittleFS (%s)", esp_err_to_name(ret));
                    break;
            }
            return;
        } else {
            ESP_LOGD(TAG, "Successfully mounted littefs partition: %s", CONFIG_WAREHOUSE_PARTITION_LABEL);
        }        
    }
}

void warehouse_t::load() {
    
    // if (!mounted) mount(); 
    constexpr auto TAG = "Wavehouse::load";

    namespace fs = std::filesystem;
    fs::path path = CONFIG_WAREHOUSE_PARTITION_LABEL;
    path /= CONFIG_WAREHOUSE_DEFAULT_FILENAME;
    path += get_extension();
    
    auto file = fopen(fs::absolute(path).c_str(), "rb");
    if(file == nullptr) {                      
        ESP_LOGE(TAG, "open failed.");        
        return;                   
    } else {
        // if (!json::accept(file)) { // check if the file is a valid json
        //     log_d("Invalid JSON file.");            
        // } else { 
            // actual load                
                using namespace nlohmann;
                try {
                    data = json::from_msgpack(file, true, true);
                } catch (json::parse_error& ex) {
                    ESP_LOGE(TAG, "Failed to load data: %d", ex.byte);
                    data.clear();
                }

                if(data.is_discarded()) {
                    ESP_LOGE(TAG, "Failed to load data.");
                    data.clear();
                }

        // }
    }

    fclose(file);
}

void warehouse_t::save() {
    // if (!mounted) mount();
    constexpr auto TAG = "Wavehouse::save";

    namespace fs = std::filesystem;
    fs::path path = CONFIG_WAREHOUSE_PARTITION_LABEL;
    path /= CONFIG_WAREHOUSE_DEFAULT_FILENAME;
    path += get_extension();

    auto file = fopen(fs::absolute(path).c_str(), "wb");
    if(file == nullptr) {
        ESP_LOGE(TAG, "open failed");
    } else {
        // if(save_as_raw) {
            using namespace nlohmann;
            const auto v = json::to_msgpack(data);
            fwrite(v.data(), v.size(), 1, file);
            ESP_LOGI(TAG, "Data written: %d bytes.", v.size());
        // } else {
        //     fprintf(file, "%s", data.dump().c_str());
        //     log_d("Data written: %d bytes.", data.dump().size());
        // }
    }
    fclose(file);    
}
