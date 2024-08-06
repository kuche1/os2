
// I think this will work, but only in realtime mode
// https://wiki.osdev.org/BIOS 0=blocking 1=nonoblocking
// static inline char getch (void){

//    uint16_t inchar;

//    __asm__ __volatile__(
// 		"int $0x16\n\t"
// 		: "=a"(inchar)
// 		: "0"(0x0)
// 	);

//    return ((char)inchar);
// }

#define in$KEYBOARD_DATA_PORT 0x60
#define in$KEYBOARD_STATUS_PORT 0x64
#define in$KEYBOARD_STATUS_MASK_OUT_BUF 0x01

inline uint8_t in$inb(uint16_t port){
    uint8_t ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

inline void in$outb(uint16_t port, uint8_t val){
    __asm__ __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}

char in$scancode_to_ascii[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', /* Backspace */
    '\t', /* Tab */
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
    0, /* Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, /* Right shift */
    '*',
    0,  /* Alt */
    ' ', /* Space bar */
    0,  /* Caps lock */
    0,  /* F1 key ... > */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* < ... F10 */
    0,  /* Num lock */
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
    '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
    '+',
    0,  /* End key */
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0, 0, 0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

char in$ch(void){
    uint8_t status;
    unsigned char keycode;

    // Wait for a key press
    while(true){
        status = in$inb(in$KEYBOARD_STATUS_PORT);
        if(status & in$KEYBOARD_STATUS_MASK_OUT_BUF){
            keycode = in$inb(in$KEYBOARD_DATA_PORT);
            if(keycode < sizeof(in$scancode_to_ascii)){
                return in$scancode_to_ascii[keycode];
            }
        }
    }
}
