# ccinfra

***

## Introduction

ccinfra是一套C\++的基础库，目的是降低嵌入式下使用C\++进行开发的难度。基于ccinfra可以低成本地写出漂亮、健壮的C++代码。虽然ccinfra中的很多元素是为了解决嵌入式下的问题，但是ccinfra的使用绝不仅限于嵌入式，它的很多元素是通用的，可以根据你的场景选择使用ccinfra的不同部分来帮助你。

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
- Base ： 提供基本类型封装，状态码以及各种断言机制。另外利用宏扩展C\++的关键字，提供语法糖。有些关键字后来C\++11已经支持，但是ccinfra将其进行了封装，使得使用的时候无需区分当前编译器版本是否支持某一C\++11特性。
	适用场景： 广泛适用

- DCI ： 基于C\++的[DCI](https://en.wikipedia.org/wiki/Data,_context_and_interaction)框架。利用其可以低成本的在C++中实现组合式编程以支持DCI架构。同时里面包含一个可以替代RTTI的机制，可以在许多不能打开RTTI特性的场合作为替代手段。
	适用场景： 广泛适用

- Memory ： 提供自定义内存管理的一些组件。包括针对不同场景的内存分配器、针对嵌入式内存特征的智能指针、单例和一些辅助工具。
	适用场景： 嵌入式开发，或者需要自管理内存的场景

- Container ： 提供适合嵌入式内存特征需求的Array、List、HashMap和一些辅助类。
	适用场景： 嵌入式开发，或者对容器内存布局需要自管理的场景

- Algorithm ： 一些循环、排序算法的封装。
	适用场景： 广泛适用

- Concurrency ： 提供嵌入式下多线程编程可以使用的一些辅助类。
	适用场景： 广泛适用

- Log ： 封装了一套简单的Log机制，作为ccinfra的默认log机制。
	适用场景： 广泛适用

- Other ： ccinfra还提供了一些辅助的宏以及对函数式编程的辅助粹取类。
	适用场景： 广泛适用

上述组件中，Base、DCI、Memory、Container相对比较完整，其余各个组件还在不断完善中...

下面针对一些主要的组件进行用法介绍，可以通过阅读针对每个组件的测试用例了解到更多的细节。未介绍到的组件请自行阅读测试用例或源码。

### Base

#### 基本类型和状态码

首先在"ccinfra/base/BaseTypes.h"中对基本类型进行了封装，提供了 `U8`，`U16`，`U32`以及`U64`这几种常用的无符号整形，以及`S8`，`S16`，`S32`以及`S64`等几种常用的有符号整形。最后提供了一个针对指针的类型`PTR_VALUE`，用于将地址转为一个无符号整形，经常用在对地址的值进行比较的场合。`PTR_VALUE`会根据当前系统是32位还是64位，自动决定将地址存为`U32`还是`U64`。

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

"ccinfra/base/Status.h"定义了ccinfra的基本成功和错误码：`CCINFRA_SUCCESS`和`CCINFRA_FAILURE`，另外提供了两个辅助宏：

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

#### 断言

"ccinfra/base/Assertions.h"提供了各种断言机制。这里的断言都是运行时断言，分为两大类，expect和assert。expect类型的在断言失败后，会用`WARN_LOG`进行输出，而assert类型断言失败后则会用`ERR_LOG`进行错误输出。对于两种类型断言都支持三大类行判断： 1）对bool是否为true进行判断； 2）对指针有效性进行判断； 3）对函数返回值是否成功进行判断。每一类断言失败后的返回值都支持返回void，bool，错误码， 或者自定义返回值。具体见`Assertions.h`文件实现。 断言的使用非常简单，如下实例：

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

"ccinfra/base/static_assert.h"提供了静态断言`STATIC_ASSERT`。在支持C++11的场景下，`STATIC_ASSERT`被映射成关键字`static_assert`，否则被映射成自定义实现。

~~~cpp
// static_assert.h
#if __SUPPORT_STATIC_ASSERT
# define STATIC_ASSERT(expr) static_assert(expr, #expr)
#else
# define STATIC_ASSERT(expr) do {               \
    enum { static_assert_failure = 1/(expr) };  \
} while(0)
#endif
~~~

用法如下：

~~~cpp
//HashMap.h
template < typename KEY
         , typename VALUE
         , size_t   ELEM_SIZE = 1024
         , size_t   HASH_SIZE = ELEM_SIZE
         , typename HASH_FN = HashFn<KEY, HASH_SIZE>
         , typename EQUAL_FN = EqualFn<KEY> >
