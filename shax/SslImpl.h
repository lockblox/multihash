#pragma once

#include <openssl/evp.h>
#include <shax/Algorithm.h>
#include <shax/HashType.h>

namespace shax
{

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
    void update(const string_view& data) override;
    std::vector<char> digest() override;

private:
    Context context_;
    const DigestType type_;
    static Cleanup cleanup_;
};

} // namespace shax
