idf_component_register(
        SRCS "Vernon_GT911/Vernon_GT911.c"
        INCLUDE_DIRS "Vernon_GT911/includes"
        REQUIRES driver esp_driver_i2c esp_driver_gpio)