struct HashMap
{
// ...
    HashMap() : num(0)
    {
        STATIC_ASSERT(ELEM_SIZE > 0);
        STATIC_ASSERT(HASH_SIZE > 0);
    }
// ...
};
~~~

#### 关键字与语法糖

ccinfra提供了`DEF_INTERFACE`、`ABSTRACT`、`OVERRIDE`、`DEFAULT`、`EXTENDS`、`IMPLEMENTS`等关键字封装，让C\++ 语言更有表达力。这些封装无关gcc的版本是否支持 C\++11，它会根据gcc的版本自动做适配。

~~~cpp
DEF_INTERFACE(Shape)
{
    ABSTRACT(bool isCentrosymmetry() const);
};

struct Circle : Shape
{
private:
    OVERRIDE(bool isCentrosymmetry() const)
    {
        return true;
    }
};

DEF_INTERFACE(Shape2D) EXTENDS(Shape)
{
    DEFAULT(int, getArea() const);
};

struct Rectangle : Shape2D
{
    Rectangle(int w, int h) : width(w), height(h)
    {
    }

private:
    OVERRIDE(bool isCentrosymmetry() const)
    {
        return false;
    }

    OVERRIDE(int getArea() const)
    {
        return width * height;
    }

private:
    int width;
    int height;
};

struct Square : Shape2D
{
    Square(int l) : length(l)
    {
    }

private:
    OVERRIDE(bool isCentrosymmetry() const)
    {
        return true;
    }

    OVERRIDE(int getArea() const)
    {
        return length * length;
    }

private:
    int length;
};

struct Triangle : Shape2D
{
private:
    OVERRIDE(bool isCentrosymmetry() const)
    {
        return false;
    }
};

TEST(...)
{
    Shape2D *square = new Square(5);
    Shape2D *rectangle = new Rectangle(2, 3);
    Shape2D *triangle = new Triangle();

    ASSERT_EQ(25, square->getArea());
    ASSERT_EQ(6,  rectangle->getArea());
    ASSERT_EQ(0,  triangle->getArea());

    Shape *r = rectangle;
    Shape *c = new Circle();

    ASSERT_FALSE(r->isCentrosymmetry());
    ASSERT_TRUE(c->isCentrosymmetry());

    delete r;
    delete c;
    delete square;
    delete triangle;
}

~~~

从上例中可以清晰地看到这些扩展关键字的用法。

- `DEF_INTERFACE` : 定义一个接口。一个接口里面至少需要包含一个纯虚方法。对于C\++如果一个类包含虚接口，最佳实践告诉我们最好同时定义虚析构方法（参考《Effective C\++》。当用`DEF_INTERFACE`定义接口的时候，会自动为这个接口定义了一个默认的虚析构方法。这里默认的虚析构函数仅仅为了让编译器不产生告警，如果它不能满足你的需求，请手动定义虚析构函数的具体实现。

- `EXTENDS`：当需要用一个接口扩展一个基类接口的时候，用`EXTENDS`关键字。`EXTENDS`仅仅用在`DEF_INTERFACE`或者`DEFINE_ROLE`(见后面DCI章节介绍)定义的接口后面。

- `ABSTRACT`：定义一个抽象方法，对应C\++一个纯虚方法。如果一个方法定义为`ABSTRACT`，那么该类的所有子类必须实现该方法，可以用关键字`OVERRIDE`进行显示复写。

- `OVERRIDE`：复写一个虚方法。C\++11下，如果`OVERRIDE`中对应的方法原型在父类中找不到对应的虚方法声明，则会编译错误。

- `DEFAULT`：定义一个具有默认实现的虚方法。一旦一个方法被定义为`DEFAULT`，该方法在子类中可以不需要`OVERRIDE`。对于有返回值的函数，`DEFAULT`定义的默认实现中会自动返回默认值。对于bool类型返回false，对于数型和指针都返回0。如果默认实现不能满足你的要求，需要显示在子类中用`OVERRIDE`进行具体实现定义。

“ccinfra/base/NullPtr.h”中对空指针进行了封装，统一用`__null_ptr__`表示空地址。该定义在支持C\++11的场景下会映射为关键字`nullptr`，否则为数字0。同时提供对空指针进行判断的两个宏。

