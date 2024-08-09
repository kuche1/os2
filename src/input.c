
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

err_t in$scancode_to_ascii(unsigned char scancode, char * out_ascii){
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
            * out_ascii = (char) (scancode - 2 + '1');
            break;
        case 11:
            * out_ascii = '0';
            break;
        case 12:
            * out_ascii = '-';
            break;
        case 13:
            * out_ascii = '=';
            break;
        case 14: // backspace
            * out_ascii = '\b';
            break;
        // case 15: // tab
        case 16:
            * out_ascii = 'q';
            break;
        case 17:
            * out_ascii = 'w';
            break;
        case 18:
            * out_ascii = 'e';
            break;
        case 19:
            * out_ascii = 'r';
            break;
        case 20:
            * out_ascii = 't';
            break;
        case 21:
            * out_ascii = 'y';
            break;
        case 22:
            * out_ascii = 'u';
            break;
        case 23:
            * out_ascii = 'i';
            break;
        case 24:
            * out_ascii = 'o';
            break;
        case 25:
            * out_ascii = 'p';
            break;
        case 26:
            * out_ascii = '[';
            break;
        case 27:
            * out_ascii = ']';
            break;
        case 28:
            * out_ascii = '\n';
            break;
        // case 29: // control
        case 30:
            * out_ascii = 'a';
            break;
        case 31:
            * out_ascii = 's';
            break;
        case 32:
            * out_ascii = 'd';
            break;
        case 33:
            * out_ascii = 'f';
            break;
        case 34:
            * out_ascii = 'g';
            break;
        case 35:
            * out_ascii = 'h';
            break;
        case 36:
            * out_ascii = 'j';
            break;
        case 37:
            * out_ascii = 'k';
            break;
        case 38:
            * out_ascii = 'l';
            break;
        case 39:
            * out_ascii = ';';
            break;
        case 40:
            * out_ascii = '\'';
            break;
        case 41:
            * out_ascii = '`';
            break;
        // case 42: // left shift
        case 43:
            * out_ascii = '\\';
            break;
        case 44:
            * out_ascii = 'z';
            break;
        case 45:
            * out_ascii = 'x';
            break;
        case 46:
            * out_ascii = 'c';
            break;
        case 47:
            * out_ascii = 'v';
            break;
        case 48:
            * out_ascii = 'b';
            break;
        case 49:
            * out_ascii = 'n';
            break;
        case 50:
            * out_ascii = 'm';
            break;
        case 51:
            * out_ascii = ',';
            break;
        case 52:
            * out_ascii = '.';
            break;
        case 53:
            * out_ascii = '/';
            break;
        // case 54: // right shift
        case 55:
            * out_ascii = '*';
            break;
        // case 56: // alt
        case 57:
            * out_ascii = ' ';
            break;
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
            * out_ascii = '-';
            break;
        // 75 left arrow
        // 76 no dea
        // 77 right arrow
        case 78:
            * out_ascii = '+';
            break;
        // 79 end
        // 80 down arrow
        // 81 page down
        // 82 insert
        // 83 delete
        // 84 85 86 no idea
        // 87 f11
        // 88 f12

        default:
            return err$ERR;
    }

    return err$OK;
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

            char ascii;
            err_t err = in$scancode_to_ascii(keycode, &ascii);

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
                arr$char_t$del_last(line);
                out$clear_last_char();
            }
            continue;
        }

        err_t err = arr$char_t$push(line, ch);

        if(err){
            // TODO I would like to make this more interactive,
            // explaining to the user that there is not eough space
            // in the buffer, and that he needs to edit the line
            continue;
        }

        out$ch(ch);

    }

}
