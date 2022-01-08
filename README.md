# TSP ant colony optimization

The project aims to solve TSP by ant colony optimization method

## config.ini file

The program reads data from the config.ini configuration file in the following syntax

```
<results file name>
<graph file name> <number of repeats> <alpha parameter> <beta parameter> <rho parameter> <number of iterations> <number of ants> <init tau parameter> <quantity of pheromone> <evaporation method> <correct TSP path weight> <correct TSP path> 
```
- results file name - name of CSV file
- graph file name - name of file with adjacency matrix of graph
- number of repeats - number of repeats of algorithm
- alpha parameter - 
- beta parameter - 
- rho parameter - 
- number of iterations - 
- number of ants - 
- init tau parameter
- quantity of pheromone - 
- evaporation method - 
- correct TSP path weight - integer number
- correct TSP path - sequence of integers number separated by space

## Graphs

In the *graphs* folder there are files with example graphs that can be used to test the program.

| graph file name | number of vertices | weight of minimum hamilton cycle |
|-----------------|--------------------|----------------------------------|
| burma14.txt     | 14                 | 3323                             | 
| gr17.txt        | 17                 | 2085                             |
| gr21.txt        | 21                 | 2707                             | 
| gr24.txt        | 24                 | 1272                             |
| bays29.txt      | 29                 | 2020                             |  
| ftv33.txt       | 33                 | 1286                             | 
| ftv44.txt       | 44                 | 1613                             |   
| ftv70.txt       | 70                 | 1950                             |   
| ch150.txt       | 150                | 6528                             |  
| ftv170.txt      | 170                | 2755                             |    
| gr202.txt       | 202                | 40160                            |    
| rbg323.txt      | 323                | 1326                             |   
| pcb442.txt      | 442                | 50778                            |   
| rgb443.txt      | 443                | 2720                             |   
| pr1002.txt      | 1002               | 259045                           |