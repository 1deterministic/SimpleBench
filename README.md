# SimpleBench

SimpleBench is a quick benchmarking tool I wrote to evaluate the performance of the systems I came across. It tests only the most basic arithmetic operations for integers and floating points besides memory bandwitdth.

![Image](./screenshots/1.png)

It supports the following options:
```
--show-gui [on/off]: enable/disable the text feedback during the tests
        --st-test [on/off]: enable/disable the singlethread test
        --mt-test [on/off]: enable/disable the multithread test
        --threads [integer greater than zero]: sets the number of threads to run with
        --pin-threads [on/off]: enable/disable pinning process threads to system threads, does not work on macOS
        --hardware-level [integer greater than zero]: chooses how taxing the benchmark will be
              1: 8.25MB of RAM, 1 lenght unit
              2: 33MB of RAM, 4 lenght units
              3: 132MB of RAM, 16 lenght units
              4: 528MB of RAM, 64 lenght units
              5: 2112MB of RAM, 256 lenght units
              6: 8448MB of RAM, 1024 lenght units
               :
              * every increase of 1 in the hardware level quadruples the requirements
              * at the moment there is no limit to the hardware level
        --version: shows the benchmark version (not the program version)
        --build: shows the program version, changes here have no effect on the score system
        --help: shows this screen
```

SimpleBench is not a stress test and not is not a reliable benchmarking tool for any kind of advanced features the processor may have. 

Also, the algorithm is not complex so the compiler optimizations will basically skip all work it should have done. If you want to build from the sources, disable any optimizations to have a meaningful score. There are scripts to build with the correct compiler flags included in the source folder too.

[These are the systems I benchmarked so far (I plan to include more systems in the future).](./SCORES.md)

## [1deterministic](https://github.com/1deterministic), 2019
