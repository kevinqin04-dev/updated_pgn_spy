Currently compiling using the c++ variant of gcc, g++, with the following command:
`g++ -o main ./analysis/* ./imports/* ./reports/* main.cpp`
> and running `./main.exe` combined with command line arguments
Alternative compilation to include `tests/unit-tests.cpp` will be a future endeavor

A true CI/CD pipeline is possible, but unlikely to be more effective than having a couple of copied commands or a saved bash script