~~~cpp
#if __SUPPORT_NULL_PTR
#define __null_ptr__ nullptr
#else
#define __null_ptr__ 0
#endif

#define __notnull__(ptr) ptr != __null_ptr__
#define __null__(ptr) ptr == __null_ptr__

#endif
~~~

用法如下：

~~~cpp
// HashMaps.h
Status put(const KEY& key, const VALUE& value)
{
    const VALUE* v = get(key);

    if(__notnull__(v))
    {
        // ...
        return CCINFRA_SUCCESS;
    }

    return __null__(insert(key ,value)) ? CCINFRA_FAILURE : CCINFRA_SUCCESS;
}
~~~

"ccinfra/base/EqHelper.h"定义了一些辅助宏，用于方面类实现比较运算符。这些辅助宏在你定义了`==`的实现后，会自动扩展出`！=`的实现，在你定了`==`和`<`的实现后，会自动扩展出其它所有比较运算符的实现。具体用法如下：

~~~cpp
// Complex.h
struct Complex
{
    Complex(int r, int i) : real(r), image(i)
    {
    }

    __DECL_COMP(Complex);

private:
    int real;
    int image;
};
~~~

~~~cpp
// Complex.cpp
__DEF_EQUALS(Complex)
{
    return (real == rhs.real) && (image == rhs.image);
}

__DEF_COMP(Complex)
{
    if(real == rhs.real) return image < rhs.image;
    return real < rhs.real;
}
~~~

~~~cpp
//TestComplex.h
TEST(...)
{
    ASSERT_TRUE(Complex(3, 1) == Complex(3, 1));
    ASSERT_TRUE(Complex(3, 0) != Complex(3, 1));
    ASSERT_TRUE(Complex(3, 0) < Complex(3, 1));
    ASSERT_TRUE(Complex(2, 8) < Complex(3, 0));
    ASSERT_TRUE(Complex(2, 8) <= Complex(3, 0));
    ASSERT_TRUE(Complex(2, 8) > Complex(1, 10));
    ASSERT_TRUE(Complex(2, 8) >= Complex(2, 7));
    ASSERT_TRUE(Complex(2, 8) >= Complex(2, 8));
}
~~~

### DCI

