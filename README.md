Infix Calculator
----------------
[Infix Notation](https://en.wikipedia.org/wiki/Infix_notation) is the mathematical notation scheme
 we all 
learned as children. For example: `(4 * 5) + (36 / 4) - 5`. This application solves equations in 
infix notation.
```
$ calculator

Press Enter to quit.
Enter an arithmetic equation in infix notation: (4 * 5) + (36 / 4) - 5
    
( 4 * 5 ) + ( 36 / 4 ) - 5 = 24
    
Enter an arithmetic equation in infix notation: 
$ 

```
It can handle both missing and extra spaces in its input:
```
$ calculator

Press Enter to quit.
Enter an arithmetic equation in infix notation: 8--7   +8++7    
    
8 - -7 + 8 + +7 = 30
    
Enter an arithmetic equation in infix notation: 
$ 

```
The input equation is pretty-printed and solved. Input may contain the alternate operators x, ÷, 
and −. These are swapped for the symbols *, /, and - supported by C++.

The debug mode shows the algorithm in action:
```
$ calculator -d

Press Enter to quit.
Enter an arithmetic equation in infix notation: (8−−7)+(9×+1)−−3.4−4÷7*2^2

( 8 - -7 ) + ( 9 * +1 ) - -3.4 - 4 / 7 * 2 ^ 2
Pushed ( onto operator stack. Size = 1
Pushing 8 onto value stack. Size = 1
Pushed - onto the operator stack. Size = 2
Pushing -7 onto value stack. Size = 2
operator -, operand_l 8, operand -7
Pushing 15 onto value stack. Size = 1
Pushed + onto the operator stack. Size = 1
Pushed ( onto operator stack. Size = 2
Pushing 9 onto value stack. Size = 2
Pushed * onto the operator stack. Size = 3
Pushing 1 onto value stack. Size = 3
operator *, operand_l 9, operand 1
Pushing 9 onto value stack. Size = 2
operator +, operand_l 15, operand 9
Pushing 24 onto value stack. Size = 1
Pushed - onto the operator stack. Size = 1
Pushing -3.4 onto value stack. Size = 2
operator -, operand_l 24, operand -3.4
Pushing 27.4 onto value stack. Size = 1
Pushed - onto the operator stack. Size = 1
Pushing 4 onto value stack. Size = 2
Pushed / onto the operator stack. Size = 2
Pushing 7 onto value stack. Size = 3
operator /, operand_l 4, operand 7
Pushing 0.571429 onto value stack. Size = 2
Pushed * onto the operator stack. Size = 2
Pushing 2 onto value stack. Size = 3
Pushed ^ onto the operator stack. Size = 3
Pushing 2 onto value stack. Size = 4
operator ^, operand_l 2, operand 2
Pushing 4 onto value stack. Size = 3
operator *, operand_l 0.571429, operand 4
Pushing 2.28571 onto value stack. Size = 2
operator -, operand_l 27.4, operand 2.28571
Pushing 25.1143 onto value stack. Size = 1

( 8 - -7 ) + ( 9 * +1 ) - -3.4 - 4 / 7 * 2 ^ 2 = 25.11

Enter an arithmetic equation in infix notation: 
$ 

```
The test mode `calculator -t` runs a [map full of equations](calculator_testing.cpp) in debug mode 
and reports any 
errors.

Credits
-------
[Bjarne Stroustrup](http://www.stroustrup.com/) uses the challenge of writing a calculator as an 
instructional exercise in 
“Programming: Principles and Practice Using C++, Second Edition”. This gave me the inspiration 
for the project.

[Dave Matuszek](https://www.linkedin.com/in/david-matuszek-018a2/)
is the designer of the [algorithm](https://www.cis.upenn.edu/~matuszek/cit594-2002/Assignments/5-expressions.html) used in this application. All algorithm design credit goes to 
him. Naturally any implementation blame should go to the author.
