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
  
bool land_grab_phase = false;
bool troop_placement_phase = false;
bool troop_movement_phase = false;
bool attack_phase = false;

#ifdef DEBUG
int db_integer=0;
#endif

using namespace std;

vector<int> attacker_dice;
vector<int> defender_dice;

// forward declarations
class Country;
class Region;
void vectorToMenu(vector<Country*> v);
void displayMap();
int getNumberOfPlayersCountries( int p );

class Player
{
public:
  Player( string * s = new string("no_name"), int o = 0, int t = 0, int strat = 0)
  {
#ifdef DEBUG
    cerr << "creating player " << *s << endl;
#endif
    order = o;
    name = s;
    strategy = strat;
    unplaced_troops = t;
    ai=false;
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

  void setUnplacedTroops(int i){ unplaced_troops = i;};
  int getUnplacedTroops(){ return unplaced_troops; };
  void setAI(bool b)
  {
    ai=b;
  }
  bool getAI()
  {
    return ai;
  }

  int chooseInitialPlacement(vector<Country*> v);
  int chooseCountryToMoveTroopsFrom(vector<Country*> v);
  int chooseCountryToMoveTroopsTo(vector<Country*> v);
  void setStrategy( int s ){ strategy = s; };

  int moveTroops();
private:
  string * name;

  // Strategies
  // 1 -
  //    Agressive (battle whenever possible)
  //    Try to get countries that have many connecting links
  int strategy;
  int unplaced_troops;
  int order;
  bool ai;
};

ostream& operator <<(ostream& out, const Player& p)
{
  out << "\e[1;" << to_string(p.order+33)<< "m";
  out << *p.name;
  out << "\e[38;5;40m";
  
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
    x=c->x;
    y=c->y;
    short_name = c->short_name;
    //name = c->name;
    random_number = c->random_number;
    number_of_troops = c->number_of_troops;
    player = c->player;
  }
  
  Country(string * s, int id_for_vector, int X, int Y, string * sn, int n = 0, int p = -1)
  {
    //id=country_index++;
    x=X;
    y=Y;
    short_name = sn;
    id=id_for_vector;
    name = s;
    number_of_troops = n;
    player = p;
    random_number = (rand() % 42);
  };
  ~Country();
  string getName(){ return *name; };
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
  int getNumberOfNeighbors(){return bordering_country.size();}
  bool isIsolated()
  {
    bool return_value=false;
    if( getNumberOfHostileNeighbors() == 0 )
      {
	return_value=true;
      }
    return return_value;
  }

  bool hasMoveableTroops()
  {
    bool return_value = false;
    if( number_of_troops > 0 )
      {
	return_value = true;
      }
    return return_value;
  }
  
  void setX( int newx )
  {
    x = newx;
  }
  void setY( int newy )
  {
    y = newy;
  }

  void setXY( int newx, int newy )
  {
    x=newx;
    y=newy;
  }

  void alreadyMovedFrom( bool b ) { already_moved_from = b; };
  bool alreadyMovedFrom(){ return already_moved_from; };

  void map_print()
  {
    cout << "\e[1;" << to_string(player+33)<< "m";    
    cout << "\e[" << x*4+2 << ";" << y*15+7 << "H" << *short_name << "\e[" << x*4+3 << ";" << y*15+9 << "H" << number_of_troops;
    cout << "\e[38;5;40m";

  }
  
  vector <Country*> getFriendlyNeighbors();
  vector <Country*> getHostileNeighbors();
  vector <Country*> getAllNeighbors();

