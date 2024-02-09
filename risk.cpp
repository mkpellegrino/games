#include <iostream>
#include <vector>
#include <string>
#include <algorithm>    // std::sort
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


// NORTH AMERICA
#define ALASKA 0
#define NORTHWEST_TERRITORY 1
#define ALBERTA 2
#define GREENLAND 3
#define ONTARIO 4
#define QUEBEC 5
#define WESTERN_US 6
#define EASTERN_US 7
#define CENTRAL_AMERICA 8
#define ICELAND 9

// SOUTH AMERICA
#define VENEZUELA 10
#define PERU 11
#define ARGENTINA 12
#define BRAZIL 13

// EUROPE
#define GREAT_BRITAIN 14
#define SCANDANAVIA 15
#define NORTHERN_EUROPE 16
#define WESTERN_EUROPE 17
#define SOUTHERN_EUROPE 18
#define UKRAINE 19

// AFRICA
#define NORTH_AFRICA 20
#define EAST_AFRICA 21
#define EGYPT 22
#define CONGO 23
#define SOUTH_AFRICA 24
#define MADAGASCAR 25

// AUSTRALIA
#define NEW_GUINEA 26
#define INDONESIA 27
#define WESTERN_AUSTRALIA 28
#define EASTERN_AUSTRALIA 29

// ASIA
#define URAL 30
#define SIBERIA 31
#define YAKUTSK 32
#define IRKUTSK 33
#define JAPAN 34
#define MONGOLIA 35
#define AFGHANISTAN 36
#define THE_MIDDLE_EAST 37
#define INDIA 38
#define CHINA 39
#define SIAM 40
#define KAMCHATKA 41
  

using namespace std;

vector<int> attacker_dice;
vector<int> defender_dice;



class Player
{
public:
  Player( string * s = new string("no_name"), int strat = 0 )
  {
#ifdef DEBUG
    cerr << "creating player " << *s << endl;
#endif
    name = s;
    strategy = strat;
  }
  
  ~Player()
  {
#ifdef DEBUG
    cerr << "deleting player " << *name << endl;
#endif    
    delete name;
    strategy = 0;
  }
  friend ostream& operator <<(ostream& out, const Player& p);

private:
  string * name;
  int strategy;
};

ostream& operator <<(ostream& out, const Player& p)
{
  out << *p.name;
  return out;
}; 


vector<Player*> players;

int country_index=0;

class Country
{
public:
  // Copy Constructor
  Country( Country * c )
  {
#ifdef DEBUG
    cerr << "copying country " << *c << endl;
#endif
    id = c->id;
    //name = c->name;
    random_number = c->random_number;
    number_of_troops = c->number_of_troops;
    player = c->player;
  }
  
  Country(string * s, int id_for_vector, int n = 0, int p = 0)
  {
#ifdef DEBUG
    cerr << "creating country " << *s << endl;
#endif
    //id=country_index++;
    id=id_for_vector;
    name = s;
    number_of_troops = n;
    player = p;
    random_number = (rand() % 42);
  };
  ~Country()
  {
#ifdef DEBUG
    cerr << "deleting country " << *name << endl;
#endif    
    delete name;
    id=0;
    number_of_troops = 0;
  };
  void setPlayer( int p ) { player = p; };
  void setNumber( int n ) { number_of_troops = n; };
  int getNumber() { return number_of_troops; };
  int getPlayer() { return player; };
  int troopReduction() { return(--number_of_troops); };
  int getID(){ return id; };
  int getRandomNumber(){ return random_number; };
  void addTroops( int x ){ number_of_troops += x; };
  void addBorderingCountry( Country * c )
  {
#ifdef DEBUG
    cerr << "adding border country " << *c << endl;
#endif
    bordering_country.push_back( c );
  };

  friend ostream& operator <<(ostream& out, const Country& c);
  vector<Country*> bordering_country;
  // for sorting purposes
  bool operator< (const Country &other) const
  {
    return id < other.id;
  }

  int getNumberOfFriendlyNeighbors(int p)
  {
    int return_value = 0;
    for( int i = 0; i< bordering_country.size(); i++ )
      {
	if( bordering_country[i]->getPlayer() == p )
	  {
	    return_value++;
	  }
      }
    return return_value;
  }
private:
  int id;
  string * name;
  int number_of_troops;
  int player;
  int random_number;
};

