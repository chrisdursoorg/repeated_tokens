# repeated_tokens
A simple corporate test, or is it so simple?

The following was intended to be a twenty minute corporate engineering test or a couple of twenty minute tests.  The first part is to do the plain and simple checking agains defined tokens.  The follow up question, a bit more tricky, is to continue where the tokenization is ambiguous (or in this case completely non trivial).  It took me a bit more than twenty minutes to more or less optimize this second part of the test.  

Besides realizing that a iterative, recursing permutation approach more or less solves the problem in high order time, the limit of the width of the second argument lowers the complexity to O(2**(width(arg2)).  I further optimized by creating a str_in_range a string class allowing for range definition of elsewhere data, hence reduction of a lot of string allocation.  (Possibly this is the only useful reusable code.)  I could also have optimized by customizing the outer vector<> as that would undoubtedly be allocating and freeing objects at a furious rate.  This is probably not too much a help however with the complexity being as it is.


```
bash-3.2$ git clone git@github.com:chrisdursoorg/repeated_tokens.git repeated_tokens
Cloning into 'repeated_tokens'...
remote: Counting objects: 4, done.        
remote: Compressing objects: 100% (4/4), done.        
remote: Total 4 (delta 0), reused 0 (delta 0), pack-reused 0        
Receiving objects: 100% (4/4), 12.17 KiB | 0 bytes/s, done.
Checking connectivity... done.
bash-3.2$ cd repeated_tokens
bash-3.2$ cd ~/git/repeated_tokens; c++ -O3 -o repeated_tokens repeated_tokens.cpp 
bash-3.2$ ./repeated_tokens

PROGRAM 'token0token1...TokenN'  'matchstring'

this program seaches out tokens that can match 'matchstring', in order to match each token must match exactly each charactor in matchstring

EXAMPLE: 

PROGRAM 'foobarfoofoo'  'wyww'
1 foo bar foo foo 
checked a total of 232 potential matches

PROGRAM  'foobarfoo' 'fobo'
1 f o obarfo o 
2 f oo barf oo 
3 fo o barfo o
checked a total of 93 potential matches

bash-3.2$   time ./repeated_tokens  'foobarfoo' 'wmw'
     1 foo bar foo 
checked a total of 37 potential matches

real	0m0.005s
user	0m0.001s
sys	0m0.002s
bash-3.2$ cd ~/git/repeated_tokens; c++ -O3 -o repeated_tokens repeated_tokens.cpp 
bash-3.2$ ./repeated_tokens  'foobarfoo' 'wmw'
     1 foo bar foo 
checked a total of 37 potential matches
bash-3.2$ ./repeated_tokens  'foobarfoo' 'wmww'
checked a total of 93 potential matches
bash-3.2$ ./repeated_tokens  'foobarfoo' 'fobo'
     1 f o obarfo o 
     2 f oo barf oo 
     3 fo o barfo o 
checked a total of 93 potential matches
bash-3.2$./repeated_tokens  'foobarfoofoo' 'wyww'
     1 foo bar foo foo 
checked a total of 232 potential matches
bash-3.2$ time ./repeated_tokens  'doyoudodothatvoodothatyoudo' 'dyddtvdtyd'
     1 do you do do that voo do that you do 
checked a total of 5658537 potential matches

real	0m10.936s
user	0m10.920s
sys	0m0.009s
bash-3.2$ 
```