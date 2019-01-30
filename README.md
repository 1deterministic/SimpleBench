# SimpleBench

SimpleBench is a quick benchmarking tool I wrote to evaluate the performance of the systems I came across. It tests only the most basic arithmetic operations for integers and floating points besides memory bandwitdth.

![Image](screenshots/1.png)

It supports the following options:
```
--show-gui [on/off]: enable/disable the text feedback during the tests
--st-test [on/off]: enable/disable the singlethread test
--mt-test [on/off]: enable/disable the multithread test
--threads [integer greater than zero]: sets the number of threads to run with
--old-hardware [on/off]: enable/disable a configuration better suited to old hardware
--version: shows the benchmark version (not the program version)
--help: shows this screen
```

SimpleBench is not a stress test and not is not a reliable benchmarking tool for any kind of advanced features the processor may have. 

Also, the algorithm is not complex so the compiler optimizations will basically skip all work it should have done. If you want to build from the sources, disable any optimizations to have a meaningful score. There are scripts to build with the correct compiler flags included in the source folder too.

These are the systems I benchmarked so far (I plan to include more systems in the future).

<table id="Scores 1.0">
    <tr> <!--Table header-->
        <th>Version</th>
        <th>ST Score</th>
        <th>MT Score</th>
        <th>Multiplier</th>
        <th>Architecture</th>
        <th>CPU Vendor</th>
        <th>CPU Model</th>
        <th>CPU Base Clock</th>
        <th>CPU Boost Clock</th>
        <th>CPU Cores</th>
        <th>CPU Threads</th>
        <th>CPU TDP</th>
        <th>CPU LLC</th>
        <th>RAM Vendor</th>
        <th>RAM Model</th>
        <th>RAM Clock</th>
        <th>RAM Channels</th>
        <th>RAM Capacity</th>
        <th>RAM Latency</th>
        <th>Operating System</th>
        <th>OS Update</th>
        <th>Kernel</th>
        <th>CLI Options</th>
        <th>Build</th>
    </tr>
    <tr> <!--Intel Xeon E3 1280-->
        <td>1.0</td><!--Version-->
        <td>99.47</td><!--ST Score-->
        <td>394.04</td><!--MT Score-->
        <td>3.96</td><!--Multiplier-->
        <td>AMD64</td><!--Architecture-->
        <td>Intel</td><!--CPU Vendor-->
        <td>Xeon E3 1280</td><!--CPU Model-->
        <td>3600</td><!--CPU Base Clock-->
        <td>3900</td><!--CPU Boost Clock-->
        <td>4</td><!--CPU Cores-->
        <td>8</td><!--CPU Threads-->
        <td>95</td><!--CPU TDP-->
        <td>8192</td><!--CPU LLC-->
        <td>HyperX</td><!--RAM Vendor-->
        <td>Fury</td><!--RAM Model-->
        <td>1333</td><!--RAM Clock-->
        <td>1</td><!--RAM Channels-->
        <td>8192</td><!--RAM Capacity-->
        <td></td><!--RAM Latency-->
        <td>Arch Linux</td><!--Operating System-->
        <td>2019/01</td><!--OS Update-->
        <td>Linux 4.20</td><!--Kernel-->
        <td></td><!--CLI Options-->
        <td>generic</td><!--Build-->
    </tr> 
    <tr> <!--Intel Core i5 2410M-->
        <td>1.0</td><!--Version-->
        <td>75.19</td><!--ST Score-->
        <td>151.48</td><!--MT Score-->
        <td>2.01</td><!--Multiplier-->
        <td>AMD64</td><!--Architecture-->
        <td>Intel</td><!--CPU Vendor-->
        <td>Core i5 2410M</td><!--CPU Model-->
        <td>2300</td><!--CPU Base Clock-->
        <td>2900</td><!--CPU Boost Clock-->
        <td>2</td><!--CPU Cores-->
        <td>4</td><!--CPU Threads-->
        <td>35</td><!--CPU TDP-->
        <td>3072</td><!--CPU LLC-->
        <td>Kingston/Smart</td><!--RAM Vendor-->
        <td></td><!--RAM Model-->
        <td>1333</td><!--RAM Clock-->
        <td>2</td><!--RAM Channels-->
        <td>12288</td><!--RAM Capacity-->
        <td></td><!--RAM Latency-->
        <td>Arch Linux</td><!--Operating System-->
        <td>2019/01</td><!--OS Update-->
        <td>Linux 4.20</td><!--Kernel-->
        <td></td><!--CLI Options-->
        <td>generic</td><!--Build-->
    </tr>
    <tr> <!--VIA C7-->
        <td>1.0</td><!--Version-->
        <td>6.53</td><!--ST Score-->
        <td>6.53</td><!--MT Score-->
        <td>1.0</td><!--Multiplier-->
        <td>x86</td><!--Architecture-->
        <td>VIA</td><!--CPU Vendor-->
        <td>C7</td><!--CPU Model-->
        <td>1600</td><!--CPU Base Clock-->
        <td>1600</td><!--CPU Boost Clock-->
        <td>1</td><!--CPU Cores-->
        <td>1</td><!--CPU Threads-->
        <td>15</td><!--CPU TDP-->
        <td>128</td><!--CPU LLC-->
        <td>Kingston</td><!--RAM Vendor-->
        <td></td><!--RAM Model-->
        <td>667</td><!--RAM Clock-->
        <td>1</td><!--RAM Channels-->
        <td>2048</td><!--RAM Capacity-->
        <td></td><!--RAM Latency-->
        <td>Arch32</td><!--Operating System-->
        <td>2019/01</td><!--OS Update-->
        <td>Linux 4.20</td><!--Kernel-->
        <td></td><!--CLI Options-->
        <td>generic</td><!--Build-->
    </tr>
    <tr> <!--Template-->
        <td></td><!--Version-->
        <td></td><!--ST Score-->
        <td></td><!--MT Score-->
        <td></td><!--Multiplier-->
        <td></td><!--Architecture-->
        <td></td><!--CPU Vendor-->
        <td></td><!--CPU Model-->
        <td></td><!--CPU Base Clock-->
        <td></td><!--CPU Boost Clock-->
        <td></td><!--CPU Cores-->
        <td></td><!--CPU Threads-->
        <td></td><!--CPU TDP-->
        <td></td><!--CPU LLC-->
        <td></td><!--RAM Vendor-->
        <td></td><!--RAM Model-->
        <td></td><!--RAM Clock-->
        <td></td><!--RAM Channels-->
        <td></td><!--RAM Capacity-->
        <td></td><!--RAM Latency-->
        <td></td><!--Operating System-->
        <td></td><!--OS Update-->
        <td></td><!--Kernel-->
        <td></td><!--CLI Options-->
        <td></td><!--Build-->
    </tr>
</table>

## [1deterministic](https://github.com/1deterministic), 2019