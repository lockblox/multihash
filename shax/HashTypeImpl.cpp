#include "HashTypeImpl.h"
#include <algorithm>
#include <sstream>

namespace shax
{

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

std::ostream& operator<<(std::ostream& os, const shax::HashType& hash_type)
{
    os << hash_type.name();
    return os;
}

std::ostream& operator<<(std::ostream& os, const shax::HashCode& hash_code)
{
    os << std::hex << static_cast<int>(hash_code);
    return os;
}

} // namespace shax
