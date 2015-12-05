#include "Multihash.h"
#include <sstream>
#include <mutex> //for call_once
#include <iomanip>

namespace multihash
{

Exception::Exception(std::string message) : message_(std::move(message)) 
{
}

const char* Exception::what() const noexcept
{
    return message_.c_str();
}

Hash::Hash(HashType type, const Buffer& digest) : pImpl(new Impl(type, digest)) 
{
}

Hash::Hash(Hash&& rhs) noexcept : pImpl(std::move(rhs.pImpl)) 
{
}

Hash::Hash(const Hash& rhs) : pImpl(rhs.pImpl) 
{
}

Hash::~Hash() 
{
}

bool Hash::operator==(const Hash& rhs) const
{
    return type() == rhs.type() && digest() == rhs.digest();
}

const HashType& Hash::type() const
{
    return pImpl->type();
}

const Buffer& Hash::digest() const
{
    return pImpl->digest();
}

HashFunction::HashFunction(const HashType& hash_type) : pImpl(new Impl(hash_type))
{
}

HashFunction::HashFunction(HashFunction&& rhs) noexcept : pImpl(std::move(rhs.pImpl))
{
}

/** Though the destructor is trivial, we must define it separately since the
 * default destructor of unique_ptr would require a complete declaration of Impl */
HashFunction::~HashFunction()
{
}

Hash HashFunction::operator()(std::istream& input)
{
    return (*pImpl)(input);
}

HashFunction::Impl::Impl(const HashType& hash_type) 
    : hash_type_(hash_type), algorithm_(nullptr)
{
    switch(hash_type.code())
    {
        case HashCode::SHA1:
        case HashCode::SHA2_256:
        case HashCode::SHA2_512:
            algorithm_.reset(new SslImpl(hash_type_));
            break;
        case HashCode::SHA3:
        case HashCode::BLAKE2B:
        case HashCode::BLAKE2S:
            throw Exception("No hash function for " + hash_type.name());
            break;
    }
}

Hash HashFunction::Impl::operator()(std::istream& input)
{
    if (!input.good())
    {
        throw Exception("HashFunction input is not good");
    }
    auto block_size(algorithm_->block_size());
    if (! (block_size > 0))
    {
        throw Exception("Block size of 0");
    }

    Buffer buffer(block_size);
    algorithm_->reset();
    auto begin  = buffer.begin();
    auto end    = buffer.end();
    auto it     = begin;

    while (!input.eof())
    {
        input.read(reinterpret_cast<char*>(buffer.data()), block_size);

        if (!input.eof())
        {
            //filled the buffer so update the hash
            algorithm_->update(buffer);
        }
    }
    //final update to hash with partially filled vector
    auto chars_read(input.gcount());
    std::advance(begin, chars_read);
    buffer.erase(begin, end);
    algorithm_->update(buffer);
    return Hash(hash_type_, algorithm_->digest());
}

SslImpl::SslImpl(const HashType& hash_type) 
    : evpmd_(evpmd(hash_type.code())), mdctx_(nullptr) 
{
    if (evpmd_ == nullptr) 
    {
        throw Exception("Invalid hash type: " + hash_type.name());
    }
    init();
}

SslImpl::~SslImpl()
{
    EVP_MD_CTX_destroy(mdctx_);
}

const EVP_MD* SslImpl::evpmd(HashCode code) const
{
    switch(code)
    {
        case HashCode::SHA1 :
        {
            return EVP_sha1();
            break;
        }
        case HashCode::SHA2_256 :
        {
            return EVP_sha256();
            break;
        }
        case HashCode::SHA2_512 :
        {
            return EVP_sha512();
            break;
        }
        default :
        {
            return nullptr;
            break;
        }
    }
}

void SslImpl::init()
{
    mdctx_ = EVP_MD_CTX_create();
    if (EVP_DigestInit_ex(mdctx_, evpmd_, nullptr) != 1)
    {
        throw Exception("Unable to initialise hash digest function");
    }
}

void SslImpl::update(const Buffer& data)
{
    if (EVP_DigestUpdate(mdctx_, &data[0], data.size()) != 1)
    {
        throw Exception("Failed to update digest");
    }
}

Buffer SslImpl::digest()
{
    Buffer output(EVP_MD_size(evpmd_));
    EVP_MD_CTX* mdctx_copy(EVP_MD_CTX_create());
    if (mdctx_copy == nullptr)
    {
        throw Exception("Unable to create context copy");
    }
    if (EVP_MD_CTX_copy_ex(mdctx_copy, mdctx_) != 1)
    {
        throw Exception("Unable to backup hash context");
    }
    unsigned int digest_size;
    if (EVP_DigestFinal_ex(mdctx_copy, &output[0], &digest_size) != 1)
    {
        throw Exception("Unable to get digest");
    }
    if (output.size() != digest_size)
    {
        throw Exception("Unexpected digest size");
    }
    return output;
}

void SslImpl::reset()
{
    EVP_MD_CTX_destroy(mdctx_);
    init();
}

Hash BufferDecoder::Impl::decode(const Buffer& raw_bytes)
{
    auto code = static_cast<HashCode>((raw_bytes.at(0)));
    auto size(raw_bytes.at(1));
    auto hash_type = HashType(code);

    Buffer digest(begin(raw_bytes)+2, end(raw_bytes));
    if (digest.size() != size)
    {
        std::ostringstream err;
        err << "Header misreports digest size as " << size 
            << "; true size is" << digest.size();
        throw Exception(err.str());
    }
    if (size != hash_type.size())
    {
        std::ostringstream err;
        err << "Non standard digest size " << size << "; expected " 
            << hash_type.size();
        throw Exception(err.str());
    }
    return Hash(std::move(hash_type), digest);
}

BufferEncoder::BufferEncoder() : pImpl(new BufferEncoder::Impl) 
{
}

BufferEncoder::~BufferEncoder()
{
}

Buffer BufferEncoder::operator() (const Hash& hash)
{
    return pImpl->encode(hash);
}

BufferDecoder::BufferDecoder() : pImpl(new BufferDecoder::Impl) 
{
}

Hash BufferDecoder::operator() (const Buffer& input)
{
    return pImpl->decode(input);
}

BufferDecoder::~BufferDecoder() {}

Buffer BufferEncoder::Impl::encode(const Hash& hash)
{
    Buffer data;
    data.reserve(hash.digest().size() + 2);
    data.push_back(static_cast<unsigned char>(hash.type().code()));
    data.push_back(static_cast<unsigned char>(hash.digest().size()));
    std::copy(begin(hash.digest()), end(hash.digest()), std::back_inserter(data));
    return data;
}

int SslImpl::block_size()
{
    return EVP_MD_block_size(evpmd_);
}

} // namespace multihash

std::ostream& operator<<(std::ostream& os, const multihash::Hash& hash)
{
    multihash::BufferEncoder encode;
    auto data(encode(hash));
    for (auto c : data)
    {
        os << std::hex << std::setfill('0') << std::setw(2);
        os << static_cast<int>(c);
    }
    return os;
}
