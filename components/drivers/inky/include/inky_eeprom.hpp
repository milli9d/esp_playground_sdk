/**
 * Read the EEPROM on an Inky display
 *
 * This can fetch dislplay manufacturing data
 *
 * @author Milind Singh
 */

#include "driver/i2c.h"

#define INKY_EEPROM_I2C_SCL         GPIO_NUM_5          /* gpio number for I2C master clock */
#define INKY_EEPROM_I2C_SDA         GPIO_NUM_4          /* gpio number for I2C master data  */
#define INKY_EEPROM_I2C_NUM         I2C_NUM_0           /* I2C port number for master dev */

#define INKY_EEPROM_I2C_ADDR        0xA0u

#define I2C_TICKS_TO_WAIT               300u
#define INKY_EEPROM_I2C_TICKS_TO_WAIT   (I2C_TICKS_TO_WAIT / portTICK_RATE_MS)

#define TAG_INKY_EEPROM         "InkyEEPROM"

namespace ESPP
{

    typedef struct inky_eeprom_sys_s {
        gpio_num_t scl;
        gpio_num_t sda;
        i2c_port_t i2c_port;
    } inky_eeprom_sys_t;


    class inky_eeprom_i2c{
        public:

        private:
        

    };

    class inky_eeprom
    {
    public:
        inky_eeprom(void) = delete;

        inky_eeprom(inky_eeprom_sys_t eeprom);

        /* public API */

    private:
        i2c_config_t _i2c_sys;
        inky_eeprom_sys_t _eeprom;

        /* private API */
        esp_err_t _i2c_init(void);
        esp_err_t _eeprom_access_test(void);
        
    };
}