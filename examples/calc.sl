\ 4 function calculator \

\ get input \
"Enter a number: " i
"Enter another number: " i
"Enter an operation (+, -, *, /): " I_

\ decide which operation to do, jump \
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

\iiI_#43=1$?j_#45=2$?j_#42=3$?j_#47=4$?j1@_+!0j2@_-!0j3@_*!0j4@_/!0@\
