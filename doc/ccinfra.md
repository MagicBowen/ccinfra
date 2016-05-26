# ccinfra

***

## Introduction

ccinfra是一套`C++`的基础库，目的是降低嵌入式下使用`C++`进行开发的难度。基于ccinfra可以低成本地写出漂亮、健壮的C++代码。虽然ccinfra中的很多元素是为了解决嵌入式下的问题，但是ccinfra的使用绝不仅限于嵌入式，它的很多元素是通用的，可以根据你的场景选择使用ccinfra的不同部分来帮助你。 本文后面每讲到一个组件，都会说明其适用场景，你可以根据自己的需求酌情选择。

## Install

ccinfra的安装是简单的。ccinfra被设计为以头文件和静态库的形式进行发布和使用。只用编译ccinfra的源码，得到“libccinfra.a”文件，然后将其和“ccinfra/include/ccinfra”目录拷贝到你的项目对应路径下，就可以使用了。当然如果你有多个项目依赖ccinfra，你可以将头文件和库拷贝到公共的路径下，配置每个项目的编译依赖，让其依赖同一份ccinfra。

ccinfra的编译依赖[cmake](https://cmake.org/)，确保你的机器已经安装好cmake。

在你的shell中执行以下命令获取及编译ccinfra的源码。

~~~bash
git clone git@github.com:MagicBowen/ccinfra.git
cd ccinfra
mkdir build
cd build
cmake ..
make
~~~

如果编译通过，会产生 “ccinfra/build/src/libccinfra.a”， 这就是你应该依赖的ccinfra静态库。你最好将ccinfra的头文件和libccinfra.a拷贝到一个公共的地方，让所有项目都可以方便的依赖。例如在“/home/shared"下创建ccinfra目录，将ccinfra源码下的incude目录拷贝到ccinfra目录下，然后在”home/shared/ccinfra“目录下创建lib目录，将libccinfra.a拷贝进去。最后，目录结构如下： ”/home/shared/ccinfra/include/ccinfra“， ”/home/shared/ccinfra/lib/libccinfra.a“。
如果你的项目需要依赖ccinfra，修改你项目的构建脚本，在头文件依赖中增加 ”/home/shared/ccinfra/include“， 在链接依赖中增加静态库目录 ”/home/shared/ccinfra/lib“， 并让其和ccinfra进行链接。
如果简单的话可以在build目录下执行`sudo make install`，这样ccinfra的头文件和库，将会自动安装到你系统默认的路径下，对于linux可能会是 “/usr/local/include” 以及 “/usr/local/lib” 目录。

可以尝试运行ccinfra的测试，看看ccinfra在你的系统下是否可以存在错误。
ccinfra的测试需要依赖[gtest](https://github.com/google/googletest)，下载gtest的源码，编译出gtest的库后，手动进行gtest的安装（最新的gtest不支持自动安装）。选择一个目录，例如“/home/shared”下，创建一个gtest目录，将gtest源码下的include目录拷贝进去。在“/home/shared/gtest”目录下新建lib目录，将编译好的gtest库拷贝进去。最后目录结构如下：”/home/shared/gtest/include/gtest"，"/home/shared/gtest/lib/libgtest.a"。

安装好gtest后，在ccinfra的源码目录下执行：

~~~bash
mkdir build
cd build
cmake -DGTEST_ROOT="/home/shared/gtest" ..
make
./test/ccinfra-test
~~~

如果所有测试运行通过，则说明ccinfra测试一切正常。
在ubuntu14.04系统下，gcc版本4.8.4，ccinfra测试一切OK。别的linux系统理论上应该可以正常运行ccinfra，windows和mac系统未经测试。另外，ccinfra的HashMap和对其的测试需要`C++11`特性的支持，确保你的编译选项中开启了`C++11`.

## Usage

### Base

### Core

### DCI

### Memory

### Container

### Others

## Finally