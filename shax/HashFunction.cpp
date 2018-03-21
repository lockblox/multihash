#include "HashFunctionImpl.h"
#include <shax/HashFunction.h>

namespace shax
{

HashFunction::HashFunction(const HashType& hash_type)
    : pImpl(new Impl(hash_type))
{
}

HashFunction::HashFunction(HashCode code) : pImpl(new Impl(HashType(code)))
{
}

HashFunction::HashFunction(const std::string& hash_type)
    : pImpl(new Impl(HashType(hash_type)))
{
}

HashFunction::HashFunction(const HashFunction& other)
    : pImpl(new Impl(other.type()))
{
}

HashFunction& HashFunction::operator=(const HashFunction& rhs)
{
    if (!(type() == rhs.type()))
    {
        pImpl.reset(new Impl(HashType(rhs.type())));
    }
    return *this;
}

/** Though the destructor is trivial, we must define it separately since the
 * default destructor of unique_ptr would require a complete declaration of
 * Impl
 */
HashFunction::~HashFunction()
{
}

Hash HashFunction::operator()(std::istream& input) const
{
    return (*pImpl)(input);
}

Hash HashFunction::operator()(const string_view input) const
{
    return (*pImpl)(input);
}

const HashType& HashFunction::type() const
{
    return pImpl->type();
}

} // namespace shax
