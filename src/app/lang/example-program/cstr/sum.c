
// asks for a 1-len number
// asks for another 1-len number
// prints the num
// note that the result will look weird if it's >=10

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

    // *0x11 = input("b:")
    lang$ic$out$arg,
    'b',
    lang$ic$out$arg,
    ':',
    lang$ic$out$arg,
    '\n',
    lang$ic$in$cell,
    0x11,

    // *0x00 = *0x10
    lang$ic$copy$cell$0x00,
    0x10,
    // *0x00 -= '0'
    lang$ic$sub$0x00$arg,
    '0',
    // *0x10 = *0x00
    lang$ic$copy$0x00$cell,
    0x10,

    // *0x00 = *0x11
    lang$ic$copy$cell$0x00,
    0x11,
    // *0x00 -= '0'
    lang$ic$sub$0x00$arg,
    '0',
    // *0x11 = *0x00
    lang$ic$copy$0x00$cell,
    0x11,

    // *00 = *0x10
    lang$ic$copy$cell$0x00,
    0x10,

    // *00 += *0x11
    lang$ic$add$0x00$cell,
    0x11,

    // *00 += '0'
    lang$ic$add$0x00$arg,
    '0',

    // print(*0x00)
    lang$ic$out$cell,
    0x00,

}
