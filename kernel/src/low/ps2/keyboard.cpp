#include <io/io.h>
#include <idt/regs.h>
#include <shared.h>
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

void flush_ps2_buffer()
{
    while (inb(PS2_STATUS_PORT) & PS2_STATUS_OUTPUT_FULL)
    {
        inb(PS2_DATA_PORT);
    }
}

void ps2_keyboard_enable()
{
    outb(PS2_COMMAND_PORT, PS2_COMMAND_ENABLE_FIRST_PORT);
}

void ps2_keyboard_disable()
{
    outb(PS2_COMMAND_PORT, PS2_COMMAND_DISABLE_FIRST_PORT);
}

void ps2_keyboard_handler(registers_t *r)
{
    if (!(inb(PS2_STATUS_PORT) & PS2_STATUS_OUTPUT_FULL))
        return;

    uint8_t scancode = inb(PS2_DATA_PORT);
    key_t key;
    key.scancode = scancode;
    key.released = scancode & 0x80;

    for (size_t i = 0; i < key_handlers.Length(); i++)
    {
        key_handlers[i](key);
    }
}
void wait_for_input_buffer() {
    while (inb(PS2_STATUS_PORT) & PS2_STATUS_INPUT_FULL);
}

void ps2_keyboard_reset()
{
    flush_ps2_buffer();

    // Send reset command
    outb(PS2_DATA_PORT, PS2_COMMAND_RESET);

    // Wait for response
    while (!(inb(PS2_STATUS_PORT) & PS2_STATUS_OUTPUT_FULL));
    uint8_t response = inb(PS2_DATA_PORT);

    if (response != PS2_ACK)
    {
        // Log or retry if reset fails
        printf("PS/2 Keyboard: Reset failed! Response: 0x%x\n", response);
        return;
    }

    // Wait for self-test completion
    while (!(inb(PS2_STATUS_PORT) & PS2_STATUS_OUTPUT_FULL));
    response = inb(PS2_DATA_PORT);

    if (response != 0xAA) // 0xAA = Self-test passed
    {
        printf("PS/2 Keyboard: Self-test failed! Response: 0x%x\n", response);
        return;
    }
}

void init_ps2_keyboard()
{
    flush_ps2_buffer();

    // Disable both ports
    outb(PS2_COMMAND_PORT, PS2_COMMAND_DISABLE_FIRST_PORT);
    outb(PS2_COMMAND_PORT, PS2_COMMAND_DISABLE_SECOND_PORT);

    // Flush buffer again
    flush_ps2_buffer();

    // Read and modify configuration byte
    outb(PS2_COMMAND_PORT, PS2_COMMAND_READ_CONFIG);
    wait_for_input_buffer(); // Ensure controller is ready
    uint8_t config_byte = inb(PS2_DATA_PORT);

    config_byte |= 0x01; // Enable IRQ1
    config_byte &= ~0x20; // Disable second port
    outb(PS2_COMMAND_PORT, PS2_COMMAND_WRITE_CONFIG);
    outb(PS2_DATA_PORT, config_byte);

    // Enable first port
    outb(PS2_COMMAND_PORT, PS2_COMMAND_ENABLE_FIRST_PORT);

    // Reset keyboard
    ps2_keyboard_reset();

    // Set default settings
    flush_ps2_buffer();
    outb(PS2_DATA_PORT, PS2_COMMAND_SET_DEFAULTS);

    while (!(inb(PS2_STATUS_PORT) & PS2_STATUS_OUTPUT_FULL));
    uint8_t response = inb(PS2_DATA_PORT);

    if (response != PS2_ACK)
    {
        printf("PS/2 Keyboard: Failed to set default settings! Response: 0x%x\n", response);
        return;
    }

    // Enable scanning
    flush_ps2_buffer();
    outb(PS2_DATA_PORT, PS2_COMMAND_ENABLE_SCANNING);
    printf("PS/2 Keyboard initialized successfully.\n");
}