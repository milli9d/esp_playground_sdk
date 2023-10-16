/**
 * @file espp_shell.cpp
 *
 * @brief ESP Shell
 *
 */

#include <cstdio>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <driver/uart.h>
#include <esp_console.h>
#include <esp_system.h>
#include <esp_vfs_dev.h>

#include "linenoise/linenoise.h"

#include <esp_log.h>
#define ESPP_WIFI_BASE_TAG "ESPP Shell"

#include "espp_shell.hpp"

namespace espp {

/* ========================================================================= */
/* Extra Commands */
/* ========================================================================= */

/**
 * @brief Command to reboot system [Soft]
 * @param argc
 * @param argv
 */
int espp_shell::_system_cmd_reboot(int argc, char** argv)
{
    /* reboot system [soft] */
    esp_restart();
    return 0;
}

/**
 * @brief Command to clear shell
 * @param argc
 * @param argv
 */
int espp_shell::_system_cmd_clear(int argc, char** argv)
{
    /* clear shell screen */
    linenoiseClearScreen();
    return 0;
}

/**
 * @brief Add system commands [Shell]
 * @param
 */
void espp_shell::_init_system_commands()
{
    /* register reboot command */
    esp_console_cmd_t rbt_cmd{};
    rbt_cmd.command = "reboot";
    rbt_cmd.help = "Reboot the system [Soft]";
    rbt_cmd.func = &espp_shell::_system_cmd_reboot;

    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_console_cmd_register(&rbt_cmd));

    /* register clear command */
    esp_console_cmd_t clr_cmd{};
    clr_cmd.command = "clear";
    clr_cmd.help = "Clear the shell";
    clr_cmd.func = &espp_shell::_system_cmd_clear;

    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_console_cmd_register(&clr_cmd));
}

/* ========================================================================= */
/* Private API */
/* ========================================================================= */

#ifdef CONFIG_IDF_TARGET_ESP8266
/**
 * @brief Initialize UART [ESP8266]
 * @param
 */
void espp_shell::_init_uart(void)
{
    /* disable buffering on stdin */
    setvbuf(stdin, NULL, _IONBF, 0);

    /* line ending settings (useful for navigation) */
    esp_vfs_dev_uart_set_rx_line_endings(ESP_LINE_ENDINGS_CR);
    esp_vfs_dev_uart_set_tx_line_endings(ESP_LINE_ENDINGS_CRLF);

    /* Configure UART. Note that REF_TICK is used so that the baud rate remains
     * correct while APB frequency is changing in light sleep mode.
     */
    uart_config_t uart_config{};
    uart_config.baud_rate = CONFIG_ESP_CONSOLE_UART_BAUDRATE;
    uart_config.data_bits = UART_DATA_8_BITS;
    uart_config.parity = UART_PARITY_DISABLE;
    uart_config.stop_bits = UART_STOP_BITS_1;

    ESP_ERROR_CHECK_WITHOUT_ABORT(
        uart_param_config(static_cast<uart_port_t>(CONFIG_ESP_CONSOLE_UART_NUM), &uart_config));

    /* Install UART driver for interrupt-driven reads and writes */
    ESP_ERROR_CHECK_WITHOUT_ABORT(
        uart_driver_install(static_cast<uart_port_t>(CONFIG_ESP_CONSOLE_UART_NUM), 256, 0, 0, NULL, 0));

    /* Tell VFS to use UART driver */
    esp_vfs_dev_uart_use_driver(static_cast<uart_port_t>(CONFIG_ESP_CONSOLE_UART_NUM));
}
#else
/**
 * @brief Initialize UART
 * @param
 */
void espp_shell::_init_uart(void)
{
    /* disable buffering on stdin */
    setvbuf(stdin, NULL, _IONBF, 0);

    /* line ending settings (useful for navigation) */
    esp_vfs_dev_uart_port_set_rx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CR);
    esp_vfs_dev_uart_port_set_tx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CRLF);

    /* Configure UART. Note that REF_TICK is used so that the baud rate remains
     * correct while APB frequency is changing in light sleep mode.
     */
    uart_config_t uart_config = {
        .baud_rate = CONFIG_ESP_CONSOLE_UART_BAUDRATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
    };
    ESP_ERROR_CHECK_WITHOUT_ABORT(
        uart_param_config(static_cast<uart_port_t>(CONFIG_ESP_CONSOLE_UART_NUM), &uart_config));

    /* Install UART driver for interrupt-driven reads and writes */
    ESP_ERROR_CHECK_WITHOUT_ABORT(
        uart_driver_install(static_cast<uart_port_t>(CONFIG_ESP_CONSOLE_UART_NUM), 256, 0, 0, NULL, 0));

    /* Tell VFS to use UART driver */
    esp_vfs_dev_uart_use_driver(static_cast<uart_port_t>(CONFIG_ESP_CONSOLE_UART_NUM));
}
#endif

