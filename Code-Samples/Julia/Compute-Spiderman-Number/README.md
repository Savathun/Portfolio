____
* Name: Daniel Schlatter
* Date: 8/1/19
* Class: CSCI 3415
___

# Program Report

## Abstract

This report contains the problem statement for Program 3, my approach to coding the program,
my test results, and conclusions about what I learned while coding it.

## Problem Statement

Each student will write a Julia program to compute the Spider Man numbers of a set of Marvel
characters. Specifically, the program should take as input an arbitrary number of Marvel
characters – either as integer indexes (easiest) or strings – and for each of these characters, print
the character’s name and their Spider Man number.

## Approach

The method to reading in the network is done simply by reading in each value into a separate
array for characters and comics, and a matrix for appearances. A sparse matrix is created by
multiplying appearances by its transpose, which is then passed to the Spidey number function.
The Spidey number is calculated by creating a Boolean matrix from the sparse matrix, then going
through this matrix to find the levels of separation of each character. An array set to -1’s has the
numbers of characters who have appeared directly with Spiderman set to 1’s, then characters
who have not appeared with Spiderman but have appeared with those characters set to 2, and so
on.

It is then a simple matter of printing the character desired by the user’s info, their name and
Spidey number, simply by accessing those arrays and printing their contents at the user entered
index.

## Results
```
julia> include("marvel.jl")
Reading Marvel universe network
Calculating Spidey Numbers
Enter the numbers (1 - 6486) of marvel characters (one at a
time)
1
 1 "24-HOUR MAN/EMMANUEL" : 3
2
 2 "3-D MAN/CHARLES CHAN" : 1
3
 3 "4-D MAN/MERCURIO" : 2
95
 95 "AMAZO-MAXI-WOMAN/" : >6
389
389 "BAV-TEK" : 3
957
957 "CERA, KAY" : 1
1082
1082 "CLINTON, BILL" : 1
1089
1089 "CLUMSY FOULUP" : >6
1388
1388 "DE LA SPIROSA, COUNT" : 2
1711
1711 "EL TORO ROJO" : 2
1841
1841 "FENRIS" : >6
2052
2052 "ÁGAMORA" : 1
2216
2216 "GREASE" : 2
2262
2262 "GROSS, IRA" : 1
2483
2483 "HITLER, ADOLF" : 2
2908
2908 "KUBIK" : 2
3202
3202 "LITTLE, ABNER" : 2
3481
3481 "MARTINEZ, ALITHA" : 1
3863
3863 "MYLZOB'RYN, CHIEF" : 3
4033
4033 "O'KEEFE, CHANDRA" : 1
4095
4095 "OSWALD" : >6
4387
4387 "PSI-BORG/" : 2
4575
4575 "REAGAN, NANCY" : 2
4630
4630 "REPTYL" : 1
4750
4750 "ROSE, MICHKA" : 3
5306
5306 "SPIDER-MAN/PETER PAR" : 0
5705
5705 "THANOS" : 1
6411
6411 "ZANTOR" : >6
6452
6452 "ZURI" : 1
6484
6484 "STORMER" : 3
6486
6486 "ZONE" : 2
ZDEUFH
Invalid Input. Enter a number between 1 and 6486. Enter -9
to escape
0
Invalid Input. Enter a number between 1 and 6486. Enter -9
to escape
88888
Invalid Input. Enter a number between 1 and 6486. Enter -9
to escape
Invalid Input. Enter a number between 1 and 6486. Enter -9
to escape
-9
Program Exited.
julia>
```

## Conclusions

Throughout the coding process, I learned many things about Julia syntax (goto, ends, @
symbols), and command line arguments for Julia, as well as using the debugger and using
Atom/Juno in general. I also learned about sparse matrices and their implementation in Julia.
