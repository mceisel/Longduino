/*
  Copyright (c) 2020 SCP.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once

// Standard C functions required in Arduino API
// If these functions are not provided by the standard library, the
// core should supply an implementation of them.

#include <inttypes.h>

#include "pins_arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

void usart_com_init(uint32_t com, uint32_t wlen, unsigned long baud);
int usart_readable(uint32_t com);
int usart_writable(uint32_t com);
int usart_put_char(uint32_t com, uint32_t wlen, uint8_t ch);
int usart_put_buf(uint32_t com, uint32_t wlen, const uint8_t *buffer, size_t size);
int usart_get_char(uint32_t com, uint32_t wlen);

#ifdef __cplusplus
} // extern "C"
#endif
