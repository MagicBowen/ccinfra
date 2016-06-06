# ccinfra

***

## Introduction

`ccinfra` is a `C++` infrastructure library for writting better and effective `C++` codes! It includes below main features:
- Some grammer sugar that wrappered `C++` keywords for a better cross-platform C++ programming style.
- A [DCI](https://en.wikipedia.org/wiki/Data,_context_and_interaction) programming frame for implementing composed programming in C++ easily!
- Memory utils. For example: memory allocator,  AutoMsg, TransData ...
- Data structures (eg. array, list, map) for self memory government demands! It's useful for embeded programming.
- Some algorithm for bits operator, loop, bound...
- Design pattern, such as Singleton and State pattern.
- Concurrency tools, such as thread pool, lock utils... 
- Log, an implementation of log for ccinfra.

To learn more, visit the doc folder! View the source code of tests of ccinfra in the test folder for learning how to use every library elements in details!

## Install and Test

- Get ccinfra

~~~ bash
git clone git@github.com:MagicBowen/ccinfra.git
~~~

- Build ccinfra

~~~ bash
cd ccinfra
mkdir build
cd build
cmake ..
make
~~~

- Install ccinfra

~~~ bash
sudo make install
~~~

You can also install ccinfra manually, just copy "ccinfra/include" and "ccinfra/build/src/libccinfra.a" to your special installation path.

- Test ccinfra

To test ccinfra, you need install [gtest](https://github.com/google/googletest). Gtest do not support auto installation now, so you need compile gtest, and manually put the include folder and lib like below:

> 1. Download gtest source code, and compile to generate the gtest library.
> 2. Select a path for manually installation of gtest. For example choose to manually install gtest to folder "/home/shared", enter "/home/shared" and create a folder named "gtest";
> 3. Copy the "gtest/incude" to "/home/shared/gtest", the folder will like "/home/shared/gtest/include/gtest";
> 4. Copy the "libgtest.a" which you compiled out before to "/home/shared/gtest/lib"
> 5. Now the path "/home/shared/gtest" will be your gtest root path.

~~~ bash
cd build
cmake -DGTEST_ROOT="/home/shared/gtest" ..
make
./test/ccinfra-test
~~~

The "~/project/gtest" should be replaced to your gtest installed path.

## Finally

The ccinfra now only be supported on Linux platform, all the tests run passed on Unbuntu14.04. Other OS such as Windows or Mac has not been tested! Some features in ccinfra such as "sched" which including thread need C++11 suport, therefor compile all the components of ccinfra should enable the C++11 flag of you compiler!  
