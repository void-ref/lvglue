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

#include "lvglue.h"

#include <Arduino.h>
#include <freertos/FreeRTOS.h>

#include <atomic>

namespace {
    TaskHandle_t            task_handle       = nullptr;
    std::atomic<TickType_t> task_period       = pdMS_TO_TICKS(5);
    std::atomic<bool>       task_control_flag = false;
} // namespace

void     lvglue_log(lv_log_level_t level, const char* buf);
void     lvglue_rtos_task(void*);
uint32_t lvglue_timebase_tick();

void uni::lvglue_init() {
    lv_init();
    lv_tick_set_cb(lvglue_timebase_tick);
    lv_log_register_print_cb(lvglue_log);
}

void uni::lvglue_start() {
    if (lvglue_is_running())
        return;
    xTaskCreate(
        lvglue_rtos_task,
        "lvgl-glue",
        LVGLUE_STACK_DEPTH,
        nullptr,
        LVGLUE_PRIORITY,
        &task_handle
    );
}

void uni::lvglue_stop() {
    task_control_flag = false;
}

bool uni::lvglue_is_running() {
    //? could query freertos task info for more confidence,
    //? but there's a minor performance hit for fetching task state
    return task_control_flag;
}

void uni::lvglue_set_task_handler_interval(uint32_t ms) {
    TickType_t ticks = pdMS_TO_TICKS(ms);
    task_period      = (ticks == 0) ? 1 : ticks;
}

uint32_t lvglue_timebase_tick() {
    return millis();
}

void lvglue_rtos_task(void* ctx) {
    TickType_t prev_wake_time = xTaskGetTickCount();
    while (task_control_flag) {
        lv_task_handler();
        xTaskDelayUntil(&prev_wake_time, task_period.load());
    }
}

#if LV_USE_LOG != 0
void lvglue_log(lv_log_level_t level, const char* buf) {
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif
