#include "Type.h"
#include <algorithm>
#include <sstream>

namespace x
{

namespace hashi
{

InvalidHashException::InvalidHashException(const std::string& message)
    : Exception(message)
{
}

HashType::Impl::Impl(HashCode code, std::string name, size_t size)
    : code_(code), name_(std::move(name)), size_(size)
{
}

const HashType::Impl HashType::Impl::identity =
    HashType::Impl(HashCode::IDENTITY, "identity", 0);
const HashType::Impl HashType::Impl::sha1 =
    HashType::Impl(HashCode::SHA1, "sha1", 20);
const HashType::Impl HashType::Impl::sha2_256 =
    HashType::Impl(HashCode::SHA2_256, "sha2-256", 32);
const HashType::Impl HashType::Impl::sha2_512 =
    HashType::Impl(HashCode::SHA2_512, "sha2-512", 64);

namespace
{
} // anonymous namespace

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
        std::ostringstream err;
        err << "Invalid hash name: " << name;
        throw InvalidHashException(err.str());
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
        std::ostringstream err;
        err << "Invalid hash code: " << static_cast<int>(code);
        throw InvalidHashException(err.str());
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

const HashType::Impl::Set HashType::Impl::all_types()
{
    return HashType::Impl::Set{&HashType::Impl::identity,
                               &HashType::Impl::sha1,
                               &HashType::Impl::sha2_256,
                               &HashType::Impl::sha2_512};
}

bool HashType::Impl::operator<(const HashType::Impl& rhs) const
{
    return code() < rhs.code();
}

bool HashType::operator==(const HashType& rhs) const
{
    return pImpl == rhs.pImpl;
}

bool HashType::operator!=(const HashType& rhs) const
{
    return pImpl != rhs.pImpl;
}

bool HashType::operator<(const HashType& rhs) const
{
    return code() < rhs.code();
}

std::set<HashType> HashType::types()
{
    return std::set<HashType>{HashType{HashCode::SHA1},
                              HashType{HashCode::SHA2_256},
                              HashType{HashCode::SHA2_512}};
}

} // namespace hashi

} // namespace x

std::ostream& operator<<(std::ostream& os, const x::hashi::HashType& hash_type)
{
    os << hash_type.name();
    return os;
}

std::ostream& operator<<(std::ostream& os, const x::hashi::HashCode& hash_code)
{
    os << std::hex << static_cast<int>(hash_code);
    return os;
}
