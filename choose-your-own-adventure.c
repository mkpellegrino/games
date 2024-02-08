#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Name: Mr. Pellegrino
   Date: 2019 11 20
   Prog: Choose Your Own Adventure
   Desc: A Choose Your Own Adventure Game */

/* this is for the text parser */
/* each node in linked list    */
/* will contain a word         */
struct Node 
{ 
    void  *data;
    struct Node *next; 
};

struct Node *start=NULL;

void push(void *new_data) 
{
#ifdef DEBUG
  printf( "[ push( %s ) ]\n", new_data );	  
#endif

  struct Node** head_ref=&start;
  size_t data_size = strlen(new_data);
  struct Node* new_node = (struct Node*)malloc(sizeof(struct Node)); 
  new_node->data  = malloc(data_size); 
  new_node->next = (*head_ref); 
  for (int i=0; i<data_size; i++) *(char *)(new_node->data + i) = *(char *)(new_data + i); 
  (*head_ref)    = new_node; 
}

char* pop()
{
  if( listSize() == 0 ) return NULL;
  struct Node** head_ref=&start;
  struct Node** tmp_ref=start->next;
  char* x = start->data;
  free(*head_ref);
  start=tmp_ref;
#ifdef DEBUG
  printf( "[ pop( %s ) ]\n", x );	  
#endif
  return x;
}

int listSize()
{
  int retVal=0;
  struct Node *node = start;
  while (node != NULL) 
    {
      node = node->next; 
      retVal++;
    }
  return retVal;
}

void dumpList() 
{
  struct Node *node = start;
  while (node != NULL) 
    {
      printf( "%s\n", node->data );
      node = node->next; 
    }
}


int mapofthings[]={0,0,0,0,0,0,0,0,0,0, \
		   0,0,0,0,0,0,0,0,0,0, \
		   0,0,0,0,0,0,0,0,0,0, \
		   0,0,0,0,0,0,0,0,0,0, \
		   0,0,0,0,0,0,0,0,0,0, \
		   0,0,0,0,0,0,0,0,0,0, \
		   0,0,0,0,0,0,0,0,0,0, \
		   0,0,0,0,0,0,0,0,0,0, \
		   0,0,0,0,0,0,0,0,0,0, \
		   0,0,0,0,0,0,0,0,0,0 };

int mapofentities[]={0,0,0,0,0,0,0,0,0,0, \
		     0,0,0,0,0,0,0,0,0,0, \
		     0,0,0,0,0,0,0,0,0,0, \
		     0,0,0,0,0,0,0,0,0,0, \
		     0,0,0,0,0,0,0,0,0,0, \
		     0,0,0,0,0,0,0,0,0,0, \
		     0,0,0,0,0,0,0,0,0,0, \
		     0,0,0,0,0,0,0,0,0,0, \
		     0,0,0,0,0,0,0,0,0,0, \
		     0,0,0,0,0,0,0,0,0,0 };

int bagofthings[]={0,0,0,0,0,0,0,0,0};

char stuff[]="         ";

int addtobag(int t)
{
  int retVal=0;
  if( howmuchstuff() < 10 )
    {
      bagofthings[howmuchstuff()+1]= t;
      retVal=howmuchstuff();
    }
  else
    {
#ifdef DEBUG
      printf( "[bag is full]\n" );
#endif
      retVal=-1;
    }
  return retVal;
}

int howmuchstuff()
{
  int retVal=0;
  for( int i=0; i<10; i++ )
    {
      if( bagofthings[i]==0 )
	{
	  retVal=i;
	  break;
	}
    }
  return retVal;
}

void cantGoThere()
{
  printf( "\n" );
  printf( "You can't go in that direction.\n" );
  printf( "\n" );
  
  return;
}
int parseObject( char* string )
{
  char thing[]="     ";
  int l=strlen(string);
  
  for( int i=0; i<l-3; i++ )
    {
      if( string[i] == ' ' )
	{
	  thing[0]=string[i+1];
	  thing[1]=string[i+2];
	  thing[2]=string[i+3];
	  thing[3]=string[i+4];
	  thing[4]=string[i+5];
	  thing[5]=0;
	  i=l;
	}
    }

  
  printf( "%s\n", thing );  
  return -1;

}

// 1,2,3,4 -> North South East West
// 5,6 -> Up Down
// 7,8 -> Get Put