  friend bool operator==(const Country& lhs, const Country& rhs) { return(lhs.id == rhs.id); }
private:
  bool already_moved_from;
  vector<Country*> bordering_country;
  int id;
  string * name;
  int number_of_troops;
  int player;
  int random_number;
  int x;
  int y;
  string * short_name;
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
      if( bordering_country[i]->getPlayer() == player)
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
      if( bordering_country[i]->getPlayer() != player )
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
  out << "\e[1;" << to_string(c.player+33)<< "m";
  out << setfill(' ') << left << setw(23) << *c.name
      << setw(1);
    if( c.player!=-1)
      {
	out << " P" << c.player;
	out << " T" << c.number_of_troops;
      }
  out << "\e[38;5;40m";
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


int Player::chooseCountryToMoveTroopsFrom(vector<Country*> v)
{
  int return_value=-1;
  // get a list of non-isolated countries that can be gotten to from this one
  vector<int> isolated_countries;
  for( int i=0; i<v.size(); i++ )
    {
      if( v[i]->isIsolated() )
	{
	  isolated_countries.push_back(i);
	}
    }
  if( isolated_countries.size() > 0 )
    {
      return_value = rand() % isolated_countries.size();
      v[return_value]->alreadyMovedFrom(true);
    }
  return return_value;
}

int Player::chooseCountryToMoveTroopsTo(vector<Country*> v)
{
  int return_value=-1;
  
  // get a list of non-isolated countries that can be gotten to from this one
  vector<int> non_isolated_countries;
  for( int i=0; i<v.size(); i++ )
    {
      if( !v[i]->isIsolated()  )
	{
	  non_isolated_countries.push_back(i);
	}
    }
  if( non_isolated_countries.size() > 0 )
    {
      return_value = non_isolated_countries[rand() % non_isolated_countries.size()];
    }
  else
    {
      return_value = rand() % v.size();
    }
  //cout << "[" << return_value << "] choose country to move troops to" << endl;
  //cin >> return_value;
  
  return return_value;
}

int Player::chooseInitialPlacement(vector<Country*> v)
{
  int return_value=rand() % v.size(); // default is randomness
  
  /* another way to do this is to get a list of all the countries that */
  /* are owneed by -1 and get the one with the most amount of links */
  if( land_grab_phase )
    {
      if( strategy == 1 ) /* find countries that have the most connections */
	{
	  return_value=0;
	  int max_value = 0;
	  for( int i=0; i<v.size(); i++ )
	    {
	      if( v[i]->getNumberOfNeighbors() > max_value)
		{
		  max_value = v[i]->getNumberOfNeighbors();
		  return_value = i;
		}
	    }
	}
      else if( strategy == 2 ) /* find countries near countries that the player has */
	{
	  // search through list of possible countries
	  // for one that has this player as a bordering country
	  for( int i=0; i<v.size(); i++ )
	    {
	      vector<Country*> b = v[i]->getAllNeighbors();
	      for( int j = 0; j< b.size(); j++ )
		{
		  if( b[j]->getPlayer() == order )
		    {
		      // we've found one!
		      return_value = i;
		      j=b.size();
		      i=v.size();
		    }
		}
	    }
	}
      else /* random */
	{
	  return_value = rand() % v.size();
	}
    }
  else if( troop_placement_phase )
    {
      if( strategy == 1 )
	{
	  // put most troops in a well connected territory
	  int rv = rand() % 100;
	  if( rv > 25 )
	    {
	      // create a vector of the most connected territories
	      vector<int> most_connected_territories;
	      // find the max # of connections
	      int max_value=0;
	      for( int i=0; i<v.size(); i++ )
		{
		  if( v[i]->getNumberOfNeighbors() > max_value)
		    {
		      max_value = v[i]->getNumberOfNeighbors();
		    }
		}
	      // reduce it by 1
	      max_value--;
	      for( int i=0; i<v.size(); i++ )
		{
		  if( v[i]->getNumberOfNeighbors() >= max_value)
		    {
		      most_connected_territories.push_back(i);
		    }
		}

	      // pick one at random
	      int rv2 = rand() % most_connected_territories.size();

	      // return its number
	      return_value = most_connected_territories[rv2];
	    }
	  else
	    {
	      // create a vector of the most connected territories
	      vector<int> least_connected_territories;
	      // find the max # of connections
	      int max_value=0;
	      for( int i=0; i<v.size(); i++ )
		{
		  if( v[i]->getNumberOfNeighbors() > max_value)
		    {
		      max_value = v[i]->getNumberOfNeighbors();
		    }
		}
	      for( int i=0; i<v.size(); i++ )
		{
		  if( v[i]->getNumberOfNeighbors() < max_value)
		    {
		      least_connected_territories.push_back(i);
		    }
		}

	      // pick one at random
	      int rv2 = rand() % least_connected_territories.size();

	      // return its number
	      return_value = least_connected_territories[rv2];
	    }
	}
      else if( strategy == 2 )
	{
	  // this is random for now
	  // evenly distribute the troops
	  return_value=rand() % v.size();
	}
      else
	{
	  // randomly distribute the troops
	  return_value=rand() % v.size();
	}
    }
  return return_value;
}

class ValidMove
{
public:
  ValidMove(Country * s, Country * d)
  {
#ifdef DEBUG
    cout << "creating ValidMove " << s->getName() << " and " << d->getName() << endl;
    cin >> db_integer;
#endif
    source = s; destination = d;
  };
  ~ValidMove();
  bool same( string s, string d )
  {
#ifdef DEBUG
    cout << "s: " << s << endl
	 << "d: " << d << endl
	 << "source: " << source->getName() << endl
	 << "destination: " << destination->getName() << endl;

    cin >> db_integer;
#endif
    bool return_value = false;
    if( (s == source->getName() && d == destination->getName()) || ( s == destination->getName() && d == source->getName() ))
      {
#ifdef DEBUG
	cout << "match found" << endl;
	cin >> db_integer;
#endif
	return_value = true;
      }
    return return_value;
  };
  
private:
  Country * source;
  Country * destination;
};

vector<ValidMove*> validMoves;

class Region
{
public:
  Region(Country* c);
  ~Region(){};
  bool isIn(Country* c);
  vector<Country*> getVector(){return r;};
  void dump()
  {
    // for debugging purposes
    for( int i=0; i<r.size(); i++ )
      {
	cout << *r[i] << endl;
      }
  };
  friend ostream& operator <<(ostream& out, const Region& region);

private:
  void addFriendlies(Country* c);
  
