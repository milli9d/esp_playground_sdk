/**
 * SSD1683 Controller HAL
 * 
 * This is the SPI controller on InkyWhat display
 * 
 * @author Milind Singh
*/

namespace ESPP
{
#define SSD1683_DRIVER_CONTROL      0x01u
#define SSD1683_GATE_VOLTAGE        0x03u
#define SSD1683_SOURCE_VOLTAGE      0x04u
#define SSD1683_DISPLAY_CONTROL     0x07u
#define SSD1683_NON_OVERLAP         0x0Bu
#define SSD1683_BOOSTER_SOFT_START  0x0Cu
#define SSD1683_GATE_SCAN_START     0x0Fu
#define SSD1683_DEEP_SLEEP          0x10u
#define SSD1683_DATA_MODE           0x11u
#define SSD1683_SW_RESET            0x12u
#define SSD1683_TEMP_WRITE          0x1Au
#define SSD1683_TEMP_READ           0x1Bu
#define SSD1683_TEMP_CONTROL        0x18u
#define SSD1683_TEMP_LOAD           0x1Au
#define SSD1683_MASTER_ACTIVATE     0x20u
#define SSD1683_DISP_CTRL1          0x21u
#define SSD1683_DISP_CTRL2          0x22u
#define SSD1683_WRITE_RAM           0x24u
#define SSD1683_WRITE_ALTRAM        0x26u
#define SSD1683_READ_RAM            0x25u
#define SSD1683_VCOM_SENSE          0x2Bu
#define SSD1683_VCOM_DURATION       0x2Cu
#define SSD1683_WRITE_VCOM          0x2Cu
#define SSD1683_READ_OTP            0x2Du
#define SSD1683_WRITE_LUT           0x32u
#define SSD1683_WRITE_DUMMY         0x3Au
#define SSD1683_WRITE_GATELINE      0x3Bu
#define SSD1683_WRITE_BORDER        0x3Cu
#define SSD1683_SET_RAMXPOS         0x44u
#define SSD1683_SET_RAMYPOS         0x45u
#define SSD1683_SET_RAMXCOUNT       0x4Eu
#define SSD1683_SET_RAMYCOUNT       0x4Fu
#define SSD1683_NOP                 0xFFu
}