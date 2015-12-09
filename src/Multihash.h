#pragma once

#include "Type.h"
#include <openssl/evp.h>

namespace multihash
{

class Hash::Impl
{
public:
    Impl(HashType type, Buffer digest)
        : type_(std::move(type)), digest_(std::move(digest))
    {
    }
    const HashType& type() const
    {
        return type_;
    }
    const Buffer& digest() const
    {
        return digest_;
    }

private:
    const HashType type_;
    const Buffer digest_;
};

struct Algorithm
{
    virtual int block_size() = 0;
    virtual void update(const Buffer& data) = 0;
    virtual Buffer digest() = 0;
    virtual void reset() = 0;
};

class HashFunction::Impl
{
public:
    explicit Impl(const HashType& hash_type);
    ~Impl() = default;
    const HashType& type() const
    {
        return hash_type_;
    }
    Hash operator()(std::istream& input);

private:
    HashType hash_type_;
    std::unique_ptr<Algorithm> algorithm_;
};

struct BufferEncoder::Impl
{
    Buffer encode(const Hash& hash);
};

struct BufferDecoder::Impl
{
    Hash decode(const Buffer& raw_bytes);
};

class SslImpl : public Algorithm
{
public:
    explicit SslImpl(const HashType& hash_type);
    ~SslImpl();
    int block_size() override;
    void update(const Buffer& data) override;
    Buffer digest() override;
    void reset() override;
    void init();

private:
    const EVP_MD* evpmd(HashCode code) const;
    const EVP_MD* evpmd_;
    EVP_MD_CTX* mdctx_;
};

} // namespace multihash
