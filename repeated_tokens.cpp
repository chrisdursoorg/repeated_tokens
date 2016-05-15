// main.cpp

#include <iostream>
#include <iomanip>
#include <array>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <cassert>

using namespace std;


// a str_by_range is a string by iterator pair, that is where the actual letters are not owned by the string
// the advantage is that the underlying string [begin, end) does not have to be a  c-style (or null terminated) string
//
//  This class would allow you to substr without actually making a copy, and the container can be used in the context of 
// std containers and algorithms

struct str_by_range {

  str_by_range(const char* begin): begin_(begin), end_(begin + ::strlen(begin)){} 

  str_by_range(const char* begin, const char* end): begin_(begin), end_(end){} 

  const char* begin_;
  const char* end_;

  bool operator<(const str_by_range& rhs)const
  {
    // less-than iff there exists first non matching charactor is less-than
    // or run out of less firstp
    const char* cl;
    const char* cr;
    for(cl = begin_, cr = rhs.begin_; cl != end_ && cr != rhs.end_; ++cl, ++cr )
      if(*cr != *cl)
	return *cl < *cr; 
    
    // cl,cr  END => equal
    // cr     END => overlapping equal cr ends first
    // cl     END => overlapping equal cl ends first
    return (cl == end_) && (cr != rhs.end_);
  }
};

ostream& operator<<( ostream& lhs, const str_by_range& rhs)
{
  lhs.write(rhs.begin_, rhs.end_-rhs.begin_);
  return lhs;
}


// search_algorithm
//
// search algorithm
// runs all the possible combinations of the key space, checks the
// potential results
//
// a limit() greatly reduces the scope of iteration
//
// made into class not so much for reuse, but to limit the interface stack size
// by utilizing *this, and for organization

class search_algorithm
{
public:

  search_algorithm(const char* string_begin, const char* string_end)
  : string_begin_(string_begin)
  , string_end_(string_end)
  {}

  void start(const char* b){
    checked_ = 0;
    matched_ = 0;
    permute(b, b + strlen(b));
  }

  void print_stats(){
    cout << "checked a total of " << checked_ << " potential matches" << endl;
  }
  
private: 

  void permute(const char* b, const char* e);

  bool check(vector<str_by_range>::const_iterator c1, vector<str_by_range>::const_iterator end_c1);

  // limit
  //
  // The total number of tokens for a true match is the string_size
  // much time can be saved by stopping here
  int limit() { return string_end_ - string_begin_; }


  vector<vector<str_by_range> >  stor_;
  int                            checked_;
  int                            matched_;
  const char*                    string_begin_;
  const char*                    string_end_;
};


int main(int argc, const char* argv[]){
  
  if(argc != 3)
    {
      cout << "\nPROGRAM 'token0token1...TokenN'  'matchstring'" << endl << endl;
      cout << "this program seaches out tokens that can match 'matchstring', in order to match each token must match exactly each charactor in matchstring" << endl << endl;
      cout << "EXAMPLE: \n\nPROGRAM 'foobarfoofoo'  'wyww'\n1 foo bar foo foo \nchecked a total of 232 potential matches" << endl << endl;
      cout << "PROGRAM  'foobarfoo' 'fobo'\n" << "1 f o obarfo o \n2 f oo barf oo \n3 fo o barfo o\nchecked a total of 93 potential matches\n" << endl; 
      
      return 0; 
    }

  search_algorithm c(argv[2], argv[2] + strlen(argv[2]));
  c.start(argv[1]);
  c.print_stats();

  return 0;
}


void 
print_match( const vector<str_by_range>& r, int matches)
{      
  cout << setw(6) << matches++ << " "; 
  for(vector<str_by_range>::const_iterator c = r.begin(); c != r.end(); ++c)
    cout << *c << " ";
  cout <<  endl;
}

void 
search_algorithm::permute( const char* b, const char* e){

  // we are done, test it, print it pop it
  if(b == e)
    {
      if(stor_.size())
	{
	  ++checked_;
	  const  vector<str_by_range>& token_sequence = stor_.back();
	  if(check(token_sequence.begin(), token_sequence.end()))
	    {
	      print_match(token_sequence, ++matched_);
	    }
	  stor_.pop_back();
	} 
      return;
    }
  
  vector<str_by_range> base;
  if(stor_.size())
    {
      base = stor_.back();
      stor_.pop_back();
    }
  
  // we are done if we matched the limit
  if( base.size() != limit() )
    {
      for(const char* m = b + 1; m <= e; ++m){
	vector<str_by_range> row(base);
	row.push_back(str_by_range(b, m));
	stor_.push_back(row);
	permute(m, e);
      }
    }
}


// check
//
// for each c-string respresented by C1 [begin, end), compare with C2 [begin, end)  
//
bool 
search_algorithm::check(vector<str_by_range>::const_iterator c1, vector<str_by_range>::const_iterator end_c1)
{
  const char* c2 = string_begin_;
  int token_size  = distance(c1, end_c1);
  int string_size = distance(c2, string_end_);
 
  if(token_size != string_size)
    {
      // cout << "dissimilar sized tokens and string cannot be be equivalent: token: " << token_size << " string: " << string_size << endl; 
      return false;
    }

  map<str_by_range, char> a1_a2;
  set<char>               a2_;

  for(; c1 != end_c1; ++c1, ++c2)
    {
      map<str_by_range, char>::const_iterator fs;
      if( (fs = a1_a2.find(*c1)) == a1_a2.end()  )
	{
	  if( a2_.find(*c2) != a2_.end() )
	    {
	      // cout << "encountered repeated '" << *c2 << "' when '" << *c1 << "' was not repeated" << endl; 
	      return false;
	    }
	  // a new <c1, c2> pair discovered
	  a1_a2[*c1] = *c2;
	  a2_.insert(*c2);
	}
      else
	{
	  if(a1_a2[*c1] != *c2)
	    {
	      // cout << "encountered inconsistecy, repeated '" << *c1 << "' incorrectly remapped from '" << a1_a2[*c1]<< "' to '" << *c2 << "'" << endl  ;
	      return false;
	    }
	}	  
    }

  //  cout << "sets consistent" << endl;
  return true;
  }
