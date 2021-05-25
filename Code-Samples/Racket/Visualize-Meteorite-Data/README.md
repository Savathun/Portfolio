Name: Daniel Schlatter
Date: 8/3/19
Class: CSCI 3415
# Program Report

## Abstract

This report contains the problem statement for the Visualize Meteorite Data program, my approach to coding the program, my test results, and conclusions about what I learned while coding it.

## Problem Statement

For this assignment, you will make additions to this program.

Each student will extend the simple-meteorites.rkt Racket program as follows:

For a C-level grade, you must get the simple-meteorites.rkt Racket program as written running.

For a B-level grade, you must add a latitude and longitude grid to the output. Specifically, draw a light grid for every 10 degrees of latitude and longitude. Then, draw heavier lines for the equator (i.e., latitude 0) and the prime meridian (i.e., longitude 0). Also, don’t plot those meteorites with lat and long values of 0.

For an A-level grade, you must draw the meteorite size proportional to the mass of the
meteorite, using a logarithmic scale

## Approach

The method to getting the longitude and latitude lines on the map was a simple matter of iterating over the range with a for loop and drawing lines at each point, with the additions of some extra stuff for formatting purposes (making prime meridian and equator darker, changing color and transparency of line, etc.). A simple unless statement allows me to prevent meteorites being drawn on at 0,0. In order to get the meteorites size to scale logarithmically with their mass, all I had to do was take the mass value from the file, ensure all the 0’s and “” values were set to 1, then convert into to a number, and log it, then pass the logged value to the pen width before drawing.

## Results

```
Language: racket/gui, with debugging; memory limit: 128 MB.
""
"\n    Name: Daniel Schlatter\n    Date: 8/2/19\n    Class: CSCI 3415\n    Description: Program to take a csv file of meteorite data and map it onto an image of earth. \n"
""
There are 45716 meteorite landings in file "meteorite-landings.csv".
Fell    = 1097
Found   = 37304
Other   = 0
Invalid = 7315
Nomatch = 0

>
```

## Conclusions

Throughout the coding process, I learned many things about Racket syntax (define, draw, bitmap and pen related functions, cond/when/unless, for loop style, parenthesis use, function format, among other things), and how the graphical interface works.
