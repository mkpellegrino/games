#include "stdio.h"
#include "time.h"
#include "stdlib.h"

char gallows[8][4] =
  {
    {"****"},
    {"*   "},
    {"*   "},
    {"*   "},
    {"*   "},
    {"****"},
    {"*  *"},
    {"****"}
  };

char word_list[5][30] =
  {
    /*123456789012345678901234567890*/
    {"COMPUTER PROGRAMMING"},
    {"BEVERLY"},
    {"HANGMAN"},
    {"PROGRAM"},
    {"MACARONI"}
  };

char guessed_letters[28] = "                           \0";
char unknown_word[30];
char word_buffer[30];
char users_guess;
int number_of_wrong_guesses = 0;
int word_size=0;

void initialiseUnknownWord( int s )
{
  word_size = 0;
  /* get the length of the word */
  while( word_list[s][word_size] != '\0' )
    {
      word_size++;
    }
  
  for( int i = 0; i < word_size; i++ )
    {
      if( word_list[s][i] != ' ' )
	{
	  unknown_word[i] = '.';
	  word_buffer[i] = word_list[s][i];
	}
      else
	{
	  unknown_word[i] = '-';
	}
    }
  word_buffer[word_size+1] = '\0';
  unknown_word[word_size+1] = '\0';

  printf( "[word size: %i\t\tword: %s]\n", word_size, word_list[s] );
  return;
}

void displayInstructions()
{
  printf( "HANGMAN - By Mr. Pellegrino\n2023 06 05\n" );
  return;
}

void displayTheUnknownWord()
{
  printf( "The Unknown Word: %s\n", unknown_word );
  return;
}

void displayTheAlreadyGuessedLetters()
{
  printf( "Letters already guessed: %s\n",  guessed_letters);
  return;
}

void displayGallows( int incorrect_guesses )
{
  if( incorrect_guesses == 1 )
    {
      gallows[1][2] = 'O';
    }
  if( incorrect_guesses == 2 )
    {
      gallows[2][2] = '|';
    }
  if( incorrect_guesses == 3 )
    {
      gallows[2][1] = '/';
    }
  if( incorrect_guesses == 4 )
    {
      gallows[2][3] = '\\';
    }
  if( incorrect_guesses == 5 )
    {
      gallows[3][1] = '/';
    }
  if( incorrect_guesses == 6 )
    {
      gallows[3][3] = '\\';
    }
  
  for( int i = 0; i < 8; i++ )
    {
      for( int j = 0; j<4; j++ )
	{
	  printf( "%c", gallows[i][j] );
	}
      printf( "\n" );
    }
  return;
}

void addGuessedLetter( char c )
{
  guessed_letters[(c & 0x1F)-1] = c; 
  return;
}

int putTheLetterIntoTheUnknownWord( char c )
{
  /* if the letter IS in the unknown word */
  /* then return 1                        */
  /* otherwise return -1                  */

  /* iterate over each letter, when a matching letter is found, it in unknown_word at the correct index */
  int number_of_correct_letters_found = -1;
  int i = 0;
  while( word_buffer[i] != '\0' )
    {
      if( word_buffer[i] == c )
	{
	  unknown_word[i] = c;
	  number_of_correct_letters_found++;
	}
      else if(  word_buffer[i] == ' ' )
	{
	  // do nothing if its a space
	  printf( "\t\t\t****\n" );
	}
      i = i + 1;
    }
  
  return number_of_correct_letters_found;
}

int checkForWinner()
{
  for( int i = 0; i < word_size; i++ )
    {
      if( unknown_word[i] == '.' ) return -1;
    }
  return 1;
}

int main()
{
  /* see the random number generator */
  /* ONLY DO THIS ONCE!              */
  srand(time(NULL));

  
  int x;
  displayInstructions();
  /* pick a random word form the list */
  //int which_word = rand()%5;
  int which_word = 0;
  initialiseUnknownWord( which_word );

  while( number_of_wrong_guesses < 6 )
    {
      x = 0;
      displayGallows( number_of_wrong_guesses );
      displayTheUnknownWord();
      displayTheAlreadyGuessedLetters();
      /* get the users guess */

      /* Wednesday - 6/7/2023 */
      
      printf( "What's your guess?\n" );
      scanf( "%c", &users_guess );
      
      /* ^^^^^^^^^^^^^^^^^^^^ */
      if( users_guess != '\n' )
	{
	  // Force Uppercase
	  users_guess = users_guess & 0x5F;
	  
	  addGuessedLetter( users_guess );
	  x = putTheLetterIntoTheUnknownWord( users_guess );
	  if( x == -1 )
	    {
	      /* the letter is NOT in the word */
	      /* display "NOPE" */
	      number_of_wrong_guesses = number_of_wrong_guesses + 1;
	    }
	  else
	    {
	      /* the letter IS in the word */
	      /* if the word has been solved they've won */
	      if( checkForWinner() == 1)
		{
		  printf( "You've Guessed the word!!!\n" );
		  exit(1);

		}
	    }
	}

    }
  displayGallows( number_of_wrong_guesses );
  printf( "The word was: %s\n", word_list[which_word] );
  return 0;
}
    
