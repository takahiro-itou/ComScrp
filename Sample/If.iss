int a , b
a = 0
if ( a == 0 ) then
b = 1
endif
a = 1
if ( a == 0 ) then
b = 2
endif
mes b
a = 1
if ( a == 0 ) then
b = 3
else
b = 4
endif
mes b
a = 6
if ( a <= 2 ) then
b = 10
elseif ( a <= 4 ) then
b = 20
elseif ( a <= 6 ) then
b = 30
elseif ( a <= 8 ) then
b = 40
else
b = 50
endif
mes b

