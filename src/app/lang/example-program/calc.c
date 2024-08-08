

// asks for a 1-len num
// asks for an operator (+ - * /)
// asks for another 1-len num
// prints the result (it's going to look weird if it's >= 10)

{

    // *0x10 = input("a:")
    lang$ic$out$arg,
    'a',
    lang$ic$out$arg,
    ':',
    lang$ic$out$arg,
    '\n',
    lang$ic$in$cell,
    0x10,

    // *0x11 = input("op:")
    lang$ic$out$arg,
    'o',
    lang$ic$out$arg,
    'p',
    lang$ic$out$arg,
    ':',
    lang$ic$out$arg,
    '\n',
    lang$ic$in$cell,
    0x11,

    // *0x12 = input("b:")
    lang$ic$out$arg,
    'b',
    lang$ic$out$arg,
    ':',
    lang$ic$out$arg,
    '\n',
    lang$ic$in$cell,
    0x12,

    // *0x00 = *0x10
    lang$ic$copy$cell$0x00,
    0x10,
    // *0x00 -= '0'
    lang$ic$sub$0x00$arg,
    '0',
    // *0x10 = *0x00
    lang$ic$copy$0x00$cell,
    0x10,

    // *0x00 = *0x12
    lang$ic$copy$cell$0x00,
    0x12,
    // *0x00 -= '0'
    lang$ic$sub$0x00$arg,
    '0',
    // *0x12 = *0x00
    lang$ic$copy$0x00$cell,
    0x12,

    // *0x00 = *0x11
    lang$ic$copy$cell$0x00,
    0x11,
    // *0x00 -= '+'
    lang$ic$sub$0x00$arg,
    '+',
    // if(0x00){skip}
    lang$ic$if$0x00$skipinst$arg,
    3,
        // *0x13 = *0x10 + *0x12
        lang$ic$copy$cell$0x00,
        0x10,
        lang$ic$add$0x00$cell,
        0x12,
        lang$ic$copy$0x00$cell,
        0x13,

    // *0x00 = *0x11
    lang$ic$copy$cell$0x00,
    0x11,
    // *0x00 -= '-'
    lang$ic$sub$0x00$arg,
    '-',
    // if(0x00){skip}
    lang$ic$if$0x00$skipinst$arg,
    3,
        // *0x13 = *0x10 - *0x12
        lang$ic$copy$cell$0x00,
        0x10,
        lang$ic$sub$0x00$cell,
        0x12,
        lang$ic$copy$0x00$cell,
        0x13,

    // *0x00 = *0x11
    lang$ic$copy$cell$0x00,
    0x11,
    // *0x00 -= '*'
    lang$ic$sub$0x00$arg,
    '*',
    // if(0x00){skip}
    lang$ic$if$0x00$skipinst$arg,
    3,
        // *0x13 = *0x10 * *0x12
        lang$ic$copy$cell$0x00,
        0x10,
        lang$ic$mul$0x00$cell,
        0x12,
        lang$ic$copy$0x00$cell,
        0x13,

    // *0x00 = *0x11
    lang$ic$copy$cell$0x00,
    0x11,
    // *0x00 -= '/'
    lang$ic$sub$0x00$arg,
    '/',
    // if(0x00){skip}
    lang$ic$if$0x00$skipinst$arg,
    3,
        // *0x13 = *0x10 / *0x12
        lang$ic$copy$cell$0x00,
        0x10,
        lang$ic$div$0x00$cell,
        0x12,
        lang$ic$copy$0x00$cell,
        0x13,

    // *00 = *0x13
    lang$ic$copy$cell$0x00,
    0x13,
    // *00 += '0'
    lang$ic$add$0x00$arg,
    '0',
    // print(*0x00)
    lang$ic$out$cell,
    0x00,

}
