#ifndef H671141B2_B247_440B_8E83_476558110FFE
#define H671141B2_B247_440B_8E83_476558110FFE

class Uncloneable
{
protected:
    Uncloneable() {}
    ~Uncloneable(){}

private:
    Uncloneable(const Uncloneable&);
    Uncloneable& operator=(const Uncloneable&);
};


#define DISALLOW_COPY_AND_ASSIGN(ClassName)             \
private:                                                \
    ClassName(const ClassName&);                        \
    ClassName& operator=(const ClassName&);

#define DISALLOW_ASSIGN(ClassName)                      \
private:                                                \
    ClassName& operator=(const ClassName&);

#define DISALLOW_COPY(ClassName)                        \
private:                                                \
    ClassName(const ClassName&);                        \

#endif