int isSame( char* str1, char* str2 )
{
#ifdef DEBUG
  printf( "Comparing [%s] and [%s]\n", str1, str2 );
#endif
  /* are they the same lengths? */
  /* does it really matter?     */
  /* should we just check the   */
  /* the number of characters   */
  /* in the shortest string?    */
  
  int l1=strlen( str1 );
  int l2=strlen( str2 );
  
  if( l1 != l2 ) return 0;

  for( int i=0;i<l1; i++ )
    {
      /* compare only lowercase letters a to z */
      /* punctuation doesn't count             */
      if( (str1[i] >= 'a' && str1[i] <= 'z') || (str1[i] >= 'a' && str1[i] <= 'z') )
	{
	  if( (str1[i]|32) != (str2[i]|32) ) return 0;
	}
    }
  return 1;
}

int parse( char* string )
{
  int retVal=-1;
  int i=strlen(string);
  if( isSame( string, "climb wall" ))
    {
      printf( "!!!\n" );
    }
    
  if( isSame( string, "climb" ) )
    {
      parseObject(string);
      retVal=1;
    }
  else if( isSame(string,"close"))
    {
 #ifdef DEBUG      
      printf("you close something.\n");
 #endif
      parseObject(string);
      retVal=1;
    }
  else if( isSame(string,"down"))
    {
 #ifdef DEBUG      
      printf("you go down.\n");
 #endif
      retVal=6;
    }
  else if( isSame(string,"drink")) 
    {
 #ifdef DEBUG
      printf("you drink.\n");
 #endif
      parseObject(string);
      retVal=1;
    }
  else if( isSame(string,"east"))
    {
 #ifdef DEBUG
      printf("you go east.\n");
 #endif
      retVal=3;
    }
  else if( isSame(string,"eat"))
  {
 #ifdef DEBUG
      printf("you eat.\n");
 #endif
      parseObject(string);
      retVal=1;
    }
  else if( isSame(string,"get"))
    {
 #ifdef DEBUG
      printf("you get something.\n");
 #endif
      parseObject(string);
      retVal=7;
    }
  else if( isSame(string,"go"))
    {
 #ifdef DEBUG
      printf("you go somewhere.\n");
 #endif
      parseObject(string);

      retVal=1;
    }
  else if( isSame(string,"help"))
    {
      printf( "valid commands:\nclimb\tclose\tdown\ndrink\teast\teat\tget\ngo\t\help\tjump\tkill\nlock\tlook\tnorth\topen\nplace\tput\tquit\trestart\nsouth\tunlock\tup\tuse\nwait\t\west\n\n" );
      printf( "valid objects:\nkey\tsword\tdoor\tchest\nwindow\tfood\twall\n\n" );
      retVal=-1;
    }
  else if( isSame(string,"jump"))
  {
 #ifdef DEBUG
      printf("you jump.\n");
 #endif
      retVal=1;
    }
  else if( isSame(string,"kill"))
    {
      
 #ifdef DEBUG
      printf("you kill something.\n");
 #endif
      parseObject(string);
      retVal=1;
    }
  else if( isSame(string,"lock"))
    {      
 #ifdef DEBUG
      printf("you lock something.\n");
 #endif
     parseObject(string);
     
     retVal=1;
    }
  else if( isSame(string,"look"))
    {
      /* display current room description */
 #ifdef DEBUG
      printf("you look.\n");
 #endif
      retVal=-1;
    }
  else if( isSame(string,"north"))
    {
 #ifdef DEBUG
      printf("you go north.\n");
 #endif
      retVal=1;
    }
  else if( isSame(string,"open"))
    {
 #ifdef DEBUG
      printf("you open something.\n");
 #endif
      parseObject(string);
      
      retVal=1;
    }
  else if( isSame(string,"place"))
    {
 #ifdef DEBUG
      printf("you place something.\n");
 #endif
     parseObject(string);

      retVal=1;
    }
    else if( isSame(string,"put"))
    {
      
 #ifdef DEBUG
      printf("you put something.\n");
 #endif
     parseObject(string);

      retVal=8;
    }
    else if( isSame(string,"quit"))
    {
      /* quit the game... NOOOOOO! */
      exit(-1);
    }
  else if( isSame(string,"restart"))
    {
      
 #ifdef DEBUG
      printf("you restart the game.\n");
 #endif
      retVal=1;
    }
  else if( isSame(string,"south"))
    {
      
#ifdef DEBUG
      printf("you go south.\n");
#endif
      retVal=2;
    }
    else if( isSame(string,"unlock"))
      {
	
#ifdef DEBUG
	printf("you unlock something.\n");
#endif
	parseObject(string);
	
	retVal=1;
      }
    else if( isSame(string,"up"))
    {
      
 #ifdef DEBUG
      printf("you go up.\n");
 #endif
      retVal=5;
    }
    else if( isSame(string,"use"))
    {
      printf("you use: ");
      parseObject(string);
      retVal=1;
    }
    else if( isSame(string,"wait"))
    {
      
 #ifdef DEBUG
      printf("you wait.\n");
 #endif
      retVal=1;
    }
    else if( isSame(string,"west"))
    {
      
 #ifdef DEBUG
      printf("you go west.\n");
 #endif
      retVal=4;
    }
  else 
    {
 #ifdef DEBUG
      printf( "I don't understand.\n" );
 #endif
      retVal=-1;
    }
  
  return(retVal);
}

