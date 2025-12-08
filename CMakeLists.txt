idf_component_register(SRCS "warehouse.cpp" "lib/Singleton/Singleton.cpp"

                    INCLUDE_DIRS "include" "lib/nlohmann" "lib/Singleton")

# littlefs_create_partition_image(storage ../flash_data FLASH_IN_PROJECT)

# set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS ON)

# target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++23 -fexceptions)
target_compile_options(${COMPONENT_LIB} PRIVATE -fexceptions)