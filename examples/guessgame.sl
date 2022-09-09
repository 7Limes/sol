"Guessing Game\n"
1 100r0

10@
  1+$
  "Guess a number between 1 and 100\n"
  i
  <<l20$?j_
  <<g30$?j__
  "You guessed it! The number was " !_ ".\n"
  "You guessed it in " !_ " tries."
  0j
  
  20@
  _
  "Too low, guess higher\n"
  $10j
  
  30@
  _
  "Too high, guess lower\n"
  $10j
0@