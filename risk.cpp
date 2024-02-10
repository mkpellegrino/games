#include <iostream>
#include <vector>
#include <string>
#include <algorithm>    // std::sort
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iomanip>

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
    id = c->id;
    //name = c->name;
    random_number = c->random_number;
    number_of_troops = c->number_of_troops;
    player = c->player;
  }
  
  Country(string * s, int id_for_vector, int n = 0, int p = 0)
  {
    //id=country_index++;
    id=id_for_vector;
    name = s;
    number_of_troops = n;
    player = p;
    random_number = (rand() % 42);
  };
  ~Country();
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
    bordering_country.push_back( c );
  };

  friend ostream& operator <<(ostream& out, const Country& c);
  // for sorting purposes
  bool operator< (const Country &other) const
  {
    return id < other.id;
  }
  int getNumberOfFriendlyNeighbors();
  int getNumberOfHostileNeighbors();
  vector <Country*> getFriendlyNeighbors();
  vector <Country*> getHostileNeighbors();
  vector <Country*> getAllNeighbors();
private:
  vector<Country*> bordering_country;
  int id;
  string * name;
  int number_of_troops;
  int player;
  int random_number;
};

Country::~Country()
{
  delete name;
  id=0;
  number_of_troops = 0;
};

int Country::getNumberOfFriendlyNeighbors()
{
  int return_value=0;
  for( int i=0; i<bordering_country.size(); i++ )
    {
      if( bordering_country[i]->getPlayer() == this->getPlayer() )
	{
	  return_value++;
	}
    }
  return return_value;
}

int Country::getNumberOfHostileNeighbors()
{
  int return_value=0;
  for( int i=0; i<bordering_country.size(); i++ )
    {
      if( bordering_country[i]->getPlayer() != this->getPlayer() )
	{
	  return_value++;
	}
    }
  return return_value;
}

vector<Country*> Country::getAllNeighbors()
{
  return bordering_country;
}

vector<Country*> Country::getFriendlyNeighbors()
{
  vector <Country*> return_vector;
  for( int i=0; i<bordering_country.size(); i++ )
    {
      if( bordering_country[i]->getPlayer() == this->getPlayer() )
	{
	  return_vector.push_back( bordering_country[i] );
	}
    }
  return return_vector;
}

vector<Country*> Country::getHostileNeighbors()
{
  vector <Country*> return_vector;
  for( int i=0; i<bordering_country.size(); i++ )
    {
      if( bordering_country[i]->getPlayer() != this->getPlayer() )
	{
	  return_vector.push_back( bordering_country[i] );
	}
    }
  return return_vector;
}

