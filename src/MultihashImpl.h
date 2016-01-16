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
    virtual ~Algorithm()
    {
    }
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
};

class BufferEncoder::Impl
{
public:
    Buffer encode(const Hash& hash);
};

class BufferDecoder::Impl
{
public:
    Hash decode(const Buffer& raw_bytes);
};

class SslImpl : public Algorithm
{
public:
    struct Cleanup
    {
        ~Cleanup();
    };
    class Context
    {
    public:
        Context();
        Context(const Context& rhs);
        Context(Context&& rhs);
        Context& operator=(Context rhs);
        ~Context();

        void swap(Context& rhs);
        EVP_MD_CTX* get() const;

    private:
        EVP_MD_CTX* md_ctx_;
    };

    class DigestType
    {
    public:
        explicit DigestType(const HashType& hash_type);
        DigestType(const DigestType& rhs) = default;
        DigestType& operator=(const DigestType& rhs) = default;

        const EVP_MD* get() const;
        int digest_size() const;
        int block_size() const;

    private:
        const EVP_MD* evp_md_;
    };

    explicit SslImpl(const HashType& hash_type);
    ~SslImpl();
    int block_size() override;
    void update(const Buffer& data) override;
    Buffer digest() override;

private:
    Context context_;
    const DigestType type_;
    static Cleanup cleanup_;
};

} // namespace multihash
