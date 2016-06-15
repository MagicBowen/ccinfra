#ifndef H5088D5BD_7AEF_4B80_987D_2432554E45CA
#define H5088D5BD_7AEF_4B80_987D_2432554E45CA

#include <ccinfra/ccinfra.h>
#include <vector>
#include <string>

CCINFRA_NS_BEGIN

struct Args
{
    Args(const std::vector<std::string>&);
    ~Args();

    int argc() const;
    const char** argv() const;

private:
    size_t m_argc;
    const char** m_argv;
};

CCINFRA_NS_END

#endif
