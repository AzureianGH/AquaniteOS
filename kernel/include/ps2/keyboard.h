#pragma once
// PS/2 Keyboard Driver
#include <io/io.h>
#include <idt/regs.h>
#include <shared.h>
typedef struct {
    uint8_t scancode;
    bool released;
} key_t;
//functions
void KeyboardHandler(registers_t *r);
void KeyboardInit();
void DisableKeyboard();
void EnableKeyboard();
void AddKeyboardInterrupt(void (*func)(key_t));
void RemoveKeyboardInterrupt(void (*func)(key_t));