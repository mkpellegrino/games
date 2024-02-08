Lbl 1
Disp "You Have 5 Tries"
Disp "to guess the"
Disp "Number That I am"
Disp "thinking of. GO!"
int(rand*32)+1->N
For(L,1,5)
	Disp "Guess #",L
	Input ":",G
	If (N=G)
	Then
		Disp "YOU WIN!!!"
		Goto 2
		Else
		If (N>G)
		Then
			Disp "Guess Higher"
		End
		If (N<G)
		Then
			Disp "Guess Lower"
		End
	End
End
Disp "YOU LOSE!!!"
Lbl 2
Disp "Game Over"
Disp "Again?  1=Yes"
Input X
If (X=1)
Goto 1
