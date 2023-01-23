/**
 * Read the EEPROM CAT24C32 on an Inky display
 *
 * This can fetch dislplay manufacturing data
 *
 * @author Milind Singh
 */

#include "esp_log.h"
#include "driver/i2c.h"
#include "esp_err.h"

#include "inky_eeprom.hpp"

namespace ESPP
{
    esp_err_t inky_eeprom::_eeprom_access_test(void)
    {
        esp_err_t ret = ESP_OK;

        /* REQUEST DATA FROM R0 */
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);

        /* queue slave address [1-byte]*/
        i2c_master_write_byte(cmd, INKY_EEPROM_I2C_ADDR, true);
        /* queue register address [2-byte] */
        i2c_master_write_byte(cmd, 0x00u, true);
        i2c_master_write_byte(cmd, 0x00u, true);
        i2c_master_stop(cmd);

        /* flush queue to I2C bus */
        ret = i2c_master_cmd_begin(_eeprom.i2c_port, cmd, INKY_EEPROM_I2C_TICKS_TO_WAIT);
        i2c_cmd_link_delete(cmd);

        /* READ slave data */
        cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);

        /* write slave address [1-byte]*/
        i2c_master_write_byte(cmd, INKY_EEPROM_I2C_ADDR, true);

        /* read R0 value */
        uint8_t data = 0u;
        i2c_master_read_byte(cmd, &data, i2c_ack_type_t::I2C_MASTER_ACK);

        i2c_master_stop(cmd);

        ret = i2c_master_cmd_begin(_eeprom.i2c_port, cmd, INKY_EEPROM_I2C_TICKS_TO_WAIT);
        i2c_cmd_link_delete(cmd);

        if (ret != ESP_OK) {
            ESP_LOGE(TAG_INKY_EEPROM, "I2C Access Test Failed.");
        } else {
            ESP_LOGI(TAG_INKY_EEPROM, "I2C Access Test Passed! Read R0 : %2x", data);
        }

        return ret;
    }

    /**
     *
     */
    esp_err_t inky_eeprom::_i2c_init(void)
    {
        /**
         * I2C config for EEPROM CAT24C32
         *
         *  MASTER Mode
         *  SCL : Pull-Up
         *  SDA : Pull-UP
         *  I2C Speed : Standard, Fast and Fast−Plus
         *
         *
         */
        _i2c_sys.mode = I2C_MODE_MASTER;
        _i2c_sys.scl_pullup_en = GPIO_PULLUP_ENABLE;
        _i2c_sys.sda_pullup_en = GPIO_PULLUP_ENABLE;
        _i2c_sys.clk_stretch_tick = 300;
        _i2c_sys.scl_io_num = _eeprom.scl;
        _i2c_sys.sda_io_num = _eeprom.sda;

        ESP_ERROR_CHECK(i2c_driver_install(_eeprom.i2c_port, _i2c_sys.mode));
        ESP_ERROR_CHECK(i2c_param_config(_eeprom.i2c_port, &_i2c_sys));

        esp_err_t ret = _eeprom_access_test();

        if(ret == ESP_OK) {
            ESP_LOGI(TAG_INKY_EEPROM, "I2C Set-up complete!");
        }

        return ret;
    }

    /**
     * 
    */
    inky_eeprom::inky_eeprom(inky_eeprom_sys_t eeprom)
    : _eeprom(eeprom)
    {
        _i2c_init();
    }

}