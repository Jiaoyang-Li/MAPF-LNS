# MAPF-LNS
![test_ubuntu](https://github.com/Jiaoyang-Li/MAPF-LNS/actions/workflows/test_ubuntu.yml/badge.svg)
<!---![test_macos](https://github.com/Jiaoyang-Li/MAPF-LNS/actions/workflows/test_macos.yml/badge.svg)--->

Anytime Multi-Agent Path Finding via Large Neighborhood Search


MAPF-LNS is an effifent anytime algorithm for solving Multi-Agent Path Finding (MAPF). 
More details can be found in our paper at IJCAI 2021 [1].

The code requires the external libraries 
BOOST (https://www.boost.org/) and Eigen (https://eigen.tuxfamily.org/). 
An easy way to install the required libraries on Ubuntu:    
```shell script
sudo apt update
```
- Install the Eigen library (used for linear algebra computing)
    ```shell script
    sudo apt install libeigen3-dev
    ```
- Install the boost library 
    ```shell script
    sudo apt install libboost-all-dev
    ```
    
After you installed both libraries and downloaded the source code, 
go into the directory of the source code and compile it with CMake: 
```
cmake -DCMAKE_BUILD_TYPE=RELEASE .
make
```

You also need to download the MAPF instances from the MAPF benchmark (https://movingai.com/benchmarks/mapf/index.html).

Then, you are able to run the code:
```
./lns -m random-32-32-20.map -a random-32-32-20-random-1.scen -o test.csv -k 50 -t 60
```

- m: the map file from the MAPF benchmark
- a: the scenario file from the MAPF benchmark
- o: the output file
- k: the number of agents
- t: the runtime limit

You can find more details and explanations for all parameters with:
```
./lns --help
```

## Credits

The software was developed by Jiaoyang Li and Zhe Chen.

The rule-based MAPF solvers (i.e., PPS, PIBT, and winPIBT) inside the software were borrowed from 
https://github.com/Kei18/pibt/tree/v1.3

MAPF-LNS is released under USC – Research License. See license.txt for further details.
 
## References
[1] Jiaoyang Li, Zhe Chen, Daniel Harabor, Peter J. Stuckey, Sven Koenig.
Anytime Multi-Agent Path Finding via Large Neighborhood Search.
In Proceedings of the International Joint Conference on Artificial Intelligence (IJCAI), (in print), 2021.           
[2] Jiaoyang Li, Zhe Chen, Daniel Harabor, Peter J. Stuckey, Sven Koenig.
Anytime Multi-Agent Path Finding via Large Neighborhood Search: Extended Abstract.
In Proceedings of the International Conference on Autonomous Agents and Multiagent Systems (AAMAS), pages 1581-1583, 2021.

 

 