/**
 * @brief Initialize console
 * @param
 */
void espp_shell::_init_console(void)
{
    /* Initialize the console */
    esp_console_config_t console_config = {
        .max_cmdline_length = 256,
        .max_cmdline_args = 8,
        .hint_color = atoi(LOG_COLOR_CYAN),
        .hint_bold = true,
    };
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_console_init(&console_config));
}

/**
 * @brief Initialize linenoise
 * @param
 */
void espp_shell::_init_linenoise(void)
{
    /* Configure linenoise line completion library */
    /* Enable multiline editing. If not set, long commands will scroll within
     * single line.
     */
    linenoiseSetMultiLine(1);

    /* Tell linenoise where to get command completions and hints */
    linenoiseSetCompletionCallback(&esp_console_get_completion);
    linenoiseSetHintsCallback((linenoiseHintsCallback*)&esp_console_get_hint);

    /* Set command history size */
    linenoiseHistorySetMaxLen(100);
}

/* ========================================================================= */
/* Public API */
/* ========================================================================= */

/**
 * @brief ESPP Shell thread
 * @param args
 */
void espp_shell::_thread_run(void* args)
{
    /* fetch base class pointer */
    espp_shell* ptr = static_cast<espp_shell*>(args);

    /* register default help command */
    esp_console_register_help_command();

    /* Prompt to be printed before each line.
     * This can be customized, made dynamic, etc.
     */
    const char* prompt = LOG_COLOR_E "ESP Playground SDK $> " LOG_RESET_COLOR;

    printf("========================================================== \n"
           "Welcome to ESP Playground.\n"
           "Type 'help' to get the list of commands.\n"
           "Use UP/DOWN arrows to navigate through command history.\n"
           "Press TAB when typing command name to auto-complete.\n"
           "========================================================== \n");

    /* Figure out if the terminal supports escape sequences */
    int probe_status = linenoiseProbe();
    if (probe_status) { /* zero indicates success */
        printf("\n"
               "Your terminal application does not support escape sequences.\n"
               "Line editing and history features are disabled.\n"
               "On Windows, try using Putty instead.\n");
        linenoiseSetDumbMode(1);
        /* Since the terminal doesn't support escape sequences,
         * don't use color codes in the prompt.
         */
        prompt = "ESP Playground SDK $> ";
    }

    /* init system commands */
    ptr->_init_system_commands();

    /* Main loop */
    while (true) {
        /* Get a line using linenoise.
         * The line is returned when ENTER is pressed.
         */
        char* line = linenoise(prompt);
        if (line == NULL) { /* Ignore empty lines */
            continue;
        }

        /* Add the command to the history */
        linenoiseHistoryAdd(line);

        /* Try to run the command */
        int ret;
        esp_err_t err = esp_console_run(line, &ret);
        if (err == ESP_ERR_NOT_FOUND) {
            printf("Unsupported command\n");
        } else if (err == ESP_ERR_INVALID_ARG) {
            // command was empty
        } else if (err == ESP_OK && ret != ESP_OK) {
            printf("Command returned non-zero error code: 0x%x (%s)\n", ret, esp_err_to_name(err));
        } else if (err != ESP_OK) {
            printf("Internal error: %s\n", esp_err_to_name(err));
        }

        /* linenoise allocates line buffer on the heap, so need to free it */
        linenoiseFree(line);

        /* shell latency (yield for other tasks )*/
        vTaskDelay(100u / portTICK_PERIOD_MS);
    }
}

/* ========================================================================= */
/* Constructors/Destructors */
/* ========================================================================= */

/**
 * @brief Default constructor
 */
espp_shell::espp_shell()
{
    /* init UART settings */
    _init_uart();

    /* init console */
    _init_console();

    /* init linenoise */
    _init_linenoise();

    /* init shell thread */
    xTaskCreate((TaskFunction_t)&_thread_run, "ESPP Shell", 4096u, this, 1u, &_task_handle);

    /* fin */
    ESP_LOGI(ESPP_WIFI_BASE_TAG, "ESPP Shell started!");
}

/**
 * @brief Default constructor
 */
espp_shell::~espp_shell()
{
    /* do nothing */
    ESP_LOGI(ESPP_WIFI_BASE_TAG, "ESPP Shell stopped!");
}

} // namespace espp