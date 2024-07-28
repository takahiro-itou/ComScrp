function FuncSample1
int a , b , c
a = 1
b = 2
c = a + b
result c
endfunc
function FuncSample2 a , b
int c
c = a * b
result c
endfunc
#main
int x
call FuncSample2 , 5 , 8
x = RetValue
mes 5 * 8 =  x
prototype function FuncSample3 a , b
call FuncSample3 , 32 , 4
x = RetValue
mes 32 / 4 =  x
end
function FuncSample3 a , b
result a / b
endfunc

