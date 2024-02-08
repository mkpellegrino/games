#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char unguessedletters[26]={"abcdefghijklmnopqrstuvwxyz"};
char guessedletters[26]={"                          "};
char currentword[20]={"                    "};
char words[NUM_WORDS][20]={ "debugging", "computer", "programming", "hacker", "basic", "logic", "conditional", "processor","algorithm","assembler", "syntax", "error", "language", "evaluate", "instruction", "compiler", "procedure", "pseudocode", "development", "function", "subroutine", "internet", "network"  };
void showMan(int level)
{
  printf( "\t/=====\\\n" );
  if( level > 0 ) printf( "\t  O  ||\n" );
  if( level == 0 )
    {
      for( int i=0;i<3;i++ ) printf( "\t     ||\n");
     }
  else if( level==1 )
    {
      for( int i=0;i<2;i++ ) printf( "\t     ||\n");
     }
  else if( level==2 )
    {
      printf( "\t  |  ||\n\t     ||\n");
    }
  else if( level==3 )
    {
      printf( "\t /|  ||\n\t     ||\n");
    }
  else if( level == 4 )
    {
      printf( "\t /|\\ ||\n\t     ||\n");
    }
  else if( level == 5)
    {
      printf( "\t /|\\ ||\n\t /   ||\n");
    }
  else
    {
      printf( "\t /|\\ ||\n\t / \\ ||\n");
    }
  printf( "\t     ||\n\t=======\n\n");
  /*printf( "\n\n[%s]\n\n", currentword );*/
}

int check( char aword[], char x )
{
  int ret=0;
  int i=0;
  /* printf( "Checking word: %s\n", aword ); */

  while( aword[i] != '\0' )
    {
      if( aword[i] == x )
	{
	  /* printf( "\t\tFound a(n) %c\n", x); */
	  ret++;
	}
      i++;
    }
  return( ret );
}

int removeGuessed( char x )
{
  int ret=0;
  /* a return value of 0 means it's already been guessed */
  for( int i=0; i<26; i++ )
    {
      if( x==unguessedletters[i] )
	{
	  guessedletters[i]=unguessedletters[i];
	  unguessedletters[i]=' ';
	  ret=1;
	}
    }
  return( ret );
}

void displayUnguessed()
{
  printf( "Letters you haven't guessed:\n" );
  for( int i=0; i<13; i++ )
    {
      printf( "%c ", unguessedletters[i] );
    }
  printf( "\n" );
  for( int i=13; i<26; i++ )
    {
      printf( "%c ", unguessedletters[i] );
    }
    
  printf( "\n\n" );
}

void displayGuessed()
{
  printf( "Letters you have guessed:\n" );
  for( int i=0; i<13; i++ )
    {
      printf( "%c ", guessedletters[i] );
    }
  printf( "\n" );
  for( int i=13; i<26; i++ )
    {
      printf( "%c ", guessedletters[i] );
    }
    
  printf( "\n\n" );
}

int foundin(char x)
{
  int ret=0;
  for( int i=0;i<26;i++ )
    {
      if( x == guessedletters[i] ) ret=1;
    }
  return( ret );
}

int displayBlanks( unsigned long length )
{
  printf( "Word: " );
  int ret=0;
  for( int i=0;i<length; i++ )
    {
      if( foundin( currentword[i] ))
	{
	  printf( "%c", currentword[i] );
	  ret++;
	}
      else
	{
	  printf( "-" );
	}
    }
  printf( "\n\n");
  return(ret);
}

int main(void)
{
#ifdef DEBUG
  printf( "*** DEBUG MODE ***\n" );
#endif

  srand(time(NULL));
  int playing=1;
  char again[5]={"     "};
  char cr='\0';
  int Won=0;
  printf( "\n\n\n\n\n\n\n=========================================================\n[- h a n g m a n   b y :   m r .   p e l l e g r i n o -]\n=========================================================\n\n");

  while( playing )
    {
      /* ==================================== */
      /* copy the string over to current word */
      char init='a';
      for( int i=0;i<=26; i++ )
	{
	  unguessedletters[i]=init++;
	  guessedletters[i]=' ';
	}
      int r=rand()%NUM_WORDS;
      int score=0;
      unsigned long L = strlen( words[r] );
      for( int i=0; i<L; i++ )
	{
	  currentword[i]=words[r][i];
	  currentword[i+1]='\0';
	}
      /* ==================================== */
      Won=0;
      while( score < 7 )
	{
	  char userGuess[]={"          "};
	  showMan(score);
	  if( displayBlanks( L )==L )
	    {
	      printf( "You've Won!!!\n\n" );
	      playing=0;
	      score=7;
	      Won=1;
	      break;
	    }
	  displayUnguessed();
	  printf( "Guess a letter:\n" );
	  scanf( "%s", &userGuess, &cr );
	  if( !check( currentword, userGuess[0] ))
	    {
	      if( !foundin(userGuess[0]) )
		{
		  score++;
		}
	    }
	  removeGuessed( userGuess[0] );
	}
      
      if( !Won )
	{
	  printf( "The word was: %s\n\n\n", currentword );
	  Won=0;
	}
      printf( "Would you like to play again? (y/n)\n");
      scanf( "%s", &again );
      if( again[0]=='n' || again[0] =='N' )
	{
	  return(0);
	}
      else
	{
	  playing=1;
	}
 
      /* reset everything */
      for( int i='a'; i<'z'; i++ )
	{
	  unguessedletters[i-'a']=i;
	  guessedletters[i-'a']=' ';
	}
    }  
  return(0);
}
