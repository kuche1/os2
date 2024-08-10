
// asks for a 1-len number
// asks for another 1-len number
// prints the num
// note that the result will look weird if it's >=10

"out$arg 97\n"
"out$arg 58\n"
"out$arg 10\n"
"var a\n"
"a = $getchar\n"

"out$arg 98\n"
"out$arg 58\n"
"out$arg 10\n"
"var b\n"
"b = $getchar\n"

"var ascii_0\n"
"cast ascii_0 char\n"
"ascii_0 = 48\n"

"a -= ascii_0\n"
"b -= ascii_0\n"

"a += b\n"

"a += ascii_0\n"
"out$cell a\n"
