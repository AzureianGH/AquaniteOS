#include <ps2/keyboard.h>
#include <flanterm/flantermglobal.h>

Vector<void (*)(key_t)> key_handlers;

void ps2_keyboard_add_handler(void (*handler)(key_t))
{
    key_handlers.PushBack(handler);
}

void ps2_keyboard_remove_handler(void (*handler)(key_t))
{
    for (size_t i = 0; i < key_handlers.Length(); i++)
    {
        if (key_handlers[i] == handler)
        {
            key_handlers.Erase(i);
            return;
        }
    }
}

void ps2_keyboard_handler(registers_t *r)
{
    uint8_t scancode = inb(PS2_DATA_PORT);
    key_t key;
    key.scancode = scancode;
    key.released = scancode & 0x80;
    for (size_t i = 0; i < key_handlers.Length(); i++)
    {
        key_handlers[i](key);
    }
}
void flush_ps2_buffer() {
    while (inb(PS2_STATUS_PORT) & 0x01) {
        inb(PS2_DATA_PORT);
    }
}

void ps2_keyboard_enable()
{
    outb(PS2_COMMAND_PORT, PS2_COMMAND_ENABLE_FIRST_PORT);
    io_wait();
    outb(PS2_DATA_PORT, 0xF4);
}

void ps2_keyboard_disable()
{
    outb(PS2_COMMAND_PORT, PS2_COMMAND_DISABLE_FIRST_PORT);
    io_wait();
    outb(PS2_DATA_PORT, 0xF5);
}

void ps2_keyboard_reset()
{
    outb(PS2_COMMAND_PORT, PS2_COMMAND_RESET);
    io_wait();
    outb(PS2_DATA_PORT, 0xFF);
}

void init_ps2_keyboard() {
    flush_ps2_buffer(); // Ensure no leftover data in the PS/2 buffer
    outb(PS2_COMMAND_PORT, PS2_COMMAND_TEST_KEYBOARD);
    io_wait();
    uint8_t result = inb(PS2_DATA_PORT);
    if (result != PS2_COMMAND_TEST_FAIL) {
        lprintf(logging_level::OK, "PS/2 Keyboard initialized.\n");
    } else {
        lprintf(logging_level::ERROR, "PS/2 Keyboard failed to initialize.\n");
    }
}
