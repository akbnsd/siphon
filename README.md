# `Siphon `

Siphon is a networking library for quick networking setup.
Currently siphon supports
- TCP / UDP   protocols
- IPV4 / IPV6 addressing
- Linux platform
  
siphon is currently under development.
With following Upcoming features
- Windows / Mac support
- OS routing table management



# How to buid the library?
Siphon uses cmake as a buildTool. Following packages are required preinstalled on your system.
- cmake
- make

###  using cmake
In the root source directory, run the following commands to build the library
``` 
mkdir build  
 ```

``` 
cd build 
 ```

``` 
cmake .. 
 ```

``` 
make -j4 
 ```

a static library named siphon will be generated.

# How to Integrate with cmake project?
- add siphon as a subdirectory in your project source
``` 
  git submodule add http://github.com/akbnsd/siphon.git ./siphon 
   ```
- add siphon as a cmake subdirectory in your root CMakeLists.txt
``` 
 add_subdirectory(siphon) 
  ```

link siphon with your main executable
``` 
 target_link_libraries(... ...  siphon) 
  ```
