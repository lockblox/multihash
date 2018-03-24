#pragma once
#include <shax/Algorithm.h>
#include <cryptopp/cryptlib.h>
#include <shax/HashType.h>
#include <memory>

namespace shax
{

class CryptoppImpl : public Algorithm
{
public:
    explicit CryptoppImpl(const HashType& hash_type);
    size_t block_size() override;
    void update(const string_view& data) override;
    std::vector<char> digest() override;

private:
    std::unique_ptr<CryptoPP::HashTransformation> hash_;
    std::vector<char> digest_;
};

} // namespace shax
