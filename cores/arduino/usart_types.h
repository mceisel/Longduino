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

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(SERIAL_BUFFER_SIZE)
#if defined(SERIAL_RX_BUFFER_SIZE)
#define SERIAL_BUFFER_SIZE SERIAL_RX_BUFFER_SIZE
#endif
#endif

#if !defined(SERIAL_BUFFER_SIZE)
#if ((RAMEND - RAMSTART) < 1023)
#define SERIAL_BUFFER_SIZE 16
#else
#define SERIAL_BUFFER_SIZE 64
#endif
#endif
#if  (SERIAL_BUFFER_SIZE>256)
typedef uint16_t usart_buffer_index_t;
#else
typedef uint8_t usart_buffer_index_t;
#endif

typedef struct {
    uint32_t _com;
    uint32_t wlen;
    usart_buffer_index_t head;
    usart_buffer_index_t tail;

    unsigned char data[SERIAL_BUFFER_SIZE];
} usart_buffer_t;

#ifdef __cplusplus
} // extern "C"
#endif
