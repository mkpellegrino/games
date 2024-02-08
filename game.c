#include <stdio.h>

/* these are needed for the rand() function */
#include <stdlib.h>
#include <time.h>

/* this is another file with my map in it */
#include "40x40blank.h"

/* player position in the array */
int player_x = 1;
int player_y = 1;

int monster1_x = 40;
int monster1_y = 40;

/* place to store the keyboard input */
char user_input[16];

int score=0;
/* 
   0- key
   1- sword
   2- treasure
   3- map
*/

int inventory[25] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int numberOfItems()
{
  int return_value=0;
  for( int i=0; i<25; i++ )
    {
      if( i !=2 ) return_value += inventory[i];
    }
  return return_value;
}

/*    int D = within( player_x, player_y, 7 );  */

int within(int x, int y, int L)
{
  int return_value = 0;
  if( (player_x-x)*(player_x-x) + (player_y-y)*(player_y-y) <= L*L ) return_value = 1;
  return return_value;
}

int hasKey()
{
  return inventory[0];
}

int playerLong(int monster_y )
{
  int return_value = 0;
  if( monster_y > player_y )
    {
      return_value = -1;
    }
  else if( monster_y < player_y )
    {
      return_value = 1;
    }
  return return_value;  
}

int playerLat(int monster_x )
{
  int return_value = 0;
  if( monster_x > player_x )
    {
      return_value = -1;
    }
  else if( monster_x < player_x )
    {
      return_value = 1;
    }
  return return_value;  
}

void updateMonsterPositions()
{
  for( int i=0; i<SIZEX; i++ )
    {
      for( int j=0; j<SIZEY; j++ )
	{
	  if( map[i][j] == 'x' )
	    {
	      int x_diff = playerLong( i );
	      int y_diff = playerLat( j );
	      if( map[i+x_diff][j] == ' ' )
		{
		  map[i][j] = ' ';
		  map[i+x_diff][j] = 'x';
		}
	      else if( map[i][j+y_diff] == ' ' )
		{
		  map[i][j] = ' ';
		  map[i][j+y_diff] = 'x';
		}
	    }
	}
    }
}

int hasMap()
{
  return inventory[3];
}

int isWall( int A, int B )
{
  if( ( map[player_y+A][player_x+B] == '*') || (map[player_y+A][player_x+B] == '#') ) return 1;
  return 0;
}

void showMap()
{
  printf( "\e[2J\e[H" );
  for( int i=0; i<SIZEX; i++ )
    {
      for( int j=0; j<SIZEY; j++ )
	{
	  if( i==player_y && j==player_x )
	    {
	      printf( "\e[37m&\e[0m" );
	    }
	  else
	    {
	      if( hasMap() )
		{
		  printf("%c", map[i][j] );
		}
	      else
		{
		  if( map[i][j]=='#' )
		    {
		      printf("\e[1m%c\e[0m", map[i][j] );
		    }
		  else
		    {
		      /* ANSI Escape Codes */
		      if( within( j, i, 3 ) ) printf( "\e[2m" );
		      if( within( j, i, 2 ) ) printf( "\e[0m" );
		      if( within( j, i, 1 ) ) printf( "\e[1m" );
		      if( within( j, i, 3 ) )
			{
			  printf("%c\e[0m", map[i][j] );
			}
		      else
			{
			  printf( "\e[0m " );
			}
		    }
		}
	    }
	}
      printf( "\n" );
    }
  /* reset escape sequence */
  printf( "\e[0m" );
}


int getInput()
{
  /* returns -1 if bad move */
  int return_value=-1;
  /* prompt */
  printf( "Score: %d\n> ", score );
  /* the input command */
  scanf( "%s", user_input );

  switch( user_input[0] )
    {
    case 'E':
    case 'e':
      if( map[player_y][player_x+1] == '|' && hasKey() )
	{
	  inventory[0]--;
	  score+=5;
	  map[player_y][player_x+1] = ' ';
	}
      if( !isWall(0,1) && map[player_y][player_x+1] != '|' )
	{
	  player_x++;
	  return_value = 1;
	}
      break;
    case 'W':
    case 'w':
      if( map[player_y][player_x-1] == '|' && hasKey() )
	{
	  inventory[0]--;
	  score+=5;
	  map[player_y][player_x-1] = ' ';
	}
      if( !isWall(0,-1) && map[player_y][player_x-1] != '|' )
	{
	  player_x--;
	  return_value = 1;
	}
      break;
    case 'N':
    case 'n':
      if( map[player_y-1][player_x] == '-' && hasKey() )
	{
	  inventory[0]--;
	  score+=5;
	  map[player_y-1][player_x] = ' ';
	}
      if( !isWall(-1,0) && map[player_y-1][player_x] != '-' )
	{
	  player_y--;
	  return_value = 1;
	}
      break;
    case 'S':
    case 's':
      if( map[player_y+1][player_x] == '-' && hasKey() )
	{
	  inventory[0]--;
	  score+=5;
	  map[player_y+1][player_x] = ' ';
	}

      if( !isWall(1,0) && map[player_y+1][player_x] != '-')
	{
	  player_y++;
	  return_value = 1;
	}
      break;
    case 'I':
    case 'i':
      return_value = 2;
      break;
    case '?':
    case 'H':
    case 'h':
      return_value = 3;
      break;
    case 'Q':
    case 'q':
    case 'X':
    case 'x':
      return_value = 4;
      break;
    default:
      break;
      
    }

  
  
  /* returns -1 or 1 */
  return return_value;
}