  vector<Country*> r;
};

ostream& operator <<(ostream& out, const Region& region)
{
  for( int i = 0; i<region.r.size(); i++ )
    out << region.r[i]->getName() << " ";
  return out;
};


Region::Region(Country* c)
{
  // if the country (c) is already in 'r' then don't add it
  r.push_back(c);
  addFriendlies(c);
};

bool Region::isIn(Country* c)
{
  bool found = false;
  for( int i=0; i<r.size(); i++ )
    {
      //cout << *r[i] << " ?==? " << *c << endl;
      if( *r[i] == *c ) found=true;
    }
  
  return found;
}

void Region::addFriendlies(Country* c)
{
  // look for these countries in the region
  for( int i=0; i<c->getFriendlyNeighbors().size(); i++ )
    {
      if( !isIn(c->getFriendlyNeighbors()[i]) )
	{
	 
	  //cout << "adding " << *c->getFriendlyNeighbors()[i] << endl;
	  r.push_back(c->getFriendlyNeighbors()[i]);
	  addFriendlies(c->getFriendlyNeighbors()[i]);
	}
    }
}



int Player::moveTroops()
{  
#ifdef DEBUG
  int db_input;
  cerr << "*** AI ***" << endl << "players[" << order << "]->moveTroops()" << endl;
#endif
  if( getNumberOfPlayersCountries( order ) == 0 )
    {
#ifdef DEBUG
      cerr << "*** AI ***" << endl << "players[" << order << "] is wiped out" << endl;
#endif
      return -1;
    }
  vector<Region*> r;
  int region_index=0;
  for( int i=0; i<countries.size(); i++ )
    {
      if( countries[i]->getPlayer() == order )
	{
	  if( r.size() == 0 )
	    {	      
	      Region * tmpR = new Region( countries[i] );
	      r.push_back( tmpR );
	      cerr << "adding 1st region" << endl;
	    }
	  else
	    {
	      // before creating a new region, check to make
	      // sure that countries[i] is not already part
	      // of an existing region
	      bool found=false;
	      for( int j=0; j<r.size(); j++ )
		{
		  if( r[j]->isIn( countries[i] ) )
		    {
		      found = true;
		    }		  
		}
	      if( !found )
		{
		  r.push_back( new Region( countries[i] ) );
		  cerr << "adding another region" << endl;
		}
	    }
	}
    }

  
  // iterate over all the regions
  for( int i=0; i<r.size(); i++ )
    {
      int moveable_troops = 0;
      int non_isolated_countries = 0;
      vector<Country*> v = r[i]->getVector();
      if( v.size() > 1 )
	{	  
	  // count up all of the moveable troops in region i
	  // only if there's more than one territory in region i
	  // also count up the number of non-isolated countries
	  for( int j=0; j<v.size(); j++ )
	    {
	      if( !v[j]->isIsolated() )
		{
		  cerr << *v[j] << " is not isolated" << endl;
		    non_isolated_countries++;
		}
	      moveable_troops += (v[j]->getNumber()-1);
	      v[j]->setNumber(1);
	    }
#ifdef DEBUG
	  // displayMap();
	  cout << "Region " << i << " has [" << moveable_troops << "] moveable troops and [" << non_isolated_countries << "] border countries.\t" << *r[i] << endl;
#endif

	  while( moveable_troops > 0 && v.size()>1)
	    {
	      for( int j=0; j<v.size(); j++ )
		{
		  if( v[j]->isIsolated() )
		    {
		      cout << "don't add troops to isolated country: " << *v[j] << endl;
		    }
		  else
		    {
		      v[j]->addTroops(1);
		      moveable_troops--;
		    }
		  if( moveable_troops == 0 ) j=v.size();
		}
	    }
	  

	}
#ifdef DEBUG
  cin >> db_input;
  displayMap();
#endif
    }

  
  
  return -1;
}

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
  int max_columns = 0;
  if( v.size() > 10 )
    {
      max_columns = 2;
    }
  
