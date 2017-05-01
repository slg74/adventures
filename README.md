
About  
-----  

Match a pattern in an input file. E.g. cat file | ./grup %{0S1} %{0} ran away  
Where %{0S1} matches "big brown" and %{0} matches fox, if the input line is  

"the big brown fox ran away"  

grup.cpp - compile with:  

g++ -o grup grup.cpp -lpcrecpp --std=c++11

Usage  
-----  

cat file | ./grup "the %{0S1} %{0} ran away"  

produces  

the big brown fox ran away  


Valgrind  
--------  

[scott@whargarbl adventures]$ cat file | valgrind -v --log-file=valgrind_grup.out ./grup "%{01} %{0} ran away"  
the big brown fox ran away  


This produces the file "valgrind_grup.out" in the CWD.  
This file reports 0 memory leaks or errors.  



