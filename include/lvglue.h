/*
 * Copyright 2025 Mitchell Matsumori-Kelly
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#pragma once

#include <freertos/FreeRTOS.h>
#include <lvgl.h>

#define LVGLUE_STACK_DEPTH 8192
#define LVGLUE_PRIORITY    3

namespace uni {

    struct LvglueTaskHandlerConfig {
        uint8_t  priority;
        uint32_t stack_depth;
        uint16_t period_ms;
    };

    void lvglue_init();
    void lvglue_start();
    void lvglue_stop();
    bool lvglue_is_running();
    void lvglue_set_task_handler_interval(uint32_t ms);

} // namespace uni
