#include <stdio.h>
int gameboard[9]; /* global array */

int whos_move; /* global variable */

int check_for_winner()
{
  int return_value=0;

  /* check row 1 for all 3 elements being the same and not zero */
  if( (gameboard[0] == gameboard[1]) && (gameboard[1] == gameboard[2]) && (gameboard[0] != 0) )
    {
      return_value = gameboard[0];
    }
  /* check row 2 for all 3 elements being the same and not zero */
  else if( (gameboard[3] == gameboard[4]) && (gameboard[4] == gameboard[5]) && (gameboard[3] != 0) )
    {
      return_value = gameboard[3];
    }
  /* check row 3 for all 3 elements being the same and not zero */
  else if( (gameboard[6] == gameboard[7]) && (gameboard[7] == gameboard[8]) && (gameboard[6] != 0) )
    {
      /* found error here */
      return_value = gameboard[6];
    }
  /* check col 1 for all 3 elements being the same and not zero */
  else if( (gameboard[0] == gameboard[3]) && (gameboard[3] == gameboard[6]) && (gameboard[0] != 0) )
    {
      return_value = gameboard[0];
    }
  /* check col 2 for all 3 elements being the same and not zero */
  else if( (gameboard[1] == gameboard[4]) && (gameboard[4] == gameboard[7]) && (gameboard[1] != 0) )
    {
      return_value = gameboard[1];
    }
  /* check col 3 for all 3 elements being the same and not zero */
  else if( (gameboard[2] == gameboard[5]) && (gameboard[5] == gameboard[8]) && (gameboard[2] != 0) )
    {
      return_value = gameboard[2];
    }
  /* check \ */
  else if( (gameboard[0] == gameboard[4]) && (gameboard[4] == gameboard[8]) && (gameboard[0] != 0) )
    {
      return_value = gameboard[0];
    }
  /* check / */
  else if( (gameboard[2] == gameboard[4]) && (gameboard[4] == gameboard[6]) && (gameboard[2] != 0) )
    {
      return_value = gameboard[2];
    }
  /* now check for a tie */
  else
    {
      return_value = -1;
      for( int i=0; i<9; i++ )
	{
	  if( gameboard[i] == 0 )
	    {
	      return_value=0;
	    }
	}
    }

  return return_value;
}

int displayBoard()
{

  for( int i=0;i<50; i++ ) printf( "\n" );
  
  printf( " 0 | 1 | 2 \n" );
  printf( "-----------\n" );
  printf( " 3 | 4 | 5 \n" );
  printf( "-----------\n" );
  printf( " 6 | 7 | 8 \n" );
  printf( "\n\n" );
  if( gameboard[0] == 1 ) {printf( " X |" );}
  else if( gameboard[0] == 2 ){printf( " O |" );}
  else{printf( "   |" );}
  
  if( gameboard[1] == 1 ){printf( " X |" );}
  else if( gameboard[1] == 2 ){printf( " O |" );}
  else{printf( "   |" );}
  
  if( gameboard[2] == 1 ){printf( " X \n" );}
  else if( gameboard[2] == 2 ){printf( " O \n" );}
  else{printf( "   \n" );}
  
  printf( "-----------\n" );

  if( gameboard[3] == 1 ) {printf( " X |" );}
  else if( gameboard[3] == 2 ){printf( " O |" );}
  else{printf( "   |" );}
  
  if( gameboard[4] == 1 ){printf( " X |" );}
  else if( gameboard[4] == 2 ){printf( " O |" );}
  else{printf( "   |" );}
  
  if( gameboard[5] == 1 ){printf( " X \n" );}
  else if( gameboard[5] == 2 ){printf( " O \n" );}
  else{printf( "   \n" );}

  printf( "-----------\n" );

  if( gameboard[6] == 1 ) {printf( " X |" );}
  else if( gameboard[6] == 2 ){printf( " O |" );}
  else{printf( "   |" );}
  
  if( gameboard[7] == 1 ){printf( " X |" );}
  else if( gameboard[7] == 2 ){printf( " O |" );}
  else{printf( "   |" );}
  
  if( gameboard[8] == 1 ){printf( " X \n" );}
  else if( gameboard[8] == 2 ){printf( " O \n" );}
  else{printf( "   \n" );}
  
  return 0;
}
int main()
{
  for( int i=0; i<9; i++ )
    {
      gameboard[i] = 0;
    }
  whos_move = 1;
  int player_move = 0;
  int winner=0;

  
  while( winner == 0 )
    {
      displayBoard();

      int bad_move=0;
      printf( "It's player %i's move.\n", whos_move );
      printf( "Where to? (0-8): " );
      scanf( "%i", &player_move );
      
      if( gameboard[player_move] == 0 )
	{
	  gameboard[player_move] = whos_move;
	}
      else
	{
	  printf( "\n*** Invalid Move ... choose a different spot, please.  ***\n\n" );
	  bad_move=1;
	}
      
      if( (whos_move == 1) && (bad_move !=1) )
	{
	  whos_move = 2;
	}
      else if(bad_move != 1)
	{
	  whos_move = 1;
	}
      winner=check_for_winner();
  
    }
  
  displayBoard();
  if( winner == -1 )
    {
      /* then there was a tie */
      printf( "\n\n*** The Game (sadly) ended in a tie - how futile! ***\n\n" );
    }
  else
    {
      printf( "\n\n*** Found a winner... it's player %i! ***\n\n", check_for_winner() );
    }
  return(0);
}
