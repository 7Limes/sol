\ Print first n primes \
\ 62 bytes excluding newlines and tabs \ 

i1 1
"2 "
1j
4@_
1@
  2+#
  2@
    2-
    #1=3$?j_
    <<%0=4$?j_
    20j
  3@
    _!" "
    >1+
    <<t>
    l1$?j
