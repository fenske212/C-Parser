//Takes Context Free Grammar(CFG) as input and organizes through parsing

//Organizing the grammar is demonstrated through 5 arguments


1 - Printing Terminals and Nonterminals

2 - Eliminating Useless Symbols (Non-Generating/Non-Useful)

3 - First Sets

4 - Follow Sets

5 - Determining Predictive Parser (Y/N)



//Example test case for Task 4

-make

- ./a.out 4 < testcases/Task_4/task4_test06.txt

FOLLOW(A043eSc) = { $, a043eSc, b043eSc, c043eSc, f043eSc, g043eSc }

FOLLOW(B043eSc) = { a043eSc, b043eSc, c043eSc, g043eSc }

FOLLOW(G043eSc) = { c043eSc, f043eSc, g043eSc, z043eSc }

FOLLOW(C043eSc) = { a043eSc, b043eSc, c043eSc, x043eSc, g043eSc }

FOLLOW(D043eSc) = { f043eSc, g043eSc }

FOLLOW(F043eSc) = { a043eSc, b043eSc, c043eSc, f043eSc, g043eSc }

FOLLOW(E043eSc) = { a043eSc, b043eSc, c043eSc, g043eSc
