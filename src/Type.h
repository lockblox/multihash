#pragma once

#include <hashix/hashix.h>

namespace x
{

namespace hashi
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
    const static Impl identity;
    const static Impl sha1;
    const static Impl sha2_256;
    const static Impl sha2_512;

    const HashCode code_;
    const std::string name_;
    const size_t size_;
};

} // namespace hashi

} // namespace x
