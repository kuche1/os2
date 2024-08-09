
// asks for a 1-len number
// asks for another 1-len number
// prints the num
// note that the result will look weird if it's >=10

// TODO unfinished

"out$arg 97\n"
"out$arg 58\n"
"out$arg 10\n"
"var a\n"
"in$cell a\n"

"out$arg 98\n"
"out$arg 58\n"
"out$arg 10\n"
"var b\n"
"in$cell b\n"

"a -= 48\n"

// {

//     // *0x10 = input("a:")
//     lang$ic$out$arg,
//     'a',
//     lang$ic$out$arg,
//     ':',
//     lang$ic$out$arg,
//     '\n',
//     lang$ic$in$cell,
//     0x10,

//     // *0x11 = input("b:")
//     lang$ic$out$arg,
//     'b',
//     lang$ic$out$arg,
//     ':',
//     lang$ic$out$arg,
//     '\n',
//     lang$ic$in$cell,
//     0x11,

//     // *0x00 = *0x10
//     lang$ic$copy$cell$0x00,
//     0x10,
//     // *0x00 -= '0'
//     lang$ic$sub$0x00$arg,
//     '0',
//     // *0x10 = *0x00
//     lang$ic$copy$0x00$cell,
//     0x10,

//     // *0x00 = *0x11
//     lang$ic$copy$cell$0x00,
//     0x11,
//     // *0x00 -= '0'
//     lang$ic$sub$0x00$arg,
//     '0',
//     // *0x11 = *0x00
//     lang$ic$copy$0x00$cell,
//     0x11,

//     // *00 = *0x10
//     lang$ic$copy$cell$0x00,
//     0x10,

//     // *00 += *0x11
//     lang$ic$add$0x00$cell,
//     0x11,

//     // *00 += '0'
//     lang$ic$add$0x00$arg,
//     '0',

//     // print(*0x00)
//     lang$ic$out$cell,
//     0x00,

// }
