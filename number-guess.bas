7 GOTO 30:REM SKIP ALL THE REM STATEMENTS AT THE BEGINNING
8 REM
9 REM  ------------------------------
10 REM  A BASIC Number Guessing Game
20 REM By Mr. Pellegrino - 2018 01 31
21 REM ------------------------------
22 REM
30 CLEAR:HOME:SCORE=0:REM              Clear Screen/Memory and RESET Score
40 ?"Welcome to the Guessing Game!":REM  TITLE SCREEN
50 ?"By: Mr. Pellegrino 01/31/2018"
60 ?"============================="
70 ?"  PRESS ANY KEY TO CONTINUE"
80 ?"============================="
90 IF(PEEK(49152)=0) THEN GOTO 1000:REM  While No key is pressed Get Random #

100 HOME:C=5:A$="":REM                   Clear Screen:5 Tries
105 FOR L=C TO 1 STEP -1: REM            Set a loop from #Tries to 1
110 HOME:?"Welcome to the Guessing Game!": REM Show TITLE AGAIN
120 ?"By: Mr. Pellegrino 01/31/2018"
130 ?"============================="
135 IF A$<>"" THEN ?:? GUESS;:? A$:?:REM IF Already Guessed, Show too high/low
140 ?"You have ";:? L;:? " guesses left and":? "your score is: ";:?SCORE:REM ? Number of Guesses left & Score
150 ?:?"I'm thinking of a number":?"between 1 and 100"
160 ?:INPUT "What's your guess?";GUESS:REM   Get a guess from user
165 REM CHECK IF GUESS is Spot on, or too high or low
170 IF GUESS=N THEN GOTO 240
180 IF GUESS<N THEN A$=" is too LOW - Guess again please."
190 IF GUESS>N THEN A$=" is too HIGH - Guess again please."
200 NEXT L: REM Go to top of loop
210 REM THEY DIDN'T GET IT AFTER C Tries
220 ?:? "The Number was: ";:? N:GOTO 250
230 REM THEY GOT IT IN C-L Tries
240 ?:?"You Got It!!!":SCORE=SCORE+L*10:L=0:REM Increase score and set L=0
250 INPUT "Would you like to play again (Y/N)?";YN$:REM Self Explanitory
260 IF (YN$="Y" OR YN$="y") THEN N=INT(RND(1)*100)+1:GOTO 100
270 ?:?"Your Final Score is: ";:? SCORE:END:REM Show final score and End

1000 REM GET A NEW RANDOM NUMBER
1010 N=INT(RND(1)*100)+1
1020 GOTO 90