  int which_col=0;
  for( int i = 0; i < v.size(); i++ )
    {
      cout << "(" << i << ") " << *v[i] << "\t\t";
      if( ++which_col > max_columns )
	{
	  cout << endl;
	  which_col = 0;
	}
    }
  return;
}

void clearHalf()
{
  cout << "\e[25;0H\e[J\e[25;0H";
  return;
}

void displayMap()
{
  cout << "\e[0;0H\e[J";
  cout << "                                                                                               ________       ________       ________       ________" << endl;      
  cout << "<-                                                                                            |        |-----|        |-----|        |-----|        |--->" << endl;
  cout << "  \\                                                                                           |________|     |________| \\   |________| ____|________|    " << endl;
  cout << "   \\                                                                                     ____/    |     \\____    |     \\ \\__    |     /   /    | " << endl;
  cout << "    \\________       ________       ________       ________       ________       ________/      ___|____      \\___|____  \\   \\___|____/   /  ___|____      " << endl;
  cout << "    |        |-----|        |-----|        |-----|        |-----|        |-----|        |-----|        |-----|        |  |  |        |  /  |        | " << endl;
  cout << "    |________|     |________|     |________|     |________|     |________|     |________|     |________|     |________|  |  |________| /   |________|    " << endl;
  cout << "        |     ____/    |     ____/    |              |     ____/    |     ____/    |    \\_____    |    \\_____    |    \\\\ \\__    |     / ___/" << endl;
  cout << "     ___|____/      ___|____/      ___|____       ___|____/      ___|____/      ___|____      \\___|____      \\___|___  \\\\___\\___|____/_/   " << endl;
  cout << "    |        |-----|        |-----|        |     |        |-----|        |-----|        |-----|        |-----|       | |    |        | " << endl;
  cout << "    |________|     |________|     |________|     |________|     |________|     |________|     |________|     |_______| |    |________|     " << endl;
  cout << "        |     ____/    |     ____/                         \\____    |     _____/  |      \\____    |     \\        |     /        " << endl;
  cout << "     ___|____/      ___|____/                                   \\___|____/      __|_____      \\___|____  \\    ___|____/      ________       ________  " << endl;    
  cout << "    |        |-----|        |                                   |        |-----|        |-----|        |  )  |        |-----|        |-----|        | " << endl;
  cout << "    |________|     |________|                                   |________|     |________|     |________| /   |________|     |________|     |________|    " << endl;
  cout << "              \\____    |                                                  _____/   |    \\____     |     /                       |     _____/   | " << endl;
  cout << "                   \\___|____       ________       ________      _________/      ___|____     \\____|____/                     ___|____/      ___|____      " << endl;
  cout << "                   |        |-----|        |-----|        |____/               |        |-----|        |                    |        |-----|        | " << endl;
  cout << "                   |________|     |________|     |________|                    |________|     |________|                    |________|     |________|    " << endl;
  cout << "                                            \\____    |    \\_____                         \\____    |     \\____" << endl;
  cout << "                                                 \\___|____      \\________                     \\___|____      \\________ " << endl; 
  cout << "                                                 |        |-----|        |                    |        |-----|        | " << endl;
  cout << "                                                 |________|     |________|                    |________|     |________| " << endl;
  for( int i=0; i<countries.size(); i++ )
    {
      countries[i]->map_print();
    }
  
  cout << "\e[25;0H";
  return;
}

void vectorToMenu(vector<Country*> v)
{
  int rows=0;
  for( int i = 0; i < v.size(); i++ )
    {
      cout << "(" << i << ") " << *v[i];
      if( rows++ == 3 )
	{
	  cout << endl;
	  rows=0;
	}
      else
	{
	  cout << "\t";
	}
    }
  cout << endl << ": ";
  return;
}

