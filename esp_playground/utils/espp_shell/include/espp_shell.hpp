/**
 * @file espp_shell.cpp
 *
 * @brief ESP Shell
 *
 */
#pragma once

#include <cstdio>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <driver/uart.h>
#include <esp_console.h>
#include <esp_vfs_dev.h>

#include "linenoise/linenoise.h"

namespace espp {
class espp_shell
{
  private:
    TaskHandle_t _task_handle{};

    /**
     * @brief Initialize console
     * @param
     */
    void _init_console(void);

    /**
     * @brief Initialize console
     * @param
     */
    void _init_uart(void);

    /**
     * @brief Initialize linenoise
     * @param
     */
    void _init_linenoise(void);

    /**
     * @brief Add system commands
     * @param
     */
    void _init_system_commands(void);

    /**
     * @brief ESPP Shell thread
     * @param args
     */
    static void _thread_run(void* args);

    /**
     * @brief Command to reboot system [Soft]
     * @param argc
     * @param argv
     */
    static int _system_cmd_reboot(int argc, char** argv);

    /**
     * @brief Command to clear shell
     * @param argc
     * @param argv
     */
    static int _system_cmd_clear(int argc, char** argv);

  public:
    /* ========================================================================= */
    /* Constructors/Destructors */
    /* ========================================================================= */

    /**
     * @brief Default constructor
     */
    espp_shell();

    /**
     * @brief Default destructor
     */
    ~espp_shell();

    /* ========================================================================= */
    /* Public API */
    /* ========================================================================= */
};

} // namespace espp