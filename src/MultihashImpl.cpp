#include "MultihashImpl.h"
#include <iomanip>
#include <iostream>
#include <mutex> //for call_once
#include <sstream>

namespace multihash
{

HashBytesCodec::Impl HashBytesCodec::Impl::instance_ = HashBytesCodec::Impl();

SslImpl::Cleanup SslImpl::cleanup_ = SslImpl::Cleanup();

Exception::Exception(std::string message) : message_(std::move(message))
{
}

const char* Exception::what() const noexcept
{
    return message_.c_str();
}

Hash::Hash() : pImpl(new Impl(HashType(), Bytes()))
{
}

Hash::Hash(HashType type, const Bytes& digest) : pImpl(new Impl(type, digest))
{
}

Hash::Hash(Hash&& other) noexcept : pImpl(std::move(other.pImpl))
{
}

Hash::Hash(const Hash& rhs) : pImpl(rhs.pImpl)
{
}

Hash& Hash::operator=(Hash&& rhs) noexcept
{
    pImpl = std::move(rhs.pImpl);
    return *this;
}

Hash::~Hash()
{
}

bool Hash::operator==(const Hash& rhs) const
{
    return type() == rhs.type() && digest() == rhs.digest();
}

bool Hash::operator<(const Hash& rhs) const
{
    auto result = false;
    if (type() == rhs.type())
    {
        result = digest() < rhs.digest();
    }
    else
    {
        result = type() < rhs.type();
    }
    return result;
}

const HashType& Hash::type() const
{
    return pImpl->type();
}

const Bytes& Hash::digest() const
{
    return pImpl->digest();
}

HashFunction::HashFunction(const HashType& hash_type)
    : pImpl(new Impl(hash_type))
{
}

HashFunction::HashFunction(HashCode code) : pImpl(new Impl(HashType(code)))
{
}

HashFunction::HashFunction(const std::string& hash_type)
    : pImpl(new Impl(HashType(hash_type)))
{
}

HashFunction::HashFunction(HashFunction&& other) noexcept
    : pImpl(std::move(other.pImpl))
{
}

HashFunction::HashFunction(const HashFunction& other)
    : pImpl(new Impl(other.type()))
{
}

HashFunction& HashFunction::operator=(HashFunction&& rhs) noexcept
{
    pImpl = std::move(rhs.pImpl);
    return *this;
}

HashFunction& HashFunction::operator=(const HashFunction& rhs)
{
    if (!(type() == rhs.type()))
    {
        pImpl.reset(new Impl(HashType(rhs.type())));
    }
    return *this;
}

/** Though the destructor is trivial, we must define it separately since the
 * default destructor of unique_ptr would require a complete declaration of
 * Impl
 */
HashFunction::~HashFunction()
{
}

Hash HashFunction::operator()(std::istream& input) const
{
    return (*pImpl)(input);
}

Hash HashFunction::operator()(const ArrayRef input) const
{
    return (*pImpl)(input);
}

const HashType& HashFunction::type() const
{
    return pImpl->type();
}

HashFunction::Impl::Impl(HashType hash_type)
    : hash_type_(std::move(hash_type)), algorithm_(nullptr)
{
    switch (hash_type_.code())
    {
        case HashCode::SHA1:
        case HashCode::SHA2_256:
        case HashCode::SHA2_512:
            algorithm_.reset(new SslImpl(hash_type_));
            break;
        case HashCode::SHA3:
        case HashCode::BLAKE2B:
        case HashCode::BLAKE2S:
        case HashCode::IDENTITY:
            throw Exception("No hash function for " + hash_type_.name());
    }
}

Hash HashFunction::Impl::operator()(std::istream& input) const
{
    if (!input.good())
    {
        throw Exception("HashFunction input is not good");
    }

    Bytes buffer(algorithm_->block_size());
    auto begin = buffer.begin();
    auto end = buffer.end();

    while (!input.eof())
    {
        input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

        if (!input.eof())
        {
            // filled the buffer so update the hash
            algorithm_->update(buffer);
        }
    }
    // final update to hash with partially filled vector
    auto chars_read(input.gcount());
    std::advance(begin, chars_read);
    buffer.erase(begin, end);
    algorithm_->update(buffer);
    return Hash(hash_type_, algorithm_->digest());
}

Hash HashFunction::Impl::operator()(const ArrayRef input) const
{
    auto block_size = algorithm_->block_size();
    auto begin = input.begin();
    auto size = std::min(input.size(), block_size);
    auto remaining = input.size();

    while (size > 0)
    {
        auto buffer = ArrayRef(begin, size);
        algorithm_->update(buffer);
        begin += size;
        remaining -= size;
        size = std::min(remaining, block_size);
    }
    return Hash(hash_type_, algorithm_->digest());
}

SslImpl::Cleanup::~Cleanup()
{
    EVP_cleanup();
}

SslImpl::Context::Context() : md_ctx_(EVP_MD_CTX_create())
{
}

SslImpl::Context::Context(const Context& rhs) : md_ctx_(EVP_MD_CTX_create())
{
    if (EVP_MD_CTX_copy(md_ctx_, rhs.get()) != 1)
    {
        EVP_MD_CTX_destroy(md_ctx_);
        throw Exception("Unable to copy hash context");
    }
}

SslImpl::Context::Context(Context&& rhs) noexcept : md_ctx_(rhs.md_ctx_)
{
    rhs.md_ctx_ = nullptr;
}

SslImpl::Context& SslImpl::Context::operator=(Context rhs)
{
    this->swap(rhs);
    return *this;
}

SslImpl::Context::~Context()
{
    EVP_MD_CTX_destroy(md_ctx_);
}

void SslImpl::Context::swap(Context& rhs)
{
    std::swap(md_ctx_, rhs.md_ctx_);
}

EVP_MD_CTX* SslImpl::Context::get() const
{
    return md_ctx_;
}

SslImpl::DigestType::DigestType(const HashType& hash_type) : evp_md_(nullptr)
{
    switch (hash_type.code())
    {
        case HashCode::SHA1:
        {
            evp_md_ = EVP_sha1();
            break;
        }
        case HashCode::SHA2_256:
        {
            evp_md_ = EVP_sha256();
            break;
        }
        case HashCode::SHA2_512:
        {
            evp_md_ = EVP_sha512();
            break;
        }
        default:
        {
            throw Exception("Invalid hash type: " + hash_type.name());
        }
    }
}

const EVP_MD* SslImpl::DigestType::get() const
{
    return evp_md_;
}

int SslImpl::DigestType::digest_size() const
{
    return EVP_MD_size(evp_md_);
}

int SslImpl::DigestType::block_size() const
{
    return EVP_MD_block_size(evp_md_);
}

SslImpl::SslImpl(const HashType& hash_type) : type_(DigestType(hash_type))
{
    if (EVP_DigestInit_ex(context_.get(), type_.get(), nullptr) != 1)
    {
        throw Exception("Unable to initialise hash digest function");
    }
}

void SslImpl::update(const ArrayRef data)
{
    if (EVP_DigestUpdate(context_.get(), &data[0], data.size()) != 1)
    {
        throw Exception("Failed to update digest");
    }
}

Bytes SslImpl::digest()
{
    Context context(context_);
    Bytes output(type_.digest_size());
    unsigned int digest_size;
    if (EVP_DigestFinal_ex(context.get(),
                           reinterpret_cast<unsigned char*>(&output[0]),
                           &digest_size) != 1)
    {
        throw Exception("Unable to get digest");
    }
    if (output.size() != digest_size)
    {
        throw Exception("Unexpected digest size");
    }
    return output;
}

Hash HashBytesCodec::Impl::decode(const Bytes& raw_bytes) const
{
    auto code = static_cast<HashCode>((raw_bytes.at(0)));
    auto size = uint8_t(raw_bytes.at(1));
    auto hash_type = HashType(code);

    Bytes digest(begin(raw_bytes) + 2, end(raw_bytes));
    if (digest.size() != size)
    {
        std::ostringstream err;
        err << "Header misreports digest size as " << static_cast<int>(size)
            << "; true size is " << digest.size();
        throw Exception(err.str());
    }
    if (size != hash_type.size())
    {
        std::ostringstream err;
        err << "Non standard digest size " << static_cast<int>(size)
            << "; expected " << hash_type.size();
        throw Exception(err.str());
    }
    return Hash(std::move(hash_type), digest);
}

HashBytesCodec::Impl* HashBytesCodec::Impl::instance()
{
    return &instance_;
}

HashBytesCodec::HashBytesCodec() : pImpl(HashBytesCodec::Impl::instance())
{
}

HashBytesCodec::HashBytesCodec(const HashBytesCodec& other)
    : pImpl(other.pImpl)
{
}

HashBytesCodec& HashBytesCodec::operator=(const HashBytesCodec& rhs)
{
    pImpl = rhs.pImpl;
    return *this;
}

HashBytesCodec::~HashBytesCodec()
{
}

Bytes HashBytesCodec::operator()(const Hash& hash) const
{
    return pImpl->encode(hash);
}

Hash HashBytesCodec::operator()(const Bytes& input) const
{
    return pImpl->decode(input);
}

Bytes HashBytesCodec::Impl::encode(const Hash& hash) const
{
    auto size = uint8_t(hash.digest().size());
    auto code = uint8_t(hash.type().code());
    Bytes data;
    data.reserve(size + 2);
    data.push_back(code);
    data.push_back(size);
    std::copy(begin(hash.digest()), end(hash.digest()),
              std::back_inserter(data));
    return data;
}

size_t SslImpl::block_size()
{
    return type_.block_size();
}

} // namespace multihash

std::ostream& operator<<(std::ostream& os, const multihash::Hash& hash)
{
    multihash::HashBytesCodec encode;
    auto data(encode(hash));
    for (auto c : data)
    {
        auto uc = uint8_t(c);
        os << std::hex << std::setfill('0') << std::setw(2);
        os << static_cast<int>(uc);
    }
    return os;
}
