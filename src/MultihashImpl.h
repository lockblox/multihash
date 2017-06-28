#pragma once

#include "Type.h"
#include <openssl/evp.h>

namespace x
{

namespace hashi
{

class Hash::Impl
{
public:
    Impl(HashType type, Bytes digest)
        : type_(std::move(type)), digest_(std::move(digest))
    {
    }
    const HashType& type() const
    {
        return type_;
    }
    const Bytes& digest() const
    {
        return digest_;
    }

private:
    const HashType type_;
    const Bytes digest_;
};

struct Algorithm
{
    virtual size_t block_size() = 0;
    virtual void init() = 0;
    virtual void update(const string_view data) = 0;
    virtual Bytes digest() = 0;
    virtual ~Algorithm() = default;
};

class HashFunction::Impl
{
public:
    explicit Impl(HashType hash_type);
    ~Impl() = default;
    const HashType& type() const
    {
        return hash_type_;
    }
    Hash operator()(std::istream& input) const;
    Hash operator()(const string_view input) const;

private:
    HashType hash_type_;
    std::unique_ptr<Algorithm> algorithm_;
};

class HashBytesCodec::Impl
{
public:
    Bytes encode(const Hash& hash) const;
    Hash decode(const Bytes& raw_bytes) const;
    static Impl* instance();

private:
    static Impl instance_;
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

    void init() override;
    explicit SslImpl(const HashType& hash_type);
    ~SslImpl() override = default;
    size_t block_size() override;
    void update(const string_view data) override;
    Bytes digest() override;

private:
    Context context_;
    const DigestType type_;
    static Cleanup cleanup_;
};

} // namespace hashi

} // namespace x
