/*
 * Copyright (C) 2018  SuperGreenLab <towelie@supergreenlab.com>
 * Author: Constantin Clauzel <constantin.clauzel@gmail.com>
 *
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

#include "utils.h"

#include "../misc/log.h"

void print_time(time_t t) {
  struct tm timeinfo;
  localtime_r(&t, &timeinfo); 

  print_timeinfo(timeinfo);
}

void print_timeinfo(struct tm timeinfo) {
  char buffer[26] = {0};
  strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", &timeinfo);
  ESP_LOGI(TAG, "%s", buffer);
}
