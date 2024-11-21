#define HTTP_PORT 80

#define WEBSOCKETS_URL "/ws"

#define STA_SSID "flea"
#define STA_PASSPHARSE "flea1234"

#define STA_IP "192.168.0.1"
#define STA_GATEWAY "192.168.0.1"
#define STA_NETMASK "255.255.255.0"

#define JSON_BUFFER_SIZE 2048

/**
 * Enables the LittleFS File System feature
 */
#define ENABLE_LITTLEFS true

/**
 * Enable to read input from a serial console
 */
#define ENABLE_SERIAL_READ true

/**
 * Enable the 16-channel PWM Controller
 */
#define ENABLE_PWM false

/**
 * Enable the I2C Display module
 */
#define ENABLE_I2C_DISPLAY false


/**
 * ESP-8266 specific features
 */
#ifdef ESP8266

/**
 * Enable DHT22 Sensor module. Doesn't work with ESP32 because of conflicting sensor_t types
 */
#define ENABLE_DHT22 true

#endif


/**
 * ESP32 specific features
 */
#ifdef ESP32

/**
 * Enables the ESP32 Camera module
 */
#define ENABLE_CAMERA true

/**
 * Enables the ESP32 Flashlight module
 */
#define ENABLE_FLASHLIGHT true

#endif

/**
 * ESP8266 specific features
 */
#ifdef ESP8266

#endif

