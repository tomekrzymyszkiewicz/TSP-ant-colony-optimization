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
| tsp\_6\_1.txt   | 6                  | 132                              | 
| tsp\_10.txt     | 10                 | 212                              |
| tsp\_12.txt     | 12                 | 264                              | 
| tsp\_13.txt     | 13                 | 269                              |
| burma14.txt     | 14                 | 3323                             |  
| tsp\_15.txt     | 15                 | 291                              | 
| tsp\_17.txt     | 17                 | 39                               |   
| gr21.txt        | 21                 | 2707                             |   
| gr24.txt        | 24                 | 1272                             |  
| bays29.txt      | 29                 | 2020                             |    
| att48.txt       | 48                 | 10628                            |    
| eil51.txt       | 51                 | 426                              |   
| berlin52.txt    | 52                 | 7542                             |   
| st70.txt        | 70                 | 675                              |   
| gr96.txt        | 96                 | 55209                            |   
| kroA100.txt     | 100                | 21282                            |   
| gr120.txt       | 120                | 6942                             | 
| kroB150.txt     | 150                | 26130                            |   
| pr152.txt       | 152                | 73682                            |   
| ftv170.txt      | 170                | 2755                             |  
| kroB200.txt     | 200                | 29437                            |   
| rbg323.txt      | 323                | 1326                             |  