ostream& operator <<(ostream& out, const Country& c)
{
  out << *c.name << " " << "P" << c.player << " T" << c.number_of_troops;
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


class Continent
{
public:
  Continent(){number_of_countries=0;};
  ~Continent();
  void addCountry( Country * c );
  
private:
  vector<Country*> my_countries;
  int number_of_countries;
};

void Continent::addCountry( Country * c )
{
  my_countries.push_back( c );
  number_of_countries = my_countries.size();
  return;
}


vector<Country*> countries;

void displayPlayers()
{
  for( int i = 0; i < players.size(); i++ )
    {
      cout << *players[i] << endl << endl;
    }
  return;
}

vector<Country*> getPlayersCountriesThatHaveFriendlyNeighbors( int p )
{
  vector<Country*> return_vector;
  for( int i=0; i<countries.size(); i++ )
    {
      if( countries[i]->getPlayer() == p && countries[i]->getNumberOfFriendlyNeighbors()>0)
	{
	  return_vector.push_back( countries[i] );
	}
    }
  return return_vector;
}

vector<Country*> getPlayersCountriesThatHaveHostileNeighbors( int p )
{
  vector<Country*> return_vector;
  for( int i=0; i<countries.size(); i++ )
    {
      if( countries[i]->getPlayer() == p && countries[i]->getNumberOfHostileNeighbors()>0)
	{
	  return_vector.push_back( countries[i] );
	}
    }
  return return_vector;
}

vector<Country*> getPlayersCountriesThatHaveHostileNeighborsWithMoreThanOneTroop( int p )
{
  vector<Country*> return_vector;
  for( int i=0; i<countries.size(); i++ )
    {
      if( countries[i]->getNumber() > 1 && countries[i]->getPlayer() == p && countries[i]->getNumberOfHostileNeighbors()>0)
	{
	  return_vector.push_back( countries[i] );
	}
    }
  return return_vector;
}

vector<Country*> getPlayersCountriesThatHaveFriendlyNeighborsWithMoreThanOneTroop( int p )
{
  vector<Country*> return_vector;
  for( int i=0; i<countries.size(); i++ )
    {
      if( countries[i]->getNumber() > 1 && countries[i]->getPlayer() == p && countries[i]->getNumberOfFriendlyNeighbors()>0)
	{
	  return_vector.push_back( countries[i] );
	}
    }
  return return_vector;
}

vector<Country*> getPlayersCountries( int p )
{
  vector<Country*> return_vector;
  for( int i=0; i<countries.size(); i++ )
    {
      if( countries[i]->getPlayer() == p )
	{
	  return_vector.push_back( countries[i] );
	}
    }
  return return_vector;
}

int getNumberOfPlayersCountries( int p )
{
  return getPlayersCountries(p).size();
}

void displayCountriesHostileNeighbors( Country * c )
{
  vector<Country*> v = c->getHostileNeighbors();
  for( int i = 0; i < v.size(); i++ )
    {
      cout << "(" << i << ") " << *v[i] << endl;
    }
  return;  
}

void displayCountriesFriendlyNeighbors( Country * c )
{
  vector<Country*> v = c->getFriendlyNeighbors();
  for( int i = 0; i < v.size(); i++ )
    {
      cout << "(" << i << ") " << *v[i] << endl;
    }
  return;  
}

void displayCountriesNeighbors( Country * c )
{
  vector<Country*> v = c->getAllNeighbors();
  for( int i = 0; i < v.size(); i++ )
    {
      cout << "(" << i << ") " << *v[i] << endl;
    }
  return;  
}

void displayPlayersCountries( int p )
{
  vector<Country*> v = getPlayersCountries(p);
  
  for( int i = 0; i < v.size(); i++ )
    {
      cout << "(" << i << ") " << *v[i] << endl;
    }
  return;
}

void displayAllCountries()
{
  int rows=0;
  for( int i = 0; i < countries.size(); i++ )
    {
      cout << "(" << right << setfill('0') << setw(2) << i << ") " << left << setfill(' ') << setw(13) << *countries[i] << "\t";
      if( rows++ == 4 )
	{
	  cout << endl;
	  rows=0;
	}      
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

void m( int n )
{
  cout << "[" << *countries[n] << "]";
}

void l()
{
  cout << "---";
}

void drawMap()
{
 cout << "\033[H ______ ";
cout << "\033[#E/      \\";
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
  countries.push_back( new Country( new string("NW Terr"), NORTHWEST_TERRITORY, 1, 0));
  countries.push_back( new Country( new string("Alberta"), ALBERTA, 1, 0));  
  countries.push_back( new Country( new string("Greenlnd"), GREENLAND, 1, 0));
  countries.push_back( new Country( new string("Ontario"), ONTARIO, 1, 0));
  countries.push_back( new Country( new string("Quebec"), QUEBEC, 1, 1));
  countries.push_back( new Country( new string("W. U.S."), WESTERN_US, 1, 0));
  countries.push_back( new Country( new string("E. U.S."), EASTERN_US, 1, 0));
  countries.push_back( new Country( new string("Ctrl Amer"), CENTRAL_AMERICA, 1, 0));
  countries.push_back( new Country( new string("Iceland"), ICELAND, 1, 0));

  countries.push_back( new Country( new string("Venez"), VENEZUELA, 1, 0 ));
  countries.push_back( new Country( new string("Peru"), PERU, 1, 0 ));
  countries.push_back( new Country( new string("Argntna"), ARGENTINA, 1, 0 ));
  countries.push_back( new Country( new string("Brazil"), BRAZIL, 1, 0 ));

  countries.push_back( new Country( new string("Britain"), GREAT_BRITAIN, 1, 0));
  countries.push_back( new Country( new string("Scandnvia"), SCANDANAVIA, 1, 0 ));
  countries.push_back( new Country( new string("N. Europe"), NORTHERN_EUROPE, 1, 0 ));
  countries.push_back( new Country( new string("W. Europe"), WESTERN_EUROPE, 1, 0 ));
  countries.push_back( new Country( new string("S. Europe"), SOUTHERN_EUROPE, 1, 0 ));
  countries.push_back( new Country( new string("Ukraine"), UKRAINE, 1, 0 ));
  
  countries.push_back( new Country( new string("N. Africa"), NORTH_AFRICA, 1, 0 ));
  countries.push_back( new Country( new string("E. Africa"), EAST_AFRICA, 1, 0 ));
  countries.push_back( new Country( new string("Egypt"), EGYPT, 1, 0 ));
  countries.push_back( new Country( new string("Congo"), CONGO, 1, 0 ));
  countries.push_back( new Country( new string("S. Africa"), SOUTH_AFRICA, 1, 0 ));
  countries.push_back( new Country( new string("Madgscar"), MADAGASCAR, 1, 0 ));
  
  countries.push_back( new Country( new string("New Guinea"), NEW_GUINEA, 1, 0 ));
  countries.push_back( new Country( new string("Indnsia"), INDONESIA, 1, 0 ));
  countries.push_back( new Country( new string("W Austr"), WESTERN_AUSTRALIA, 1, 0 ));
  countries.push_back( new Country( new string("E Austr"), EASTERN_AUSTRALIA, 1, 0 ));

  countries.push_back( new Country( new string("Kamchatka"), KAMCHATKA, 1, 0));
  countries.push_back( new Country( new string("Ural"), URAL, 1, 0 ));
  countries.push_back( new Country( new string("Siberia"), SIBERIA, 1, 0 ));
  countries.push_back( new Country( new string("Yakutsk"), YAKUTSK, 1, 0 ));
  countries.push_back( new Country( new string("Irkutsk"), IRKUTSK, 1, 0 ));
  countries.push_back( new Country( new string("Japan"), JAPAN, 1, 0 ));
  countries.push_back( new Country( new string("Mongolia"), MONGOLIA, 1, 0 ));
  countries.push_back( new Country( new string("Afghan"), AFGHANISTAN, 1, 0 ));
  countries.push_back( new Country( new string("Mid East"), THE_MIDDLE_EAST, 1, 0 ));
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
  
  //drawMap();

  //return -1;
  bool gotValidInput=false;
  while(1)
    {
      // iterate through everyone's turn
      displayAllCountries();
      for( int whosTurn=0; whosTurn < players.size(); whosTurn++ )
	{
	  bool still_their_turn = true;
	  while( still_their_turn )
	    {
	      cout << endl << "*** Player: " << *players[whosTurn] << "'s turn ***" << endl;

	      // create a vector of possible starting points
	      vector<Country*> possible_countries_to_start_from = getPlayersCountriesThatHaveHostileNeighborsWithMoreThanOneTroop(whosTurn);
	      vector<Country*> possible_countries_to_attack;
	      
	      cout << endl << "*** Choose where your attack will originate from ***" << endl;
	      
	      // Display the vector of possible starting countries
	      for( int i = 0; i < possible_countries_to_start_from.size(); i++ )
		{
		  cout << "(" << i << ") " << *possible_countries_to_start_from[i] << endl;
		}

	      cout << "(-1) to end turn" << endl;
	      gotValidInput=false;
	      int x=0;
	      while(!gotValidInput)
		{
		  cin >> x;
		  if( x >= -1 && x < (int)possible_countries_to_start_from.size())
		    {
		      gotValidInput=1;
		    }
		}
	      Country * c;
	      Country * d;

	      switch( x )
		{
		case -1:
		  still_their_turn = false;
		  break;
		  
		case -2:
		  break;
		  
		default:
		  c =  possible_countries_to_start_from[x];
		  cout << "*** you chose " << *c << " ***" << endl;
		  // build list of possible countries to attack
		  //{
		    vector<Country*> list_of_hostile_neighbors = c->getHostileNeighbors();
		    displayCountriesHostileNeighbors(c);		      
		    cout << endl << "*** Choose where your attack will take place ***" << endl;
		    cout << "(-1) to exit" << endl;
		    gotValidInput=false;
		    int y;
		    while( !gotValidInput )
		      {
			cin >> y;
			if( y>=-1 && y< (int)list_of_hostile_neighbors.size() )
			  {
			    gotValidInput=true;
			  }
		      }

		    switch(y)
		      {
		      case -1:
			break;
		      default:
			d = list_of_hostile_neighbors[y];
			break;
		      }
		      
		    cout << endl << "*** you chose to attack " << *d << " from " << *c << " ***" << endl;

		    // the attack will now take place
		    cout << "how many troops do you want to use?" <<endl;
		    int attacker_troops_total = 0;
		    int defender_troops_total = d->getNumber();

		    gotValidInput=false;
		    while( !gotValidInput )
		      {
			cin >> attacker_troops_total;
			if( attacker_troops_total>0 && attacker_troops_total<(int)(c->getNumber()) )
			  {
			    gotValidInput=true;
			  }
			else
			  {
			    cout << "invalid input " << c->getNumber() << endl;
			  }
		      }

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

#ifdef DEBUG			    
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
#endif

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

			}

		      if( attacker_troops_total == 0 )
			{
			  no_one_has_won_yet = false;
			  d->setNumber( defender_troops_total );
			  cout << "*** " << *d << " staved off the attack from " << *c << " ***" << endl;
			}
		      if( defender_troops_total == 0 )
			{
			  no_one_has_won_yet = false;
			  d->setPlayer( whosTurn );
			  d->setNumber( attacker_troops_total );
			  cout << "*** " << *c << " wins and now owns " << *d << " ***" << endl;
			}
		      possible_countries_to_attack.erase(possible_countries_to_attack.begin(),possible_countries_to_attack.begin());
		      //}
		  }
		}
	    }
	}

    
    // TROOP MOVEMENTS
	
    cout << "* * * * * T R O O P   M O V E M E N T S * * *" << endl;
    for( int i=0; i<players.size(); i++ )
      {
	/* ==================================== */
	int input = 0;
	while( input != -1 )
	  {
	    cout << endl << endl << "*** PLAYER: " << *players[i] << " TROOP MOVEMENTS ***" << endl;
	    vector<Country*> player_countries = getPlayersCountriesThatHaveFriendlyNeighborsWithMoreThanOneTroop(i);
	    for( int i=0; i<player_countries.size(); i++ )
	      {
		cout << "(" << i << ") " << *player_countries[i] << endl;
	      }
	    cout << "*** move from (-1 to end): ***" << endl;
	    gotValidInput=false;
	    while( !gotValidInput )
		{
		  cin >> input;
		  if( input>=-1 && input<(int)player_countries.size() )
		    {
		      gotValidInput=true;
		    }
		}

	    if( input == -1 )
	      {
		cerr << endl << "*** end turn ***" << endl;
	      }
	    else
	      {
		Country * starting_from = player_countries[input];

		cout << "   move to? (-1 to cancel)" << endl;

		vector<Country*> friendlies = starting_from->getFriendlyNeighbors();

		displayCountriesFriendlyNeighbors(starting_from);
		gotValidInput=false;
		int input2;
		while( !gotValidInput )
		{
		  cin >> input2;
		  if( input2>=-1 && input2<(int)player_countries.size() )
		    {
		      gotValidInput=true;
		    }
		}

		friendlies[input2]->addTroops(1);
		starting_from->troopReduction();
	      }
	  }
	/* ==================================== */
      }

    // calculate additions

    cout << "*** DISTRIBUTE TROOPS ***" << endl;

    for( int i=0; i<players.size(); i++ )
      {
	int additional_troops = getNumberOfPlayersCountries(i)/3;
	cout << "*** PLAYER: " << *players[i] << " has " << additional_troops << " troops that need to be placed ***" << endl;
	for( int j = 0; j < additional_troops; j++ )
	  {
	    displayPlayersCountries(i);
	    vector<Country*> v = getPlayersCountries(i);
	    cout << ">> where would you like to place one?" << endl;
	    gotValidInput = false;
	    while( !gotValidInput )
	      {
		int input3;
		cin >> input3;

		if( input3 >= 0 && input3 < (int)v.size() )
		  {
		    v[input3]->addTroops(1);
		    gotValidInput = true;
		  }
		else
		  {
		    cout << "*** invalid input ***" << endl;
		  }
	      }
	    
	    
	  }
      }

	    
    }
 
  
 deleteAll();
  
 return 0;
}