ostream& operator <<(ostream& out, const Country& c)
{
  out << *c.name << " (" << "P:" << c.player << " T:" << c.number_of_troops << ")";
#ifdef DEBUG
  out << " [" << c.random_number << "]";
#endif
  // for( int i = 0; i < c.bordering_country.size(); i++ )
  //   {
  //     //if( c.player != c.bordering_country[i]->getPlayer() )
  // 	  {
  // 	    out << "\n\t" << *c.bordering_country[i]->name << " owned by " << *players[c.bordering_country[i]->getPlayer()] << " with " << c.bordering_country[i]->getNumber() << " troops";
  // 	  }
  //   }
  return out;
}; 

vector<Country*> countries;

void displayPlayers()
{
  for( int i = 0; i < players.size(); i++ )
    {
      cout << *players[i] << endl << endl;
    }
  return;
}

void displayPlayersFriendlyCountries( int p )
{
  for( int i = 0; i < countries.size(); i++ )
    {
      if( countries[i]->getPlayer() == p && countries[i]->getNumberOfFriendlyNeighbors(p) > 0 )
	{
	  cout << "(" << i << ") " << *countries[i] << endl;
	}
    }
  return;
}

void displayPlayersCountries( int p )
{
  for( int i = 0; i < countries.size(); i++ )
    {
      if( countries[i]->getPlayer() == p )
	{
	  cout << "(" << i << ") " << *countries[i] << endl;
	}
    }
  return;
}

void displayCountries()
{
  for( int i = 0; i < countries.size(); i++ )
    {
      cout << *countries[i] << endl;
    }
  return;
}

void deleteAll()
{
  for( int i = 0; i < countries.size(); i++ )
    {
      delete countries[i];
    }
  for( int i = 0; i < players.size(); i++ )
    {
      delete players[i];
    }
  return;
}

int die()
{
  return (rand() % 6) + 1;
}

