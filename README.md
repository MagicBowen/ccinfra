# ccinfra

***

## Introduction

`ccinfra` is a `C++` infrastructure library for writting better `C++` codes! It includes below elements:
- Macros that wrappered `C++` keywords for a better cross-platform C++ programming style.
- A [DCI](https://en.wikipedia.org/wiki/Data,_context_and_interaction) programming frame for implementing composed programming in C++ easily!
- Memory utils. For example: memory allocator, Singleton , TransData, AutoMsg ...
- Data structures (eg. array, list, map) for self memory government demands! It's useful for embeded programming.
- Some algorithm and thread utils that fit for embeded C++ programming!

To learn more, visit the doc folder! View the source code of tests of ccinfra in the test folder for learning how to use every library elements in details!

## Test and Install

To test ccinfra, you need install [gtest](https://github.com/google/googletest). Gtest do not support auto installation now, so you need compile gtest, and manually put the include folder and lib like below:

> 1. Download gtest source code, and compile to generate the gtest library.
> 2. Select a path for manually installation of gtest. For example choose to manually install gtest to folder "/home/shared", enter "/home/shared" and create a folder named "gtest";
> 3. Copy the "gtest/incude" to "/home/shared/gtest", the folder will like "/home/shared/gtest/include/gtest";
> 4. Copy the "libgtest.a" which you compiled out before to "/home/shared/gtest/lib"
> 5. Now the path "/home/shared/gtest" will be your gtest root path.

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

~~~ bash
cd build
cmake -DGTEST_ROOT="/home/shared/gtest" ..
make
./test/ccinfra-test
~~~

The "~/project/gtest" should be replaced to your gtest installed path.

## Finally

The ccinfra now only be supported on Linux platform, all the tests run passed on Unbuntu14.04. Other OS such as Windows or Mac has not been tested!