#pragma once
// PS/2 Keyboard Driver
#include <io/io.h>
#include <idt/regs.h>
#include <shared.h>

#define PS2_DATA_PORT 0x60
#define PS2_STATUS_PORT 0x64
#define PS2_COMMAND_PORT 0x64

#define PS2_STATUS_OUTPUT_FULL 0x01
#define PS2_STATUS_INPUT_FULL 0x02

#define PS2_ACK 0xFA
#define PS2_RESEND 0xFE
#define PS2_ERROR 0xFC

#define PS2_COMMAND_READ_CONFIG 0x20
#define PS2_COMMAND_WRITE_CONFIG 0x60
#define PS2_COMMAND_DISABLE_PORT1 0xAD
#define PS2_COMMAND_DISABLE_PORT2 0xA7
#define PS2_COMMAND_ENABLE_PORT1 0xAE
#define PS2_COMMAND_ENABLE_PORT2 0xA8
#define PS2_COMMAND_TEST_PORT1 0xAB
#define PS2_COMMAND_TEST_PORT2 0xA9
#define PS2_COMMAND_TEST_CONTROLLER 0xAA
#define PS2_COMMAND_SELF_TEST 0xAA
#define PS2_COMMAND_TEST_KEYBOARD 0xAB
#define PS2_COMMAND_TEST_MOUSE 0xA9
#define PS2_COMMAND_TEST_PASS 0x55
#define PS2_COMMAND_TEST_FAIL 0xFC
#define PS2_COMMAND_DISABLE_SCANNING 0xF5
#define PS2_COMMAND_ENABLE_SCANNING 0xF4
#define PS2_COMMAND_SET_DEFAULTS 0xF6
#define PS2_COMMAND_RESEND 0xFE
#define PS2_COMMAND_RESET 0xFF

#define PS2_COMMAND_DISABLE_FIRST_PORT 0xAD
#define PS2_COMMAND_DISABLE_SECOND_PORT 0xA7
#define PS2_COMMAND_ENABLE_FIRST_PORT 0xAE
#define PS2_COMMAND_ENABLE_SECOND_PORT 0xA8
typedef struct {
    uint8_t scancode;
    bool released;
} key_t;
//functions
void init_ps2_keyboard();
void ps2_keyboard_handler(registers_t *r);
void ps2_keyboard_enable();
void ps2_keyboard_disable();
void ps2_keyboard_reset();
void ps2_keyboard_add_handler(void (*handler)(key_t));
void ps2_keyboard_remove_handler(void (*handler)(key_t));