int main()
{
  srand( time(NULL) );
  time_t start_seconds=time(NULL);
  time_t end_seconds;

  int KEEP_PLAYING=1;
  showMap();
  printf( "\nQuest - By: Mr. Pellegrino\n" );
  printf( "Type: ? for help\n" );
  
  while(KEEP_PLAYING)
    {
      /* Get User Input */
      int i = getInput();
      updateMonsterPositions();
      showMap();
      int t;
      switch(i)
	{
	case 1:
	  {
	    switch( map[player_y][player_x] )
	      {
	      case 'K':
		printf( "key taken!\n" );
		inventory[0]++;
		score+=25;
		map[player_y][player_x]=' ';
		break;
	      case 'S':
		inventory[1] = 1;
		score+=50;
		printf( "sword taken!\n" );
		map[player_y][player_x]=' ';
		break;
	      case 'T':
		t = rand()%50+25;
		inventory[2] += t;
		score+=t;
		printf( "treasure worth $%d taken!\n", t );
		map[player_y][player_x]=' ';
		break;
	      case 'M':
		score+=50;
		inventory[3] = 1;
		printf( "you've got the map!\n" );
		map[player_y][player_x]=' ';
		showMap();
		break;
	      
	      }
	  }
	  break;
	case 2:
	  /* show inventory */
	  printf( "You have [%d] items\n", numberOfItems() );
	  if( hasKey() ) printf( "a key, " );
	  if( inventory[1] > 0 ) printf( "a sword, " );
	  if( inventory[2] > 0 )
	    {
	      printf( "$%d worth of treasure, ", inventory[2] );
	    }
	  if( inventory[3] > 0 ) printf( "the map, " );
	  if( inventory[4] > 0 ) printf( ", " );
	  if( inventory[5] > 0 ) printf( ", " );
	  if( inventory[6] > 0 ) printf( ", " );
	  if( inventory[7] > 0 ) printf( ", " );
	  if( inventory[8] > 0 ) printf( ", " );
	  if( inventory[9] > 0 ) printf( ", " );
	  if( inventory[10] > 0 ) printf( ", " );
	  if( inventory[11] > 0 ) printf( ", " );
	  if( inventory[12] > 0 ) printf( ", " );
	  if( inventory[13] > 0 ) printf( ", " );
	  if( inventory[14] > 0 ) printf( ", " );
	  if( inventory[15] > 0 ) printf( ", " );
	  if( inventory[16] > 0 ) printf( ", " );
	  if( inventory[17] > 0 ) printf( ", " );
	  if( inventory[18] > 0 ) printf( ", " );
	  if( inventory[19] > 0 ) printf( ", " );
	  if( inventory[20] > 0 ) printf( ", " );
	  if( inventory[21] > 0 ) printf( ", " );
	  if( inventory[22] > 0 ) printf( ", " );
	  if( inventory[23] > 0 ) printf( ", " );
	  if( inventory[24] > 0 ) printf( ", " );
	  printf( "\n" );
	  break;
	case 3:
	  printf( "valid commands:\n");
	  printf( "(N)orth\t(S)outh\t(E)ast\t(W)est\n" );
	  printf( "(I)nventory\t(H)elp\n" );
	  printf( "(Q)uit\n\n" );
	  printf( "things:\n" );
	  printf( "\e[37m&\e[0m represents the player\n" );
	  printf( "(M)ap\t(T)reasure\t(K)ey\t(S)word\n" );
	  printf( "|  or  -   is a locked door\n" );
	  printf( "* or # is a wall\n" );
	  break;
	case 4:
	  printf( "are you sure you want to quit? (Y)es or (N)o >" );
	  char SURE[2];
	  scanf( "%s", SURE );
	  if( SURE[0] == 'Y' || SURE[0] == 'y') KEEP_PLAYING=0;
	}
      
      /* Process Input */
    }
  printf( "Score: %d\n", score ); 
  printf( "Goodbye.\n\n" );
  return 0;
}
