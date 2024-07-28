int i , j
int x , y
#mg0001_run
damage Enemy , 50
stop
#mg0002_run
SelectCard Player , 68 , 1
x = Selected
j = SelectedFrom
if ( j == 4 ) then
y = Player
else
y = Enemy
endif
GetCardInfo x , j
j = CardOffense
j = j + ( j * 30 / 100 )
AP y , x , j
stop
#mg0003_run
SelectCard Player , 68 , 1
x = Selected
j = SelectedFrom
if ( j == 4 ) then
y = Player
else
y = Enemy
endif
GetCardInfo x , j
j = CardDefense
j = j - ( j * 30 / 100 )
DP y , x , j
stop
#mg0004_run
SelectCard Player , 68 , 1
x = Selected
j = SelectedFrom
if ( j == 4 ) then
y = Player
else
y = Enemy
endif
GetCardInfo x , j
j = CardDefense
j = j - ( j * 30 / 100 )
DP y , x , j
stop

