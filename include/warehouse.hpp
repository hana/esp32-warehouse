#pragma once

#include <string>

// #include "esp_littlefs.h"

#include "../lib/nlohmann/json.hpp"

class warehouse_t {
public:
    void test();
    void init();

    void save();
    void load();
    
    template<typename T>
    void set(const std::string& key, const T& val) {
        data[key] = val;
    }

    template<typename T>
    const T get(const char* key, const T value_if_missing = T()) {
        if(data.count(key)) {
            return data[key];         
        } else {
            data[key] = value_if_missing;                        
            return value_if_missing;
        }
    }

    template<typename T>
    const T& get(const std::string& key, const T value_if_missing = T())  {
        get(key.c_str(), value_if_missing);
    }

    static constexpr auto get_extension() {
    #if CONFIG_WAREHOUSE_SAVE_MPAK
        return std::string(".mpak");
    #else
        return std::string(".json");
    #endif
    };


private:
    nlohmann::json data;
};

#include "../lib/Singleton/Singleton.hpp"
using Warehouse = Singleton<warehouse_t>;


