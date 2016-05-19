#ifndef CCINFRA_MEMORY_SHARED_OBJECT_H
#define CCINFRA_MEMORY_SHARED_OBJECT_H

struct SharedObject
{
    SharedObject();
    virtual ~SharedObject();
    
    void addRef();
    void subRef();
    void setOnlyFree();
    virtual void destroy(){};
    virtual bool needDestroy(){return false;};

    unsigned int getRefCount() const;

    bool onlyThisRef() const;

private:
    unsigned int count;
};

#endif
