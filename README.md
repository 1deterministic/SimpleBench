# SimpleBench

SimpleBench is a quick benchmarking tool I wrote to evaluate the performance of the systems I came across. It tests only the most basic arithmetic operations for integers and floating points besides memory bandwitdth.

![Image](screenshots/1.png)

It supports the following options:
```
SimpleBench - a quick benchmark to evaluate the core system performance
--show-gui [on/off]: enable/disable the text feedback during the tests
--st-test [on/off]: enable/disable the singlethread test
--mt-test [on/off]: enable/disable the multithread test
--threads [integer greater than zero]: sets the number of threads to run with
--hardware-level [integer greater than zero]: chooses how taxing the benchmark will be
      1: 32MB of RAM, 64KB of cache, lenght 1
      2: 64MB of RAM, 128KB of cache, lenght 4
      3: 128MB of RAM, 256KB of cache, lenght 16
      4: 512MB of RAM, 1MB of cache, lenght 64
      5: 2GB of RAM, 4MB of cache, lenght 256 (this is the current default)
      6: 8GB of RAM, 16MB of cache, lenght 1024 (HEDT users may want to use this one)
       :
      * every increase of 1 in the hardware level quadruples the requirements
      * there is some overhead in RAM requirement, you should have at least the amount required plus 5% FREE
--version: shows the benchmark version (not the program version)
--build: shows the program version, changes here have no effect on the score system
--help: shows this screen
```

SimpleBench is not a stress test and not is not a reliable benchmarking tool for any kind of advanced features the processor may have. 

Also, the algorithm is not complex so the compiler optimizations will basically skip all work it should have done. If you want to build from the sources, disable any optimizations to have a meaningful score. There are scripts to build with the correct compiler flags included in the source folder too.

These are the systems I benchmarked so far (I plan to include more systems in the future).

<table id="Scores 1.1">
    <tr> <!--Table header-->
        <th>Version</th>
        <th>Build</th>
        <th>Hardware Level</th>
        <th>ST Score</th>
        <th>MT Score</th>
        <th>Multiplier</th>
        <th>Architecture</th>
        <th>CPU</th>
        <th>RAM</th>
        <th>Operating System</th>
        <th>CLI Options</th>
    </tr>
    <tr> <!--Template-->
        <td></td><!--Version-->
        <td></td><!--Build-->
        <td></td><!--Hardware Level-->
        <td></td><!--ST Score-->
        <td></td><!--MT Score-->
        <td></td><!--Multiplier-->
        <td></td><!--Architecture-->
        <td></td><!--CPU-->
        <td></td><!--RAM-->
        <td></td><!--Operating System-->
        <td></td><!--CLI Options-->
    </tr>
</table>

## [1deterministic](https://github.com/1deterministic), 2019
