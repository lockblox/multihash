#pragma once
#include <shax/Algorithm.h>
#include <shax/HashFunction.h>

namespace shax
{

class HashFunction::Impl
{
public:
    explicit Impl(HashType hash_type);
    ~Impl() = default;
    const HashType& type() const
    {
        return hash_type_;
    }
    Hash operator()(std::istream& input);
    Hash operator()(const string_view input);

private:
    void init();
    HashType hash_type_;
    std::unique_ptr<Algorithm> algorithm_;
};

} // namespace shax
