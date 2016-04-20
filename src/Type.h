#pragma once

#include <multihash/multihash.h>

namespace multihash
{

class HashType::Impl
{
public:
    Impl(HashCode code, std::string name, size_t size);
    ~Impl() = default;
    const std::string& name() const
    {
        return name_;
    }
    HashCode code() const
    {
        return code_;
    }
    size_t size() const
    {
        return size_;
    }

    bool operator<(const Impl& rhs) const;

    typedef std::set<const Impl*> Set;
    static const Set all_types();

private:
    const static Impl sha1_;
    const static Impl sha2_256_;
    const static Impl sha2_512_;

    const HashCode code_;
    const std::string name_;
    const size_t size_;
};

} // namespace multihash
