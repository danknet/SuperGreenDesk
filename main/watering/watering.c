/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include "watering.h"

#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "../core/kv/kv.h"
#include "../core/log/log.h"

#define GPIO_OUTPUT_WATERING    18
#define GPIO_OUTPUT_WATERING_SEL  (1ULL<<GPIO_OUTPUT_WATERING)

static void watering_task(void *param);

void init_watering() {
  ESP_LOGI(SGO_LOG_EVENT, "@WATERING Initializing watering module\n");

	gpio_config_t io_conf;
	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pin_bit_mask = GPIO_OUTPUT_WATERING_SEL;
	io_conf.pull_down_en = 1;
	io_conf.pull_up_en = 0;
	gpio_config(&io_conf);

  xTaskCreate(watering_task, "WATERING", 4096, NULL, 10, NULL);
}

static void watering_task(void *param) {
  while (true) {
    const int last = get_lastwatering();
    const int period = get_wateringperiod();
    const int duration = get_wateringduration();

    time_t now;
    struct tm tm_now;
    time(&now);
    localtime_r(&now, &tm_now); 

    ESP_LOGI(SGO_LOG_EVENT, "@WATERING %ld %d\n", now, last);

    if (now - last > period) {
      ESP_LOGI(SGO_LOG_EVENT, "@WATERING start\n");
      set_lastwatering(now);
      set_iswatering(1);
      gpio_set_level(GPIO_OUTPUT_WATERING, true);
      vTaskDelay(duration * 1000 / portTICK_PERIOD_MS);
      gpio_set_level(GPIO_OUTPUT_WATERING, false);
      set_iswatering(0);
      ESP_LOGI(SGO_LOG_EVENT, "@WATERING stop\n");
    }
    vTaskDelay(5 * 1000 / portTICK_PERIOD_MS);
  }
}
