#include "HashFunctionImpl.h"
#include "SslImpl.h"
#include <stdexcept>

namespace shax
{
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
            throw std::out_of_range("No hash function for " +
                                    hash_type_.name());
    }
}

bool HashFunction::operator==(const HashFunction& rhs) const
{
    return type() == rhs.type();
}

Hash HashFunction::Impl::operator()(std::istream& input) const
{
    if (!input.good())
    {
        throw std::invalid_argument("HashFunction input is not good");
    }

    algorithm_->init();
    auto buffer = std::string(algorithm_->block_size(), ' ');
    auto begin = buffer.begin();
    auto end = buffer.end();

    while (!input.eof())
    {
        input.read(const_cast<char*>(buffer.data()), buffer.size());

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

Hash HashFunction::Impl::operator()(const string_view input) const
{
    algorithm_->init();
    auto block_size = algorithm_->block_size();
    auto begin = input.begin();
    auto size = std::min(input.size(), block_size);
    auto remaining = input.size();

    while (size > 0)
    {
        auto buffer = string_view(begin, size);
        algorithm_->update(buffer);
        begin += size;
        remaining -= size;
        size = std::min(remaining, block_size);
    }
    return Hash(hash_type_, algorithm_->digest());
}

} // namespace shax
