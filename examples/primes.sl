i 1 1
"2 "
10j

40@_
10@
  2+#
  20@
    2-
    \ check loop cond (if true, then prime)\
    #1=30$?j_
    \ check divisible \
    <<%0=40$?j_  \ is divisible (not prime) \
    \ is not divisible (continue loop) \
    20j
    
  30@
    \print prime\
    _!" "
    \inc count\
    >1+
    \check outer loop cond\
    <<t>
    l10$?j

\"readable" version
i1 1
"2 "
10j
40@_
10@
  2+#
  20@
    2-
    #1=30$?j_
    <<%0=40$?j_
    20j
  30@
    _!" "
    >1+
    <<t>
    l10$?j
\