# ccinfra

***

## Introduction

ccinfra是一套`C++`的基础库，目的是降低嵌入式下使用`C++`进行开发的难度。基于ccinfra可以低成本地写出漂亮、健壮的C++代码。虽然ccinfra中的很多元素是为了解决嵌入式下的问题，但是ccinfra的使用绝不仅限于嵌入式，它的很多元素是通用的，可以根据你的场景选择使用ccinfra的不同部分来帮助你。 本文后面每讲到一个组件，都会说明其适用场景，你可以根据自己的需求酌情选择。

## Install

ccinfra的安装是简单的。ccinfra被设计为以头文件和静态库的形式进行发布和使用。只用编译ccinfra的源码，得到对应的静态库文件（例如linux系统下是“libccinfra.a”），然后将其和“ccinfra/include/ccinfra”目录拷贝到你的项目对应路径下，就可以使用了。当然如果你有多个项目依赖ccinfra，你可以将头文件和库拷贝到公共的路径下，配置每个项目的编译依赖，让其依赖同一份ccinfra。

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

以liux系统为例，如果编译通过，会产生 “ccinfra/build/src/libccinfra.a”， 这是需要依赖的ccinfra静态库。最好将ccinfra的头文件和libccinfra.a拷贝到一个公共的地方，让所有项目都可以方便的依赖。例如在“/home/shared"下创建ccinfra目录，将ccinfra源码下的incude目录拷贝到ccinfra目录下，然后在”home/shared/ccinfra“目录下创建lib目录，将libccinfra.a拷贝进去。最后，目录结构如下： ”/home/shared/ccinfra/include/ccinfra“， ”/home/shared/ccinfra/lib/libccinfra.a“。
如果某项目需要依赖ccinfra，修改该项目的构建脚本，在头文件依赖中增加 ”/home/shared/ccinfra/include“， 在链接依赖中增加静态库目录 ”/home/shared/ccinfra/lib“， 并让其和ccinfra进行链接。
如果简单的话可以在build目录下执行`sudo make install`，这样ccinfra的头文件和库，将会自动安装到当前系统默认的安装路径下，对于linux可能会是 “/usr/local/include” 以及 “/usr/local/lib” 目录。

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

在ubuntu14.04系统下，gcc版本4.8.4，ccinfra测试一切OK。别的linux系统理论上应该可以正常运行ccinfra，windows和mac系统未经测试。

## Usage

ccinfra由主要的几个组件组成：
- Base ： 提供基本类型封装，状态码以及各种断言机制。另外利用宏扩展`C++`的关键字，提供语法糖。有些关键字后来`C++11`已经支持，但是ccinfra将其进行了封装，使得使用的时候无需区分当前编译器是否支持某一`C++11`特性。
- DCI ： 基于`C++`的[DCI](https://en.wikipedia.org/wiki/Data,_context_and_interaction)框架。利用其可以低成本的在C++中实现组合式编程以支持DCI架构。同时里面包含一个可以替代RTTI的机制，可以在许多不能打开RTTI特性的场合作为替代手段。
- Memory ： 提供自定义内存管理的一些组件。包括针对不同场景的内存分配器、针对嵌入式内存特征的智能指针、单例和一些辅助工具。
- Container ： 提供适合嵌入式内存特征需求的Array、List、HashMap和一些辅助类。
- Algorithm ： 一些循环、排序算法的封装。
- Concurrency ： 提供嵌入式下多线程编程可以使用的一些辅助类。
- Log ： 封装了一套简单的Log机制，作为ccinfra的默认log机制。
- Other ： ccinfra还提供了一些辅助的宏以及对函数式编程的辅助粹取类。

上述组件中，Base、Core、DCI、Memory、Container相对比较完整，其余各个组件还在不断完善中...

下面针对一些主要的组件进行用法介绍，可以通过阅读针对每个组件的测试用例了解到更多的细节。未介绍到的组件请自行阅读测试用例或源码。

### Base

首先在`BaseTypes.h`中对基本类型进行了封装，提供了 `U8`，`U16`，`U32`以及`U64`这几种常用的无符号整形，以及`S8`，`S16`，`S32`以及`S64`等几种常用的有符号整形。最后提供了一个针对指针的类型`PTR_VALUE`，用于将地址转为一个无符号整形，经常用在对地址的值进行比较的场合。`PTR_VALUE`会根据当前系统是32位还是64位，自动决定将地址存为`U32`还是`U64`。

~~~cpp
// BaseTypes.h
template<bool IS_64_BIT> struct PtrValueTraits
{
    typedef U64 PtrValue;
};

template<> struct PtrValueTraits<false>
{
    typedef U32 PtrValue;
};

typedef typename PtrValueTraits<(sizeof(int*) > 4)>::PtrValue PTR_VALUE;
~~~

可以如下使用`PTR_VALUE`:

~~~cpp
// MsgAllocator.cpp
static bool tryFree(void *p)
{
    PTR_VALUE lower_bound = (PTR_VALUE)(&allocator);
    PTR_VALUE upper_bound = lower_bound + MEM_BLOCK_SIZE*BLOCK_NUM;
    return (lower_bound <= (PTR_VALUE)p) && ((PTR_VALUE)p < upper_bound);
}
~~~

`Status.h`定义了ccinfra的基本成功和错误码：`CCINFRA_SUCCESS`和`CCINFRA_FAILURE`，另外提供了两个辅助宏：

~~~cpp
// Status.h
#define __CCINFRA_FAILED(result)  (result != CCINFRA_SUCCESS)
#define __CCINFRA_SUCCESS(result)  (result == CCINFRA_SUCCESS)
~~~

利用这两个宏可以帮助对错误码进行判断：

~~~cpp
if(__CCINFRA_FAILED(map.put(2, 2)))
{
	// handle error
}
~~~

`Assertions.h`提供了各种断言机制。这里的断言都是运行时断言，分为两大类，expect和assert。expect类型的在断言失败后，会用`WARN_LOG`进行输出，而assert类型断言失败后则会用`ERR_LOG`进行错误输出。对于两种类型断言都支持三大类行判断： 1）对bool是否为true进行判断； 2）对指针有效性进行判断； 3）对函数返回值是否成功进行判断。每一类断言失败后的返回值都支持返回void，bool，错误码， 或者自定义返回值。具体见`Assertions.h`文件实现。 断言的使用非常简单，如下实例：

~~~cpp
// TransData.h
Status modify()
{
    CCINFRA_ASSERT_TRUE(ACTIVE == state);

    CCINFRA_ASSERT_SUCC_CALL(getCurrentValue().copyTo(getAnotherValue()));

    state = MODIFIED;

    return CCINFRA_SUCCESS;
}
~~~

### Core

### DCI

### Memory

### Container

### Others

## Finally