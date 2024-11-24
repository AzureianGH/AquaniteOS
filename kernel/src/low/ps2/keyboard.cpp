#include <io/io.h>
#include <idt/regs.h>
#include <shared.h>
#include <ps2/keyboard.h>
#include <flanterm/flantermglobal.h>

bool AllowKeyboard = true;

Vector<void (*)(key_t)> InterruptsToCall;

void AddKeyboardInterrupt(void (*func)(key_t))
{
    InterruptsToCall.PushBack(func);
}
void RemoveKeyboardInterrupt(void (*func)(key_t))
{
    for (int i = 0; i < InterruptsToCall.Length(); i++)
    {
        if (InterruptsToCall[i] == func)
        {
            InterruptsToCall.Erase(i);
            return;
        }
    }
}
void DisableKeyboard()
{
    AllowKeyboard = false;
}
void EnableKeyboard()
{
    AllowKeyboard = true;
}
inline uint8_t getScancode()
{
    return inb(0x60);
}
void KeyboardHandler(registers_t *r)
{
    if (!AllowKeyboard)
    {
        return;
    }
    uint8_t scancode = getScancode();
    //run handlers
    key_t data;
    data.scancode = scancode;
    //check if key is pressed
    if (scancode & 0x80)
    {
        data.released = true;
    }
    else
    {
        data.released = false;
    }
    for (int i = 0; i < InterruptsToCall.Length(); i++)
    {
        InterruptsToCall[i](data);
    }
    return;
}

void KeyboardInit()
{
    InterruptsToCall = Vector<void(*)(key_t)>();
}