int prompt()
{
  printf( "> " );  

  int r=0;
  char c=0;
  char * str = (char *)malloc(sizeof(char)*2);
  int i=0;
  int j=0;
  while (c != '\n')
    {
      // read the input from keyboard standard input
      c = getc(stdin);
      
      // make is all caps.
      if( c<='Z' && c>='A' ) c=c|32;
      
      // printf( "[%i] ", c );
      // re-allocate (resize) memory for character read to be stored
      //printf( "%c\t", c );
      if( c!= 10 )
	{
	  str = (char*)realloc(str, j * sizeof(char));
	}
      // store read character by making pointer point to c
      str[i] = c;
      i++;
      j++;
    }
  str[i-1] = '\0'; // at the end append null character to mark end of string
  //r=parse(str);


  printf( "here\n" );
  for( int k=strlen(str); k>=0; k-- )
    {
      if( str[k]==' ' )
	{
	  push( (char*)(str+k*sizeof(' ') ));
	  str[k]='\0';
	}

    }
  free(str);
  return r;  // returns which command was entered as a numeric value
}


int Room411()
{
  int r=411;
  printf( "You are in Mr. Pellegrino's classroom.\nThere is a door to the north.\nThere is a door to the east.\n" );
  switch( prompt() )
    {
    case 1:
      r=400;
      break;
    case 3:
      r=409;
      break;
    case 2:
    case 4:
    case 5:
    case 6:
      cantGoThere();
      break;
      
    case -1:
    default:
      r=411;
    }
  return r;
}

int Room409()
{
  int r=409;
  printf( "You are in Ms. Delisi's classroom.\n");
  printf( "There are doors to the north, west, and east.\n" );

  if( stuff[0] == ' ' ) printf( "\nThere is an Elevator Pass in the room.\n\n" );
  
  switch( prompt() )
    {
    case 1:
      r=400;
      break;
    case 4:
      r=411;
      break;
    case 7:
      r=409;
      if( stuff[0] == ' ' ) 
	{
	  printf( "\nYou take the elevator pass.\n\n" );
	  stuff[0]='@';
	}
      break;
    case 2:
    case 3:
    case 5:
    case 6:
      cantGoThere();
      break;
    default:
      r=409;
    }
  return r;
}

int Room400()
{
  int r=400;
  printf( "You are in the hallway.\n There is a door to Ms. Delisi's classroom.\nThere is a door to Mr. Pellegrino's classroom.\n" );
  switch( prompt() )
    {
    case 1:
      r=409;
      break;
    case 2:
      r=411;
      break;
    default:
      r=400;
    }
  return r;


}

void dump()
{
  for( int i=0; i<9; i++ )
    {
      printf( "Inventory: %c\n", stuff[i] );
    }
  return;
}

int main()
{
  
  printf( "Text Adventure Game\n" );
  int roomNumber=Room411();
  while( roomNumber != 400 )
    {
      switch( roomNumber )
	{
	case 409:
	  roomNumber=Room409();
	  break;
	case 411:
	  roomNumber=Room411();
	  break;
	case 400:
	  roomNumber=Room400();
	  break;
	default:
	  break;
	}
    }
  printf( "\n\n\t\t\tYou Win!!!\n\n" );
  return 0;
}


