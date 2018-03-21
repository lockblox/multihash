#include "HashTypeImpl.h"
#include <algorithm>
#include <stdexcept>

namespace shax
{

HashType::HashType() : HashType(HashCode::IDENTITY)
{
}

HashType::HashType(const std::string& name)
{
    auto all_types(Impl::all_types());
    auto end(std::end(all_types));
    auto it(std::find_if(std::begin(all_types), end,
                         [name](const HashType::Impl* elem) -> bool {
                             return name == elem->name();
                         }));
    if (end == it)
    {
        throw std::invalid_argument("Invalid hash name: " + name);
    }
    pImpl = *it;
}

HashType::HashType(HashCode code)
{
    auto all_types(Impl::all_types());
    auto end(std::end(all_types));
    auto it(std::find_if(std::begin(all_types), end,
                         [code](const HashType::Impl* elem) -> bool {
                             return code == elem->code();
                         }));
    if (end == it)
    {
        auto string_code = std::to_string(static_cast<int>(code));
        throw std::invalid_argument("Invalid hash code: " + string_code);
    }
    pImpl = *it;
}

HashType::HashType(const HashType& rhs) : pImpl(rhs.pImpl)
{
}

const std::string& HashType::name() const
{
    return pImpl->name();
}

HashCode HashType::code() const
{
    return pImpl->code();
}

size_t HashType::size() const
{
    return pImpl->size();
}

} // namespace shax