void displayAllCountries()
{
  int rows=0;
  for( int i = 0; i < countries.size(); i++ )
    {
      cout << *countries[i];
      if( rows++ == 3 )
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

int die()
{
  return (rand() % 6) + 1;
}

int main()
{
  cout << " ███████████   █████  █████████  █████   ████" << endl;
  cout << "░░███░░░░░███ ░░███  ███░░░░░███░░███   ███░ " << endl;
  cout << " ░███    ░███  ░███ ░███    ░░░  ░███  ███   " << endl;
  cout << " ░██████████   ░███ ░░█████████  ░███████    " << endl;
  cout << " ░███░░░░░███  ░███  ░░░░░░░░███ ░███░░███   " << endl;
  cout << " ░███    ░███  ░███  ███    ░███ ░███ ░░███  " << endl;
  cout << " █████   █████ █████░░█████████  █████ ░░████" << endl;
  cout << "░░░░░   ░░░░░ ░░░░░  ░░░░░░░░░  ░░░░░   ░░░░ " << endl;
  cout << "coded by michael k pellegrino - february 2024" << endl << endl;                                           

  bool gotValidInput=false;

  srand(time(NULL));   // Initialization of Random Number Generator, should only be called once.
  int number_of_players=0;
  while( gotValidInput == false )
    {
      cout << "*** enter number of players: " << endl;
      cin >> number_of_players;
      if( number_of_players <2 || number_of_players > 6 )
	{
	  cerr << "enter a number from 2 to 6 please." << endl;
	}
      else
	{
	  gotValidInput = true;
	}
    }
  gotValidInput=false;
  
  for( int i=0; i<number_of_players; i++ )
    {      
      players.push_back( new Player( new string( "Player " + to_string(i)), i));
      players[i]->setStrategy(i);
      int isAI=0;
      while( gotValidInput == false )
	{
	  cout << "Is " << *players[i] << " an AI? (1=Yes   2=No)" << endl;
	  cin >> isAI;
	  if( isAI !=1 && isAI !=2 )
	    {
	      cerr << "enter a 1 or a 2 please." << endl;
	    }
	  else
	    {
	      if( isAI == 1 ) players[i]->setAI(true);
	      gotValidInput = true;
	    }
	}
      gotValidInput=false;
    }


  gotValidInput = false;
  bool randomize_territories = false;
  int input4=0;
  while( gotValidInput == false )
    {
      cout << "*** randomize territories (1 yes  -  2 no): " << endl;
      cin >> input4;
      if( input4 != 1 && input4 !=2 )
	{
	  cerr << "enter a 1 or a 2 please" << endl;
	}
      else
	{
	  gotValidInput = true;
	}
    }
  
  
  // CREATE THE NODES
  countries.push_back( new Country( new string("Alaska"), ALASKA, 1,0, new string("ALASKA")));
  countries.push_back( new Country( new string("Northwest Territory"), NORTHWEST_TERRITORY, 1, 1, new string("NW TER")));
  countries.push_back( new Country( new string("Alberta"), ALBERTA, 2, 0, new string("ALBRTA")));  
  countries.push_back( new Country( new string("Greenland"), GREENLAND, 1, 2, new string("GRNLND")));
  countries.push_back( new Country( new string("Ontario"), ONTARIO, 2, 1, new string("ONTARO")));
  countries.push_back( new Country( new string("Quebec"), QUEBEC, 2, 2, new string("QUEBEC")));
  countries.push_back( new Country( new string("Western U.S."), WESTERN_US, 3, 0, new string("WESTUS")));
  countries.push_back( new Country( new string("Eastern U.S."), EASTERN_US, 3, 1, new string("EASTUS")));
  countries.push_back( new Country( new string("Central America"), CENTRAL_AMERICA, 4, 1, new string("CENTAM")));
  countries.push_back( new Country( new string("Iceland"), ICELAND, 1, 3, new string("ICELND")));

  countries.push_back( new Country( new string("Venezuela"), VENEZUELA, 4, 2, new string("VENZLA")));
  countries.push_back( new Country( new string("Peru"), PERU, 5, 3, new string(" PERU")));
  countries.push_back( new Country( new string("Argntna"), ARGENTINA, 5, 4, new string("ARGENT")));
  countries.push_back( new Country( new string("Brazil"), BRAZIL, 4, 3, new string("BRAZIL")));


  countries.push_back( new Country( new string("Great Britain"), GREAT_BRITAIN, 2, 3, new string("BRITAN")));
  countries.push_back( new Country( new string("Scandanavia"), SCANDANAVIA, 1, 4, new string("SCNDVA")));
  countries.push_back( new Country( new string("N. Europe"), NORTHERN_EUROPE, 2, 4, new string("N. EUR")));
  countries.push_back( new Country( new string("W. Europe"), WESTERN_EUROPE, 3, 4, new string( "W. EUR")));
  countries.push_back( new Country( new string("S. Europe"), SOUTHERN_EUROPE, 2, 5, new string( "S. EUR")));
  countries.push_back( new Country( new string("Ukraine"), UKRAINE, 1, 5, new string("UKRANE")));
  
  countries.push_back( new Country( new string("N. Africa"), NORTH_AFRICA, 3, 5, new string("N AFRC")));
  countries.push_back( new Country( new string("E. Africa"), EAST_AFRICA, 4, 6, new string( "E AFRC")));
  countries.push_back( new Country( new string("Egypt"), EGYPT, 3, 6, new string("EGYPT")));
  countries.push_back( new Country( new string("Congo"), CONGO, 4, 5, new string("CONGO")));
  countries.push_back( new Country( new string("S. Africa"), SOUTH_AFRICA, 5, 6, new string("S AFRC")));
  countries.push_back( new Country( new string("Madagascar"), MADAGASCAR, 5, 7, new string("MADSGR")));
  
  countries.push_back( new Country( new string("New Guinea"), NEW_GUINEA, 3, 9, new string("NGUNEA")));
  countries.push_back( new Country( new string("Indonsia"), INDONESIA, 3, 8, new string("INDONS")));
  countries.push_back( new Country( new string("W Australia"), WESTERN_AUSTRALIA, 4, 8, new string("WAUSTR")));
  countries.push_back( new Country( new string("E Australia"), EASTERN_AUSTRALIA, 4, 9, new string("EAUSTR")));

  countries.push_back( new Country( new string("Kamchatka"), KAMCHATKA, 0, 9, new string("KMCHTK")));
  countries.push_back( new Country( new string("Ural"), URAL, 0, 6, new string(" URAL")));
  countries.push_back( new Country( new string("Siberia"), SIBERIA, 0, 7, new string("SIBERA")));
  countries.push_back( new Country( new string("Yakutsk"), YAKUTSK, 0, 8, new string("YAKTSK")));
  countries.push_back( new Country( new string("Irkutsk"), IRKUTSK, 1, 8, new string("IRKTSK")));
  countries.push_back( new Country( new string("Japan"), JAPAN, 1, 9, new string("JAPAN")));
  countries.push_back( new Country( new string("Mongolia"), MONGOLIA, 2, 8, new string("MONGOL")));
  countries.push_back( new Country( new string("Afghan"), AFGHANISTAN, 1, 6, new string("AFGHAN")));
  countries.push_back( new Country( new string("Middle East"), THE_MIDDLE_EAST, 2, 6, new string( "M EAST")));
  countries.push_back( new Country( new string("India"), INDIA, 2, 7, new string("INDIA")));
  countries.push_back( new Country( new string("China"), CHINA, 1, 7, new string("CHINA")));
  countries.push_back( new Country( new string("Siam"), SIAM, 3, 7, new string(" SIAM")));




  // RANDOMIZE THE COUNTRIES AND GIVE 14 TO EACH OF THE 3 PLAYERS
  sort(countries.begin(), countries.end(), [](auto const& lhs, auto const& rhs) {return lhs->getRandomNumber() < rhs->getRandomNumber();});

  /* This is for random assignment of territories */
  // 35 armies each
  if( input4 == 1 )
    {
      int number_of_countries_each = 42/number_of_players;
      //int which_country=0;
      for( int i = 0; i < number_of_players; i++ )
	{
	  for( int j=0; j<number_of_countries_each; j++ )
	    {
	      countries[j+i*number_of_countries_each]->setPlayer(i);
	      countries[j+i*number_of_countries_each]->setNumber(1);
	    }
	}
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


  countries[AFGHANISTAN]->addBorderingCountry( countries[UKRAINE] );
  countries[AFGHANISTAN]->addBorderingCountry( countries[URAL] );
  countries[AFGHANISTAN]->addBorderingCountry( countries[CHINA] );
  countries[AFGHANISTAN]->addBorderingCountry( countries[INDIA] );
  countries[AFGHANISTAN]->addBorderingCountry( countries[THE_MIDDLE_EAST] );
 
  
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


  int num_of_extra=0;
  
  if( input4 == 2 )
    {
      for( int i = 0; i < number_of_players; i++ )
	{
	  players[i]->setUnplacedTroops( 50 - number_of_players*5 );
	}
      num_of_extra = 50 - number_of_players*5;
    }
  else
    {
      for( int i = 0; i < number_of_players; i++ )
	{
	  players[i]->setUnplacedTroops( 36 - number_of_players*5 );
	}
      num_of_extra = 36 - number_of_players*5;

    }

  land_grab_phase = true;
  
  for( int i=0; i<num_of_extra; i++ )
    {
      for( int j=0; j<number_of_players; j++ )
	{
	  clearHalf();	
	  displayMap();

	  cout << "*** INITIAL DISTRIBUTION OF TROOPS AND CLAIMING OF TERRITORIES ***" << endl;
	  cout << "*** PLAYER: " << *players[j] << " has " << players[j]->getUnplacedTroops() << " troops that need to be placed ***" << endl;


	  vector<Country*> v = getPlayersCountries(-1);
	  if( v.size() == 0 )
	    {
	      troop_placement_phase = true;
	      land_grab_phase = false;
	      
	      v = getPlayersCountries(j);
	      displayPlayersCountries(j);
	    }
	  else
	    {
	      displayPlayersCountries(-1);
	    }
	  cout << endl << ">> where would you like to place a troop?" << endl;
	  gotValidInput = false;
	  while( !gotValidInput )
	    {
	      int input3;
	      if( players[j]->getAI() )
		{
		  input3 = players[j]->chooseInitialPlacement(v);
		}
	      else
		{
		  cin >> input3;
		}
	      
	      if( input3 >= 0 && input3 < (int)v.size() )
		{
		  v[input3]->addTroops(1);
		  v[input3]->setPlayer(j);
		  players[j]->setUnplacedTroops(players[j]->getUnplacedTroops()-1);
		  gotValidInput = true;
		}
	      else
		{
		  cout << "*** invalid input ***" << endl;
		}
	    }
	}
    }

  
  while(1)
    {
      // iterate through everyone's turn

      /*  **** GAME PLAY *** */
      attack_phase = true;
      land_grab_phase = false;
      troop_placement_phase = false;

      for( int whosTurn=0; whosTurn < players.size(); whosTurn++ )
	{
	  bool still_their_turn = true;
	  
	  // if a player is wiped out... end their turn
	  if( getPlayersCountries(whosTurn).size() == 0 ) still_their_turn = false;
	  if( getPlayersCountriesThatHaveHostileNeighborsWithMoreThanOneTroop(whosTurn).size() == 0 ) still_their_turn = false;
	  while( still_their_turn )
	    {
	      // create a vector of possible starting points
	      vector<Country*> possible_countries_to_start_from = getPlayersCountriesThatHaveHostileNeighborsWithMoreThanOneTroop(whosTurn);
	      vector<Country*> possible_countries_to_attack;
	      
	      
	      gotValidInput=false;
	      
	      int x=0;
	      while(!gotValidInput)
		{
		  // Display the vector of possible starting countries
		  if( possible_countries_to_start_from.size() == 0 )
		    {
		      gotValidInput=true;
		      x = -1; // end the turn because there are no more countries that can be attacked
		    }
		  else
		    {
		      clearHalf();
		      displayMap();
		      cout << endl << "*** Player: " << *players[whosTurn] << "'s turn ***" << endl;
		      cout << endl << "*** Choose where your attack will originate from ***" << endl;
		      cout << "(-1 to end turn)" << endl;
		      vectorToMenu( possible_countries_to_start_from );
		      
		      cin >> x;
		      
		      if( x >= -1 && x < (int)possible_countries_to_start_from.size())
			{
			  gotValidInput=1;
			}
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
		  // build list of possible countries to attack
		  //{
		  vector<Country*> list_of_hostile_neighbors = c->getHostileNeighbors();
		  gotValidInput=false;
		  int y;
		  while( !gotValidInput )
		    {
		      clearHalf();
		      displayMap();
		      cout << "*** you chose " << *c << " ***" << endl << endl;
		      
		      cout << endl << "*** Choose where your attack will take place ***" << endl;
		      cout << "(-1 to exit)" << endl;

		      vectorToMenu(list_of_hostile_neighbors);
		      
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
		      
		  int attacker_troops_total = 0;
		  int defender_troops_total = d->getNumber();

		  gotValidInput=false;
		  while( !gotValidInput )
		    {
		      clearHalf();
		      displayMap();
		      
		      cout << endl << "*** you chose to attack " << *d << " from " << *c << " ***" << endl;

		      // the attack will now take place
		      cout << "how many troops do you want to use?  (max: " << c->getNumber()-1 << ")" <<endl;
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

		  //cerr << "ATTACKING WITH " << attacker_troops_total << " troops" << endl;
		  //cerr << "DEFENDING WITH " << defender_troops_total << " troops" << endl;

		  // remove the number of attacking troops from the originating country
		  c->setNumber( c->getNumber() - attacker_troops_total );
		  displayMap();
		      
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

#ifdef DEBUG
      cout << "entering troop movement phase (type any number)" << endl;
      cin >> db_integer;
#endif
      attack_phase = false;
      troop_movement_phase = true;
      for( int i=0; i<players.size(); i++ )
	{
	  /* ==================================== */

	  bool still_more_movements = true;
	  int input = -1;
	  if( getPlayersCountries(i).size() <= 1 ) still_more_movements = false;
	  while( still_more_movements )
	    {
	      vector<Country*> player_countries = getPlayersCountriesThatHaveFriendlyNeighborsWithMoreThanOneTroop(i);
	      gotValidInput=false;
	      if( player_countries.size() == 0 )
		{
		  gotValidInput = true;
		  still_more_movements = false;
		}
	      while( !gotValidInput && still_more_movements )
		{
		  clearHalf();
		  displayMap();
		  cout << "* * * * * T R O O P   M O V E M E N T S * * * * *" << endl;
		  if( players[i]->getAI() )
		    {
		      //players[i]->chooseCountryToMoveTroopsFrom( player_countries );
		      //cout << "PRE" << endl;
		      input = players[i]->moveTroops();
		      //cout << "POST" << endl;
		    }
		  else
		    {
		      cout << endl << endl << "*** PLAYER: " << *players[i] << " TROOP MOVEMENTS ***" << endl;
		      cout << "*** Choose a territory to move troops FROM from (-1 to end): ***" << endl;
		      vectorToMenu( player_countries );
		      cin >> input;
		    }
		  
		  if( input>=-1 && input<(int)player_countries.size() )
		    {
		      gotValidInput=true;
		    }
		}
	      
	      if( input == -1 )
		{
		  //cerr << endl << "*** end turn ***" << endl;
		  still_more_movements = false;
		  //cin >> input;
		}
	      else
		{
		  Country * starting_from = player_countries[input];
		  Region * r = new Region( starting_from );
		  vector<Country*> friendlies = r->getVector();

		  gotValidInput=false;
		  int input2;
		  while( !gotValidInput )
		    {
		      displayMap();
		      clearHalf();
		      if( players[i]->getAI() )
			{
			  input2 = -1;// players[i]->chooseCountryToMoveTroopsTo( friendlies );
			}
		      else
			{
			  cout << ">> Move to? (-1 to cancel)" << endl;
			  vectorToMenu(friendlies);
			  cin >> input2;
			}
		      
		      if( input2>=-1 && input2<(int)friendlies.size() )
			{
			  gotValidInput=true;
			}
		     

		    }

		  if( input2 != -1 && gotValidInput )
		    {
		      bool match_found = false;
		      for( int i=0; i<validMoves.size(); i++ )
			{
			  if( validMoves[i]->same( friendlies[input2]->getName(), starting_from->getName() ) )
			    {
			      match_found = true;
			    }
			}

		      if( !match_found )
			{
			  cout << "Moving from " << *starting_from << " to " << *friendlies[input2] << endl;
			  friendlies[input2]->addTroops(1);
			  starting_from->troopReduction();			  
			  validMoves.push_back( new ValidMove( starting_from, friendlies[input2] ));
			}
		    }
		}
	    }
	  /* ==================================== */
	}

      /* DISTRIBUTE TROOPS */
      // calculate additions
      troop_placement_phase = true;
      attack_phase = false;
      land_grab_phase = false;
      troop_movement_phase = false;

      int input5;
      cout << "*** (1) ***" << endl;
      cin >> input5;
      for( int i=0; i<players.size(); i++ )
	{
	  int additional_troops=0;
	  if( getNumberOfPlayersCountries(i) == 0 )
	    {
	      additional_troops = 0;
	    }
	  else
	    {
	      additional_troops = getNumberOfPlayersCountries(i)/3;
	    }
	  
	  for( int j = 0; j < additional_troops; j++ )
	    {
	      vector<Country*> v = getPlayersCountries(i);
	      gotValidInput = false;
	      while( !gotValidInput )
		{
		  clearHalf();	
		  displayMap();

		  int input3;
		  if( !players[i]->getAI() )
		    {
		      cout << "*** DISTRIBUTE TROOPS ***" << endl;
		      cout << "*** PLAYER: " << *players[i] << " has " << (additional_troops-j) << " troops that need to be placed ***" << endl;
		      cout << ">> where would you like to place one?" << endl;
		      vectorToMenu(v);
		      cin >> input3;
		    }
		  else
		    {
		      if( additional_troops > 0 )
		      {
			cout << "PRE" << endl;
			input3 = players[i]->chooseInitialPlacement(v);
			cout << "POST" << endl;
		      }
		    }

		  if( input3 >= 0 && input3 < (int)v.size() )
		    {
		      v[input3]->addTroops(1);
		      gotValidInput = true;
		    }
		  else if( input3 == -1 )
		    {
		      gotValidInput = true;
		    }
		  else
		    {
		      cout << "*** invalid input ***" << endl;
		    }
		}
	    }
	}
      /* ------------ distribute troops ----------- */
    }
 
  
  deleteAll();
  
  return 0;
}
