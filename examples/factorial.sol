\get factorial of inputted number (>= 3)\
\21 bytes\

\setup\
i#1-
0@
\multiply, sub 1 from count\
#>*$1-
\check if 1, jump\
#0$1~?j
\print final answer\
t!
