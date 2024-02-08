/* June 1, 2023 - # Guess Game */
/* By: Mr. Pellegrino */
/* Write a Number Guessing Game */
/* gcc ./2023-number-guess.c -o ./guess    */
/* ./guess                                 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/* program will keep running as long as this is = 1 */
int keep_playing = 1;

/* the # the computer picks randomly */
int the_number;
int guess_number = 1;
int user_guess;
int player_is_still_guessing = 1;

void displayProgramNameAndInstructions()
{
  printf( "\n\n\n\n\nNumber Guessing Game by: Mr. Pellegrino\nI'm thinking of an integer between 1 and 100\nTry to guess it in as few guesses as you can.\n" );
  return;
}

int main()
{
  srand(time(NULL)); 
  while( keep_playing == 1 )
    {
      displayProgramNameAndInstructions();
      the_number = (rand() % 100) + 1;
	  
      while( player_is_still_guessing == 1 )
	{
	  printf( "<shhh... the number is %d>\n", the_number );
	  printf( "Enter guess #%d: ", guess_number );
	  scanf( "%d", &user_guess );
	  if( user_guess > the_number )
	    {
	      printf( "You guessed too high... guess lower.\n" );
	      guess_number = guess_number + 1;
	    }
	  else if( user_guess < the_number )
	    {
	      printf( "You guessed too low... guess higher.\n" );
	      guess_number = guess_number + 1;
	    }
	  else
	    {
	      printf( "You guessed it in %d tries!\n", guess_number );
	      player_is_still_guessing = 0;
	    }
	}
      char play_again;
      printf( "Play again? (Y/N)\n" );
      scanf( "%s", &play_again );
      if( play_again == 'N' || play_again == 'n' )
	{
	  keep_playing = 0;
	}
      else
	{
	  player_is_still_guessing = 1;
	  guess_number = 1;
	}
    
    }
  return 0;
}
