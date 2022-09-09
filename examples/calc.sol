\ 4 function calculator \

\ get input \
"Enter a number: " i
"Enter another number: " i
"Enter an operation (+, -, *, /): " I_

\ check which operation to do, jump \
#43=10$?j
_#45=11$?j
_#42=12$?j
_#47=13$?j
20j

"Answer: "
\ do calculations and print \
10@
_+!0j

11@
_-!0j

12@
_*!0j

13@
_/!0j

20@
"Unsupported operation"
0@