int main()
{
  srand(time(NULL));   // Initialization of Random Number Generator, should only be called once.

  players.push_back( new Player( new string( "Able" )));
  players.push_back( new Player( new string( "Baker" )));
  players.push_back( new Player( new string( "Charlie" )));

  // CREATE THE NODES
  countries.push_back( new Country( new string("Alaska"), ALASKA, 1, 0));
  countries.push_back( new Country( new string("Northwest Territory"), NORTHWEST_TERRITORY, 1, 0));
  countries.push_back( new Country( new string("Alberta"), ALBERTA, 1, 0));  
  countries.push_back( new Country( new string("Greenland"), GREENLAND, 1, 0));
  countries.push_back( new Country( new string("Ontario"), ONTARIO, 1, 0));
  countries.push_back( new Country( new string("Quebec"), QUEBEC, 1, 1));
  countries.push_back( new Country( new string("Western U.S."), WESTERN_US, 1, 0));
  countries.push_back( new Country( new string("Eastern U.S."), EASTERN_US, 1, 0));
  countries.push_back( new Country( new string("Central America"), CENTRAL_AMERICA, 1, 0));
  countries.push_back( new Country( new string("Iceland"), ICELAND, 1, 0));
  countries.push_back( new Country( new string("Kamchatka"), KAMCHATKA, 1, 0));
  countries.push_back( new Country( new string("Great Britain"), GREAT_BRITAIN, 1, 0));
  countries.push_back( new Country( new string("Venezuela"), VENEZUELA, 1, 0 ));
  countries.push_back( new Country( new string("Peru"), PERU, 1, 0 ));
  countries.push_back( new Country( new string("Argentina"), ARGENTINA, 1, 0 ));
  countries.push_back( new Country( new string("Brazil"), BRAZIL, 1, 0 ));
  countries.push_back( new Country( new string("Scandanavia"), SCANDANAVIA, 1, 0 ));
  countries.push_back( new Country( new string("Northern Europe"), NORTHERN_EUROPE, 1, 0 ));
  countries.push_back( new Country( new string("Western Europe"), WESTERN_EUROPE, 1, 0 ));
  countries.push_back( new Country( new string("Southern Europe"), SOUTHERN_EUROPE, 1, 0 ));
  countries.push_back( new Country( new string("Ukraine"), UKRAINE, 1, 0 ));
  countries.push_back( new Country( new string("North Africa"), NORTH_AFRICA, 1, 0 ));
  countries.push_back( new Country( new string("East Africa"), EAST_AFRICA, 1, 0 ));
  countries.push_back( new Country( new string("Egypt"), EGYPT, 1, 0 ));
  countries.push_back( new Country( new string("Congo"), CONGO, 1, 0 ));
  countries.push_back( new Country( new string("South Africa"), SOUTH_AFRICA, 1, 0 ));
  countries.push_back( new Country( new string("Madagascar"), MADAGASCAR, 1, 0 ));
  countries.push_back( new Country( new string("New Guinea"), NEW_GUINEA, 1, 0 ));
  countries.push_back( new Country( new string("Indonesia"), INDONESIA, 1, 0 ));
  countries.push_back( new Country( new string("Western Australia"), WESTERN_AUSTRALIA, 1, 0 ));
  countries.push_back( new Country( new string("Eastern Australia"), EASTERN_AUSTRALIA, 1, 0 ));
  countries.push_back( new Country( new string("Ural"), URAL, 1, 0 ));
  countries.push_back( new Country( new string("Siberia"), SIBERIA, 1, 0 ));
  countries.push_back( new Country( new string("Yakutsk"), YAKUTSK, 1, 0 ));
  countries.push_back( new Country( new string("Irkutsk"), IRKUTSK, 1, 0 ));
  countries.push_back( new Country( new string("Japan"), JAPAN, 1, 0 ));
  countries.push_back( new Country( new string("Mongolia"), MONGOLIA, 1, 0 ));
  countries.push_back( new Country( new string("Afghanistan"), AFGHANISTAN, 1, 0 ));
  countries.push_back( new Country( new string("The Middle East"), THE_MIDDLE_EAST, 1, 0 ));
  countries.push_back( new Country( new string("India"), INDIA, 1, 0 ));
  countries.push_back( new Country( new string("China"), CHINA, 1, 0 ));
  countries.push_back( new Country( new string("Siam"), SIAM, 1, 0 ));


  // RANDOMIZE THE COUNTRIES AND GIVE 14 TO EACH OF THE 3 PLAYERS
  sort(countries.begin(), countries.end(), [](auto const& lhs, auto const& rhs) {return lhs->getRandomNumber() < rhs->getRandomNumber();});

  // 35 armies each
  int armies=35;
  for( int i = 0; i < 14; i++ )
    {
      countries[i]->addTroops(1+rand()%5);
      countries[i]->setPlayer(0);
    }
  for( int i = 14; i < 28; i++ )
    {
      countries[i]->addTroops(1+rand()%5);
      countries[i]->setPlayer(1);
    }
  for( int i = 28; i < 42; i++ )
    {
      countries[i]->addTroops(1+rand()%5);
      countries[i]->setPlayer(2);
    }
  
  // SORT THE COUNTRIES SO THAT WE CAN USE THE #DEFINE as an INDEX  
  sort(countries.begin(), countries.end(), [](auto const& lhs, auto const& rhs) {return lhs->getID() < rhs->getID();});
      
  
  // CREATE THE CONNECTIONS
  countries[ALASKA]->addBorderingCountry( countries[NORTHWEST_TERRITORY] );
  countries[ALASKA]->addBorderingCountry( countries[ALBERTA] );
  countries[ALASKA]->addBorderingCountry( countries[KAMCHATKA] );
  
  countries[NORTHWEST_TERRITORY]->addBorderingCountry( countries[ALASKA] );
  countries[NORTHWEST_TERRITORY]->addBorderingCountry( countries[ALBERTA] );
  countries[NORTHWEST_TERRITORY]->addBorderingCountry( countries[ONTARIO] );
  countries[NORTHWEST_TERRITORY]->addBorderingCountry( countries[GREENLAND] );

  countries[ALBERTA]->addBorderingCountry( countries[ALASKA] );
  countries[ALBERTA]->addBorderingCountry( countries[NORTHWEST_TERRITORY] );
  countries[ALBERTA]->addBorderingCountry( countries[ONTARIO] );
  countries[ALBERTA]->addBorderingCountry( countries[WESTERN_US] );
  
  countries[GREENLAND]->addBorderingCountry( countries[NORTHWEST_TERRITORY] );
  countries[GREENLAND]->addBorderingCountry( countries[ONTARIO] );
  countries[GREENLAND]->addBorderingCountry( countries[QUEBEC] );
  countries[GREENLAND]->addBorderingCountry( countries[ICELAND] );

  countries[ONTARIO]->addBorderingCountry( countries[NORTHWEST_TERRITORY] );
  countries[ONTARIO]->addBorderingCountry( countries[GREENLAND] );
  countries[ONTARIO]->addBorderingCountry( countries[QUEBEC] );
  countries[ONTARIO]->addBorderingCountry( countries[EASTERN_US] );
  countries[ONTARIO]->addBorderingCountry( countries[WESTERN_US] );
  countries[ONTARIO]->addBorderingCountry( countries[ALBERTA] );

  countries[QUEBEC]->addBorderingCountry( countries[EASTERN_US] );
  countries[QUEBEC]->addBorderingCountry( countries[ONTARIO] );
  countries[QUEBEC]->addBorderingCountry( countries[GREENLAND] );

  countries[WESTERN_US]->addBorderingCountry( countries[ALBERTA] );
  countries[WESTERN_US]->addBorderingCountry( countries[ONTARIO] );
  countries[WESTERN_US]->addBorderingCountry( countries[EASTERN_US] );
  countries[WESTERN_US]->addBorderingCountry( countries[CENTRAL_AMERICA] );

  countries[EASTERN_US]->addBorderingCountry( countries[WESTERN_US] );
  countries[EASTERN_US]->addBorderingCountry( countries[CENTRAL_AMERICA] );
  countries[EASTERN_US]->addBorderingCountry( countries[ONTARIO] );
  countries[EASTERN_US]->addBorderingCountry( countries[QUEBEC] );

  countries[CENTRAL_AMERICA]->addBorderingCountry( countries[WESTERN_US] );
  countries[CENTRAL_AMERICA]->addBorderingCountry( countries[EASTERN_US] );
  countries[CENTRAL_AMERICA]->addBorderingCountry( countries[VENEZUELA] );

  countries[VENEZUELA]->addBorderingCountry( countries[CENTRAL_AMERICA] );
  countries[VENEZUELA]->addBorderingCountry( countries[BRAZIL] );
  countries[VENEZUELA]->addBorderingCountry( countries[PERU] );

  countries[BRAZIL]->addBorderingCountry( countries[VENEZUELA] );
  countries[BRAZIL]->addBorderingCountry( countries[PERU] );
  countries[BRAZIL]->addBorderingCountry( countries[ARGENTINA] );
  countries[BRAZIL]->addBorderingCountry( countries[NORTH_AFRICA] );

  countries[PERU]->addBorderingCountry( countries[VENEZUELA] );
  countries[PERU]->addBorderingCountry( countries[BRAZIL] );
  countries[PERU]->addBorderingCountry( countries[ARGENTINA] );

  countries[ARGENTINA]->addBorderingCountry( countries[PERU] );
  countries[ARGENTINA]->addBorderingCountry( countries[BRAZIL] );

  
  
  countries[ICELAND]->addBorderingCountry( countries[GREAT_BRITAIN] );
  countries[ICELAND]->addBorderingCountry( countries[GREENLAND] );
  countries[ICELAND]->addBorderingCountry( countries[SCANDANAVIA] );

  countries[KAMCHATKA]->addBorderingCountry( countries[ALASKA] );
  countries[KAMCHATKA]->addBorderingCountry( countries[JAPAN] );
  countries[KAMCHATKA]->addBorderingCountry( countries[MONGOLIA] );
  countries[KAMCHATKA]->addBorderingCountry( countries[IRKUTSK] );
  countries[KAMCHATKA]->addBorderingCountry( countries[YAKUTSK] );

  countries[JAPAN]->addBorderingCountry( countries[MONGOLIA] );
  countries[JAPAN]->addBorderingCountry( countries[KAMCHATKA] );
  
  countries[CHINA]->addBorderingCountry( countries[SIAM] );
  countries[CHINA]->addBorderingCountry( countries[INDIA] );
  countries[CHINA]->addBorderingCountry( countries[AFGHANISTAN] );
  countries[CHINA]->addBorderingCountry( countries[URAL] );
  countries[CHINA]->addBorderingCountry( countries[SIBERIA] );
  countries[CHINA]->addBorderingCountry( countries[MONGOLIA] );

  countries[MONGOLIA]->addBorderingCountry( countries[JAPAN] );
  countries[MONGOLIA]->addBorderingCountry( countries[CHINA] );
  countries[MONGOLIA]->addBorderingCountry( countries[SIBERIA] );
  countries[MONGOLIA]->addBorderingCountry( countries[IRKUTSK] );
  countries[MONGOLIA]->addBorderingCountry( countries[KAMCHATKA] );

  countries[IRKUTSK]->addBorderingCountry( countries[MONGOLIA] );
  countries[IRKUTSK]->addBorderingCountry( countries[KAMCHATKA] );
  countries[IRKUTSK]->addBorderingCountry( countries[YAKUTSK] );
  countries[IRKUTSK]->addBorderingCountry( countries[SIBERIA] );

  countries[YAKUTSK]->addBorderingCountry( countries[KAMCHATKA] );
  countries[YAKUTSK]->addBorderingCountry( countries[SIBERIA] );
  countries[YAKUTSK]->addBorderingCountry( countries[IRKUTSK] );

  countries[SIBERIA]->addBorderingCountry( countries[URAL] );
  countries[SIBERIA]->addBorderingCountry( countries[CHINA] );
  countries[SIBERIA]->addBorderingCountry( countries[MONGOLIA] );
  countries[SIBERIA]->addBorderingCountry( countries[IRKUTSK] );
  countries[SIBERIA]->addBorderingCountry( countries[YAKUTSK] );

  countries[INDIA]->addBorderingCountry( countries[THE_MIDDLE_EAST] );
  countries[INDIA]->addBorderingCountry( countries[AFGHANISTAN] );
  countries[INDIA]->addBorderingCountry( countries[CHINA] );
  countries[INDIA]->addBorderingCountry( countries[SIAM] );

  countries[THE_MIDDLE_EAST]->addBorderingCountry( countries[EGYPT] );
  countries[THE_MIDDLE_EAST]->addBorderingCountry( countries[SOUTHERN_EUROPE] );
  countries[THE_MIDDLE_EAST]->addBorderingCountry( countries[UKRAINE] );
  countries[THE_MIDDLE_EAST]->addBorderingCountry( countries[AFGHANISTAN] );
  countries[THE_MIDDLE_EAST]->addBorderingCountry( countries[INDIA] );
  countries[THE_MIDDLE_EAST]->addBorderingCountry( countries[EAST_AFRICA] );

  countries[URAL]->addBorderingCountry( countries[UKRAINE] );
  countries[URAL]->addBorderingCountry( countries[AFGHANISTAN] );
  countries[URAL]->addBorderingCountry( countries[CHINA] );
  countries[URAL]->addBorderingCountry( countries[SIBERIA] );

  countries[SIAM]->addBorderingCountry( countries[INDIA] );
  countries[SIAM]->addBorderingCountry( countries[CHINA] );
  countries[SIAM]->addBorderingCountry( countries[INDONESIA] );


  countries[EGYPT]->addBorderingCountry( countries[THE_MIDDLE_EAST] );
  countries[EGYPT]->addBorderingCountry( countries[SOUTHERN_EUROPE] );
  countries[EGYPT]->addBorderingCountry( countries[NORTH_AFRICA] );
  countries[EGYPT]->addBorderingCountry( countries[EAST_AFRICA] );

  countries[EAST_AFRICA]->addBorderingCountry( countries[EGYPT] );
  countries[EAST_AFRICA]->addBorderingCountry( countries[THE_MIDDLE_EAST] );
  countries[EAST_AFRICA]->addBorderingCountry( countries[MADAGASCAR] );
  countries[EAST_AFRICA]->addBorderingCountry( countries[SOUTH_AFRICA] );
  countries[EAST_AFRICA]->addBorderingCountry( countries[CONGO] );
  countries[EAST_AFRICA]->addBorderingCountry( countries[NORTH_AFRICA] );

  countries[MADAGASCAR]->addBorderingCountry( countries[EAST_AFRICA] );
  countries[MADAGASCAR]->addBorderingCountry( countries[SOUTH_AFRICA] );

  countries[SOUTH_AFRICA]->addBorderingCountry( countries[MADAGASCAR] );
  countries[SOUTH_AFRICA]->addBorderingCountry( countries[EAST_AFRICA] );
  countries[SOUTH_AFRICA]->addBorderingCountry( countries[CONGO] );

  countries[CONGO]->addBorderingCountry( countries[NORTH_AFRICA] );
  countries[CONGO]->addBorderingCountry( countries[EAST_AFRICA] );
  countries[CONGO]->addBorderingCountry( countries[SOUTH_AFRICA] );

  countries[NORTH_AFRICA]->addBorderingCountry( countries[BRAZIL] );
  countries[NORTH_AFRICA]->addBorderingCountry( countries[WESTERN_EUROPE] );
  countries[NORTH_AFRICA]->addBorderingCountry( countries[SOUTHERN_EUROPE] );
  countries[NORTH_AFRICA]->addBorderingCountry( countries[EGYPT] );
  countries[NORTH_AFRICA]->addBorderingCountry( countries[EAST_AFRICA] );
  countries[NORTH_AFRICA]->addBorderingCountry( countries[CONGO] );

  countries[INDONESIA]->addBorderingCountry( countries[SIAM] );
  countries[INDONESIA]->addBorderingCountry( countries[NEW_GUINEA] );
  countries[INDONESIA]->addBorderingCountry( countries[WESTERN_AUSTRALIA] );

  countries[NEW_GUINEA]->addBorderingCountry( countries[INDONESIA] );
  countries[NEW_GUINEA]->addBorderingCountry( countries[WESTERN_AUSTRALIA] );
  countries[NEW_GUINEA]->addBorderingCountry( countries[EASTERN_AUSTRALIA] );

  countries[WESTERN_AUSTRALIA]->addBorderingCountry( countries[INDONESIA] );
  countries[WESTERN_AUSTRALIA]->addBorderingCountry( countries[NEW_GUINEA] );
  countries[WESTERN_AUSTRALIA]->addBorderingCountry( countries[EASTERN_AUSTRALIA] );

  countries[EASTERN_AUSTRALIA]->addBorderingCountry( countries[NEW_GUINEA] );
  countries[EASTERN_AUSTRALIA]->addBorderingCountry( countries[WESTERN_AUSTRALIA] );

  countries[GREAT_BRITAIN]->addBorderingCountry( countries[ICELAND] );
  countries[GREAT_BRITAIN]->addBorderingCountry( countries[SCANDANAVIA] );
  countries[GREAT_BRITAIN]->addBorderingCountry( countries[NORTHERN_EUROPE] );
  countries[GREAT_BRITAIN]->addBorderingCountry( countries[WESTERN_EUROPE] );
  
  countries[SCANDANAVIA]->addBorderingCountry( countries[ICELAND] );
  countries[SCANDANAVIA]->addBorderingCountry( countries[GREAT_BRITAIN] );
  countries[SCANDANAVIA]->addBorderingCountry( countries[UKRAINE] );
  countries[SCANDANAVIA]->addBorderingCountry( countries[NORTHERN_EUROPE] );

  countries[UKRAINE]->addBorderingCountry( countries[SCANDANAVIA] );
  countries[UKRAINE]->addBorderingCountry( countries[NORTHERN_EUROPE] );
  countries[UKRAINE]->addBorderingCountry( countries[SOUTHERN_EUROPE] );
  countries[UKRAINE]->addBorderingCountry( countries[THE_MIDDLE_EAST] );
  countries[UKRAINE]->addBorderingCountry( countries[AFGHANISTAN] );
  countries[UKRAINE]->addBorderingCountry( countries[URAL] );

  countries[NORTHERN_EUROPE]->addBorderingCountry( countries[SCANDANAVIA] );
  countries[NORTHERN_EUROPE]->addBorderingCountry( countries[GREAT_BRITAIN] );
  countries[NORTHERN_EUROPE]->addBorderingCountry( countries[WESTERN_EUROPE] );
  countries[NORTHERN_EUROPE]->addBorderingCountry( countries[SOUTHERN_EUROPE] );
  countries[NORTHERN_EUROPE]->addBorderingCountry( countries[UKRAINE] );


  countries[SOUTHERN_EUROPE]->addBorderingCountry( countries[THE_MIDDLE_EAST] );
  countries[SOUTHERN_EUROPE]->addBorderingCountry( countries[UKRAINE] );
  countries[SOUTHERN_EUROPE]->addBorderingCountry( countries[NORTHERN_EUROPE] );
  countries[SOUTHERN_EUROPE]->addBorderingCountry( countries[WESTERN_EUROPE] );
  countries[SOUTHERN_EUROPE]->addBorderingCountry( countries[NORTH_AFRICA] );
  countries[SOUTHERN_EUROPE]->addBorderingCountry( countries[EGYPT] );

  countries[WESTERN_EUROPE]->addBorderingCountry( countries[GREAT_BRITAIN] );
  countries[WESTERN_EUROPE]->addBorderingCountry( countries[NORTHERN_EUROPE] );
  countries[WESTERN_EUROPE]->addBorderingCountry( countries[SOUTHERN_EUROPE] );
  countries[WESTERN_EUROPE]->addBorderingCountry( countries[NORTH_AFRICA] );

  //countries[]->addBorderingCountry( countries[] );
  //countries[]->addBorderingCountry( countries[] );
  //countries[]->addBorderingCountry( countries[] );


  displayCountries();
  

  while(1)
    {
      // iterate through everyone's turn
      for( int whosTurn=0; whosTurn < players.size(); whosTurn++ )
	{
	  bool still_their_turn = true;
	  while( still_their_turn )
	    {
	      cout << endl << "*** Player: " << *players[whosTurn] << "'s turn ***" << endl;

	      // create a vector of possible starting points
	      vector<Country*> possible_countries_to_start_from;
	      vector<Country*> possible_countries_to_attack;
	      
	      for( int i = 0; i < countries.size(); i++ )
		{
		  if( countries[i]->getPlayer() == whosTurn && countries[i]->getNumber()>1 )
		    {
		      possible_countries_to_start_from.push_back( countries[i] );
		    }
		}

	      cout << endl << "*** Choose where your attack will originate from ***" << endl;
	      // Display the vector of possible starting countries
	      for( int i = 0; i < possible_countries_to_start_from.size(); i++ )
		{
		  cout << "(" << i << ") " << *players[whosTurn] << " can start from " << *possible_countries_to_start_from[i] << endl;
		}

	      cout << "(-1) to end turn" << endl;
	      // Delay
	      int x;
	      cin >> x;
	      
	      switch( x )
		{
		case -1:
		  still_their_turn = false;
		  break;
		default:
		  // build list of possible countries to attack
		  Country * c = possible_countries_to_start_from[x];
		  Country * d;
		  cout << " you chose " << *c << endl;
		  if( c->getNumber() < 2 )
		    {
		      cout << "there are not enough troops there to attack anyone... sorry" << endl;
		    }
		  else
		    {
		      // create a vector of countries that can be attacked legally
		      for( int j = 0; j < c->bordering_country.size(); j++ )
			{
			  // add them to the vector
			  if( c->bordering_country[j]->getPlayer() != whosTurn )
			    {
			      possible_countries_to_attack.push_back( c->bordering_country[j] );
			    }
			}
		      cout << endl << "*** Choose where your attack will take place ***" << endl;
		      // display the vector
		      for( int j = 0; j < possible_countries_to_attack.size(); j++ )
			{
			  cout << "(" << j << ") " << *possible_countries_to_attack[j] << endl;
			}

		      cout << "(-1) to exit" << endl;
		      int y;
		      cin >> y;

		      switch(y)
			{
			case -1:
			  break;
			default:
			  d = possible_countries_to_attack[y];
			  break;
			}
		      
		      cout << endl << "*** you chose to attack " << *d << " from " << *c << " ***" << endl;

		      // the attack will now take place
		      cout << "how many troops do you want to use?" <<endl;
		      int attacker_troops_total = 0;
		      int defender_troops_total = d->getNumber();

		      cin >> attacker_troops_total;
		    cerr << "ATTACKING WITH " << attacker_troops_total << " troops" << endl;
		    cerr << "DEFENDING WITH " << defender_troops_total << " troops" << endl;

		    // remove the number of attacking troops from the originating country
		    c->setNumber( c->getNumber() - attacker_troops_total );

		      
		    bool no_one_has_won_yet = true;
		    while( no_one_has_won_yet )
		      {
			  
			while( attacker_troops_total > 0 && defender_troops_total > 0 )
			  {

			    /* ATTACK ATTACK ATTACK ATTACK ATTACK ATTACK ATTACK ATTACK */
			    if( attacker_troops_total >= 3 )
			      {
				// attacker rolls thrice
				attacker_dice.push_back( die() );
				attacker_dice.push_back( die() );
				attacker_dice.push_back( die() );
			      }
			    else if( attacker_troops_total == 2 )
			      {
				attacker_dice.push_back( die() );
				attacker_dice.push_back( die() );
			      }
			    else
			      {
				attacker_dice.push_back( die() );
			      }
			    sort(attacker_dice.begin(), attacker_dice.end(), greater<int>());

			    if( defender_troops_total >= 2 )
			      {
				defender_dice.push_back( die() );
				defender_dice.push_back( die() );
			      }
			    else
			      {
				defender_dice.push_back( die() );
			      }
			    sort(defender_dice.begin(), defender_dice.end(), greater<int>());
			      


			      
			    // output both sets of dice rolls
			    cerr << "*** attacker rolled: ";
			    for( int k = 0; k< attacker_dice.size(); k++ )
			      {
				cerr << attacker_dice[k] << " ";
			      }
			    cerr << " ***" << endl;
			      
			    cerr << "*** defender rolled: ";
			    for( int k = 0; k< defender_dice.size(); k++ )
			      {
				cerr << defender_dice[k] << " ";
			      }
			    cerr << " ***" << endl;


			    /*   compare the dice   */
			    if( attacker_dice[0] > defender_dice[0] )
			      {
				defender_troops_total--;
			      }
			    else
			      {
				attacker_troops_total--;
			      }
			      
			    if( attacker_dice.size() > 1 && defender_dice.size() > 1 )
			      {
				if( attacker_dice[1] > defender_dice[1] )
				  {
				    defender_troops_total--;
				  }
				else
				  {
				    attacker_troops_total--;
				  }
				  
			      }

			    attacker_dice.erase(attacker_dice.begin(), attacker_dice.end());
			    defender_dice.erase(defender_dice.begin(), defender_dice.end());
			    //possible_countries_to_attack.erase(possible_countries_to_attack.begin(),possible_countries_to_attack.begin());

			  }

			if( attacker_troops_total == 0 )
			  {
			    no_one_has_won_yet = false;
			    d->setNumber( defender_troops_total );
			    c->bordering_country[y]->setNumber( defender_troops_total );

			    cout << "*** " << *d << " staved off the attack from " << *c << " ***" << endl;
			  }
			if( defender_troops_total == 0 )
			  {
			    no_one_has_won_yet = false;
			    c->bordering_country[y]->setPlayer( whosTurn );
			    d->setPlayer( whosTurn );
			    c->bordering_country[y]->setNumber( attacker_troops_total );
			    d->setNumber( attacker_troops_total );
			    cout << "*** " << *c << " wins and now owns " << *d << " ***" << endl;
			  }
			//cin >> y; // pause
			possible_countries_to_attack.erase(possible_countries_to_attack.begin(),possible_countries_to_attack.begin());
		      }

		    }
		
		//	break;
		
		}
	    }
	}
	// calculate additions
	// player 1
	int ply[3];
	for( int i=0; i<countries.size(); i++)
	  {
	    ply[countries[i]->getPlayer()]++;
	  }
	for( int i=0; i<players.size(); i++ )
	  {
	    
	    cout << "*** Player " << i << " gets " << ply[i]/3 << " more troops" << endl;
	  }
	cout << "*** DISTRIBUTE TROOPS ***" << endl;
	for( int i=0; i<players.size(); i++ )
	  {
	    while( ply[i]/3 > 0 )
	      {
		displayPlayersCountries(i);
		cout << "*** You have " << ply[i]/3 << " troops left to distribute ***" << endl;
		cout << "*** Where would you like to place one? ***" << endl;
		int z = 0;
		cin >> z;
		countries[z]->addTroops(1);
		ply[i]-=3;
	      }
	  }
	// TROOP MOVEMENTS
	
	for( int i=0; i<players.size(); i++ )
	  {
	    /* ==================================== */
	    int input = 0;
	    while( input != -1 )
	      {
		cout << endl << "*** PLAYER " << i << " TROOP MOVEMENTS ***" << endl;
		displayPlayersFriendlyCountries(i);
		cout << "*** move from (-1 to end): ***" << endl;
		cin >> input;
		if( input == -1 )
		  {
		    cerr << "*** e x i t ***" << endl;
		  }
		else if( countries[input]->getNumber() < 2)
		  {
		    cerr << "*** not enough troops to move ***" << endl;
		  }
		else 
		  {
		    for( int ii=0; ii<countries[input]->bordering_country.size(); ii++ )
		      {
			if( countries[input]->bordering_country[ii]->getPlayer() == i)
			  cout << "(" << ii << ") " << *countries[input]->bordering_country[ii] << endl;			
		      }
		    int input2;
		    cout << "to where? (-1 to cancel)" << endl;
		    cin >> input2;
		    if( input2 != -1 )
		      {
			countries[input]->troopReduction();
			countries[input]->bordering_country[input2]->addTroops(1);
		      }
		  }
	      }
	    /* ==================================== */
	  }
	    
    }
  
 deleteAll();
  
 return 0;
}
