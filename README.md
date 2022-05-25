# Trees-and-Tents

In this project, a program was developed capable of solving tent placement problems in campsites according to a given set of restrictions.
The parks are rectangular maps (matrixes) of L rows and C columns and, consequently, L×C cells. The characterization of each map includes: two integers (L and C), a sequence of L integers (line vector - number of braces to be placed per line), a sequence of C integers (column vector - number of braces to be placed per column) and a set of L strings of C characters (L×C matrix whose cells can be empty (.) or have a tree (A)).
The instructions for solving the map are:

• Each tent must be placed next to a tree (in one of the four adjacent non-diagonal cells);

• Only one tent can be assigned to each tree;

• It is not allowed to place tents in any of the seven adjacent positions, including diagonals, of another tent already placed;

• The sum of the tents in the column vector and the sum of the tents in the lines vector must be equal to the total number of trees placed on the map;

• The sum of the tents in row Y must be equal to the integer in the Y position of the row vector (analogous for columns);

There are also two aspects of resolution: high season, number of trees equal to the number of tents to be placed (each tree must have a tent) and low season, there are more trees than tents (there will be trees to which no tent will be assigned ).