[DCI](https://en.wikipedia.org/wiki/Data,_context_and_interaction)是一种面向对象软件架构模式，它可以让面向对象更好地对数据和行为之间的关系进行建模从而更容易被人理解。DCI目前广泛被作为对DDD（领域驱动开发）的一种发展和补充，用于基于面向对象的领域建模。DCI建议将软件的领域核心代码分为Context、Interactive和Data层。Context层用于处理由外部UI或者消息触发业务场景，每个场景都能找对一个对应的context，其作为理解系统如何处理业务流程的起点。Data层用来描述系统是什么（What the system is？），在该层中采用领域驱动开发中描述的建模技术，识别系统中应该有哪些领域对象以及这些对象的生命周期和关系。而DCI最大的发展则在于Interactive层，DCI认为应该显示地对领域对象在每个context中所扮演的角色`role`进行建模，role代表了领域对象服务于context时应该具有的业务行为。正是因为领域对象的业务行为只有在去服务于某一Context时才会具有意义，所以属于role的方法不应该强塞给领域对象，否则领域对象就会随着其支持的业务场景（context）越来越多而变成上帝类。DCI认为对role的建模应该是面向context的，但是role最终还是要操作数据，那么role和领域对象之间应该存在一种注入（cast）关系。当context被触发的时候，context串联起一系列的role进行交互完成一个特定的业务流程。Context同时需要决定在当前业务场景下每个role的扮演者（领域对象），在context中完成领域对象到role的注入或者cast。基于上述DCI的特点，DCI架构使得软件具有如下好处：

- 清晰的进行了分层使得软件更容易被理解。
	1. context是很薄的一层，每个context往往无状态，只是找到合适的role，让role交互起来完成业务逻辑即可。但是简单并不代表不重要，显示化context层正是为人去理解软件业务流程提供切入点和主线。
	2. Data层描述系统有哪些领域概念及其之间的关系，该层专注于领域对象和之间关系的确立，让程序员站在对象的角度思考系统，从而让系统是什么更容易被理解。
	3. Interactive层主要体现在对role的建模，role是每个context中复杂的业务逻辑的真正执行者。Role所做的是对行为进行建模，它联接了context和领域对象！由于系统的行为是复杂且多变的，role使得系统将稳定的领域模型层和多变的系统行为层进行了分离，由role专注于对系统行为进行建模。该层往往关注于系统的可扩展性，更加贴近于软件工程实践，在面向对象中更多的是以类的视角进行设计。

- 显示的对role进行建模，解决了面向对象建模中充血和贫血模型之争。DCI通过显示的用role对行为进行建模，同时让role在context中可以和对应的领域对象进行绑定(cast)，从而既解决了数据边界和行为边界不一致的问题，也解决了领域对象中数据和行为高内聚低耦合的问题。

> 面向对象建模面临的一个棘手问题是数据边界和行为边界往往不一致。遵循模块化的思想，我们通过类将行为和其紧密耦合的数据封装在一起。但是在复杂的业务场景下，行为往往跨越多个领域对象，这样的行为放在某一个对象中必然导致别的对象需要向该对象暴漏其内部状态。所以面向对象发展的后来，领域建模出现两种派别之争，一种倾向于将跨越多个领域对象的行为建模在所谓的service中（见DDD中所描述的service建模元素）。这种做法使用过度经常导致领域对象变成只提供一堆get方法的哑对象，这种建模导致的结果被称之为贫血模型。而另一派则坚定的认为方法应该属于领域对象，所以所有的业务行为仍然被放在领域对象中，这样导致领域对象随着支持的业务场景变多而变成上帝类，而且类内部方法的抽象层次很难一致。另外由于行为边界很难恰当，导致对象之间数据访问关系也比较复杂。这种建模导致的结果被称之为充血模型。

在DCI架构中，如何将role和领域对象进行绑定，根据语言特点做法不同。对于动态语言，可以在运行时进行绑定。而对于静态语言，领域对象和role的关系在编译阶段就得确定。对于C++，DCI的论文[www.artima.com/articles/dci_vision.html](www.artima.com/articles/dci_vision.html)中介绍了采用模板Trait的技巧。但是正如我们前面所讲，role主要对复杂多变的业务行为进行建模，所以role需要更加关注于系统的可扩展性，更加贴近软件工程，对role的建模会更多地站在类的视角。由于在复杂场景下会出现复杂的行为依赖和交织，如果采用模板技术会产生复杂的模板交织代码从而让工程层面变得过于复杂，而面向对象的多态和依赖注入则可以相对更轻松地解决此类问题。另外，由于一个领域对象可能会在不同的context下扮演多种角色，这时领域对象要能够组合进来不同的role。对于所有这些问题，ccinfra提供的DCI框架采用了多重继承来进行领域对象和其支持的role之间的关系描述，采用了在多重继承树内进行关系交织来进行role之间的依赖关系描述。

对于DCI的理论介绍，以及如何利用DCI框架进行领域建模，本文就介绍这些。后面主要介绍如何利用ccinfra中的DCI框架实现和拼装role以完成一种组合式编程。

下面假设一种场景：模拟人和机器人工作制造产品。人制造产品会消耗吃饭得到的能量，缺乏能量后需要再吃饭补充；而机器人制造产品会消耗电能，缺乏能量后需要再充电。这里人和机器人在工作时都是一名worker（扮演的角色），工作的流程是一样的，但是区别在于依赖的能量消耗和获取方式不同。

~~~cpp
DEFINE_ROLE(EnergyCarrier)
{
    ABSTRACT(void consume());
    ABSTRACT(bool isExhausted() const);
};

struct HumanEnergy : EnergyCarrier
{
    HumanEnergy()
    : isHungry(false), consumeTimes(0)
    {
    }

private:
    OVERRIDE(void consume())
    {
        consumeTimes++;

        if(consumeTimes >= MAX_CONSUME_TIME)
        {
            isHungry = true;
        }
    }

    OVERRIDE(bool isExhausted() const)
    {
        return isHungry;
    }

private:
	enum
    {
        MAX_CONSUME_TIME = 10,
    }；

    bool isHungry;
    U8 consumeTimes;
};

struct ChargeEnergy : EnergyCarrier
{
    ChargeEnergy() : percent(0)
    {
    }

    void charge()
    {
        percent = FULL_PERCENT;
    }

private:
    OVERRIDE(void consume())
    {
    	if(percent > 0)
            percent -= CONSUME_PERCENT;
    }

    OVERRIDE(bool isExhausted() const)
    {
        return percent == 0;
    }

private:
    enum
    {
        FULL_PERCENT = 100,
        CONSUME_PERCENT = 1
    };

    U8 percent;
};

DEFINE_ROLE(Worker)
{
    Worker() : produceNum(0)
    {
    }

    void produce()
    {
        if(ROLE(EnergyCarrier).isExhausted()) return;

        produceNum++;

        ROLE(EnergyCarrier).consume();
    }

    U32 getProduceNum() const
    {
        return produceNum;
    }

private:
    U32 produceNum;

private:
    USE_ROLE(EnergyCarrier);
};
~~~

上面代码中使用了DCI框架中三个主要的语法糖：
- `DEFINE_ROLE`：用于定义role，一个role在这里就是一个普通的类。`DEFINE_ROLE`的实现和前面介绍的`DEF_INTERFACE`一模一样，但是在DCI框架里面使用这个命名更具有语义。`DEFINE_ROLE`定义的类中需要至少包含一个虚方法或者使用了`USE_ROLE`声明依赖另外一个ROLE。
- `USE_ROLE`：在一个类里面声明自己的实现依赖另外一个ROLE。
- `ROLE`：当一个类声明中使用了`USE_ROLE`声明依赖另外一个类XXX后，则在类的实现代码里面就可以调用 `ROLE(XXX)`来引用这个类去调用它的成员方法。

在上面的例子中，`Worker`类被定义为一个role，它依赖于一个抽象类`EnergyCarrier`。在它的实现中调用`ROLE(EnergyCarrier)`访问它提供的接口方法。`EnergyCarrier`有两个子类`HumanEnergy`和`ChargeEnergy`分别对应于人和机器人的能量特征。上面只是以类的形式定义了各种role，下面我们需要将role和领域对象关联并将role之间的依赖关系在领域对象内完成正确的交织后，软件才能工作。

~~~cpp
struct Human : Worker
             , private HumanEnergy
{
private:
    IMPL_ROLE(EnergyCarrier);
};

struct Robot : Worker
             , ChargeEnergy
{
private:
    IMPL_ROLE(EnergyCarrier);
};
~~~

通过上面的代码可以看到，实际上用多重继承完成了领域对象对role的组合。在上例中`Human`组合了`Worker`和`HumanEnergy`，而`Robot`组合了`Worker`和`ChargeEnergy`。最后在领域对象的类内还需要完成role之间的关系交织。由于`Worker`中声明了`USE_ROLE(EnergyCarrier)`，所以当`Human`和`Robot`继承了`Worker`之后就需要显示化`EnergyCarrier`从哪里来。有如下几种主要的交织方式：
- `IMPL_ROLE`： 对上例，如果`EnergyCarrier`的某一个子类也被继承的话，那么就直接在交织类中声明`IMPL_ROLE(EnergyCarrier)`。于是当`Worker`工作时所找到的`ROLE(EnergyCarrier)`就是在交织类中所继承的具体`EnergyCarrier`子类。
- `IMPL_ROLE_WITH_OBJ`： 当持有被依赖role的一个引用或者成员的时候，使用`IMPL_ROLE_WITH_OBJ`进行关系交织。例如假如上例中`Human`类中有一个成员是`HumanEnergy energy`那么就可以用`IMPL_ROLE_WITH_OBJ(EnergyCarrier, energy)`来声明交织关系。该场景同样适用于类内持有的是被依赖role的指针、引用的场景。
- `DECL_ROLE` ： 自定义交织关系。例如对上例在`Human`中定义一个方法`DECL_ROLE(EnergyCarrier){ // function implementation}`，自定义`EnergyCarrier`的来源，完成交织。

当正确完成role的依赖交织工作后，领域对象类就可以被实例化了。如果没有交织正确，一般会出现编译错误。

~~~cpp
TEST(...)
{
    Human human;
    SELF(human, Worker).produce();
    ASSERT_EQ(1, SELF(human, Worker).getProduceNum());

    Robot robot;
    SELF(robot, ChargeEnergy).charge();
    while(!SELF(robot, EnergyCarrier).isExhausted())
    {
        SELF(robot, Worker).produce();
    }
    ASSERT_EQ(100, SELF(robot, Worker).getProduceNum());
}
~~~

如上使用`SELF`将领域对象cast到对应的role上访问对应的方法。注意只有被public继承的role才可以从领域对象上cast过去。private继承的role往往是作为领域对象内部依赖的，不能从领域对象显示cast去使用（上例中`human`不能做`SELF(human, EnergyCarrier)`转换，会编译错误）。

### Memory

### Container

### Others

## Finally