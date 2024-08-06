
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

// char in$scancode_to_ascii[] = {
//     0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', /* Backspace */
//     '\t', /* Tab */
//     'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
//     0, /* Control */
//     'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
//     0, /* Left shift */
//     '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, /* Right shift */
//     '*',
//     0,  /* Alt */
//     ' ', /* Space bar */
//     0,  /* Caps lock */
//     0,  /* F1 key ... > */
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* < ... F10 */
//     0,  /* Num lock */
//     0,  /* Scroll Lock */
//     0,  /* Home key */
//     0,  /* Up Arrow */
//     0,  /* Page Up */
//     '-',
//     0,  /* Left Arrow */
//     0,
//     0,  /* Right Arrow */
//     '+',
//     0,  /* End key */
//     0,  /* Down Arrow */
//     0,  /* Page Down */
//     0,  /* Insert Key */
//     0,  /* Delete Key */
//     0, 0, 0,
//     0,  /* F11 Key */
//     0,  /* F12 Key */
//     0,  /* All other keys are undefined */
// };

err_or_char_t in$scancode_to_ascii(unsigned char scancode){
    switch(scancode){
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            return (err_or_char_t) {.err = false, .data = (char) (scancode - 2 + '1')};
        case 11:
            return (err_or_char_t) {.err = false, .data = '0'};
        case 12:
            return (err_or_char_t) {.err = false, .data = '-'};
        case 13:
            return (err_or_char_t) {.err = false, .data = '='};
        case 14: // backspace
            return (err_or_char_t) {.err = false, .data = '\b'};
        // case 15: // tab
        case 16:
            return (err_or_char_t) {.err = false, .data = 'q'};
        case 17:
            return (err_or_char_t) {.err = false, .data = 'w'};
        case 18:
            return (err_or_char_t) {.err = false, .data = 'e'};
        case 19:
            return (err_or_char_t) {.err = false, .data = 'r'};
        case 20:
            return (err_or_char_t) {.err = false, .data = 't'};
        case 21:
            return (err_or_char_t) {.err = false, .data = 'y'};
        case 22:
            return (err_or_char_t) {.err = false, .data = 'u'};
        case 23:
            return (err_or_char_t) {.err = false, .data = 'i'};
        case 24:
            return (err_or_char_t) {.err = false, .data = 'o'};
        case 25:
            return (err_or_char_t) {.err = false, .data = 'p'};
        case 26:
            return (err_or_char_t) {.err = false, .data = '['};
        case 27:
            return (err_or_char_t) {.err = false, .data = ']'};
        case 28:
            return (err_or_char_t) {.err = false, .data = '\n'};
        // case 29: // control
        case 30:
            return (err_or_char_t) {.err = false, .data = 'a'};
        case 31:
            return (err_or_char_t) {.err = false, .data = 's'};
        case 32:
            return (err_or_char_t) {.err = false, .data = 'd'};
        case 33:
            return (err_or_char_t) {.err = false, .data = 'f'};
        case 34:
            return (err_or_char_t) {.err = false, .data = 'g'};
        case 35:
            return (err_or_char_t) {.err = false, .data = 'h'};
        case 36:
            return (err_or_char_t) {.err = false, .data = 'j'};
        case 37:
            return (err_or_char_t) {.err = false, .data = 'k'};
        case 38:
            return (err_or_char_t) {.err = false, .data = 'l'};
        case 39:
            return (err_or_char_t) {.err = false, .data = ';'};
        case 40:
            return (err_or_char_t) {.err = false, .data = '\''};
        case 41:
            return (err_or_char_t) {.err = false, .data = '`'};
        // case 42: // left shift
        case 43:
            return (err_or_char_t) {.err = false, .data = '\\'};
        case 44:
            return (err_or_char_t) {.err = false, .data = 'z'};
        case 45:
            return (err_or_char_t) {.err = false, .data = 'x'};
        case 46:
            return (err_or_char_t) {.err = false, .data = 'c'};
        case 47:
            return (err_or_char_t) {.err = false, .data = 'v'};
        case 48:
            return (err_or_char_t) {.err = false, .data = 'b'};
        case 49:
            return (err_or_char_t) {.err = false, .data = 'n'};
        case 50:
            return (err_or_char_t) {.err = false, .data = 'm'};
        case 51:
            return (err_or_char_t) {.err = false, .data = ','};
        case 52:
            return (err_or_char_t) {.err = false, .data = '.'};
        case 53:
            return (err_or_char_t) {.err = false, .data = '/'};
        // case 54: // right shift
        case 55:
            return (err_or_char_t) {.err = false, .data = '*'};
        // case 56: // alt
        case 57:
            return (err_or_char_t) {.err = false, .data = ' '};
        // case 58: // caps lock
        // 59 f1
        // ...
        // 68 f10
        // 69 num lock
        // 70 scroll lock
        // 71 home
        // 72 up arrow
        // 73 page up
        case 74:
            return (err_or_char_t) {.err = false, .data = '-'};
        // 75 left arrow
        // 76 no dea
        // 77 right arrow
        case 78:
            return (err_or_char_t) {.err = false, .data = '+'};
        // 79 end
        // 80 down arrow
        // 81 page down
        // 82 insert
        // 83 delete
        // 84 85 86 no idea
        // 87 f11
        // 88 f12
    }
    return (err_or_char_t) {.err = true, .data = 0};
}

char in$ch(void){
    uint8_t status;
    unsigned char keycode;

    // Wait for a key press
    while(true){
        status = in$inb(in$KEYBOARD_STATUS_PORT);
        if(status & in$KEYBOARD_STATUS_MASK_OUT_BUF){
            keycode = in$inb(in$KEYBOARD_DATA_PORT);

            // if(keycode < sizeof(in$scancode_to_ascii)){
            //     return in$scancode_to_ascii[keycode];
            // }

            UNP(err, char, ascii, in$scancode_to_ascii(keycode));

            if(err){
                continue;
            }

            return ascii;

        }
    }
}

void in$line(arr_char_t * line){

    while(true){

        char ch = in$ch();

        if(ch == '\n'){
            out$nl();
            break;
        }

        if(ch == '\b'){
            if(line->len > 0){
                arr$char$del_last(line);
                out$clear_last_char();
            }
            continue;
        }

        err_t err = arr$char$push(line, ch);

        if(err){
            // TODO I would like to make this more interactive,
            // explaining to the user that there is not eough space
            // in the buffer, and that he needs to edit the line
            continue;
        }

        out$ch(ch);

    }

}
