#include "Type.h"
#include <algorithm>
#include <sstream>

namespace multihash
{

InvalidHashException::InvalidHashException(const std::string& message)
    : Exception(message)
{
}

HashType::Impl::Impl(HashCode code, std::string name, size_t size)
    : code_(code), name_(std::move(name)), size_(size)
{
}

const HashType::Impl::ShpSet HashType::Impl::all_types_{
    HashType::Impl::Shp{new HashType::Impl(HashCode::SHA1, "sha1", 20)},
    HashType::Impl::Shp{new HashType::Impl(HashCode::SHA2_256, "sha2-256", 32)},
    HashType::Impl::Shp{new HashType::Impl(HashCode::SHA2_512, "sha2-512", 64)},
    HashType::Impl::Shp{new HashType::Impl(HashCode::SHA3, "sha3", 64)},
    HashType::Impl::Shp{new HashType::Impl(HashCode::BLAKE2B, "blake2b", 64)},
    HashType::Impl::Shp{new HashType::Impl(HashCode::BLAKE2S, "blake2s", 32)}};

namespace
{
std::set<HashType> all_hash_types{HashType{HashCode::SHA1},
                                  HashType{HashCode::SHA2_256},
                                  HashType{HashCode::SHA2_512},
                                  HashType{HashCode::SHA3},
                                  HashType{HashCode::BLAKE2B},
                                  HashType{HashCode::BLAKE2S}};
} // anonymous namespace

HashType::HashType(const std::string& name)
{
    auto all_types(Impl::all_types());
    auto end(std::end(all_types));
    auto it(std::find_if(std::begin(all_types), end,
                         [name](const HashType::Impl::Shp& elem) -> bool {
                             return elem && (name == elem->name());
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
                         [code](const HashType::Impl::Shp& elem) -> bool {
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

HashType::HashType(HashType&& rhs) noexcept : pImpl(std::move(rhs.pImpl))
{
    rhs.pImpl = nullptr;
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

const HashType::Impl::ShpSet& HashType::Impl::all_types()
{
    return all_types_;
}

bool HashType::operator==(const HashType& rhs) const
{
    return pImpl == rhs.pImpl;
}

bool HashType::operator<(const HashType& rhs) const
{
    return code() < rhs.code();
}

std::set<HashType> hashTypes()
{
    return all_hash_types;
}

} // namespace multihash

std::ostream& operator<<(std::ostream& os, const multihash::HashType& hash_type)
{
    os << hash_type.name();
    return os;
}

std::ostream& operator<<(std::ostream& os, const multihash::HashCode& hash_code)
{
    os << std::hex << static_cast<int>(hash_code);
    return os;
}
