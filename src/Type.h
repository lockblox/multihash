#pragma once

#include <multihash/multihash.h>

namespace multihash
{

class HashType::Impl
{
public:
    Impl(HashCode code, std::string name, size_t size);
    ~Impl() = default;
    const std::string& name() const { return name_; }
    HashCode code() const { return code_; }
    size_t size() const { return size_; }

    typedef std::shared_ptr<HashType::Impl> Shp;
    typedef std::set<Shp> ShpSet;
    static const ShpSet& all_types();
private:
    const static ShpSet all_types_;

    const HashCode code_;
    const std::string name_;
    const size_t size_;
};

} // namespace multihash
