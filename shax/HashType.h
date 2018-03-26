#pragma once

#include <set>
#include <string>

namespace shax
{

/** Code values of supported hash types */
enum class HashCode : unsigned char
{
    IDENTITY = 0x00,
    SHA1 = 0x11,     //"sha1"
    SHA2_256 = 0x12, //"sha2-256"
    SHA2_512 = 0x13, //"sha2-512"
    SHA3 = 0x14,     //"sha3"
    BLAKE2B = 0x40,  //"blake2b"
    BLAKE2S = 0x41   //"blake2s"
};

/** Interface for a hash Type */
class HashType
{
public:
    HashType();
    explicit HashType(const std::string& name);
    explicit HashType(HashCode code);
    HashType(const HashType& rhs);

    const std::string& name() const;
    HashCode code() const;
    size_t size() const; /**< Default length of the hash type */

    bool operator==(const HashType& rhs) const;
    bool operator!=(const HashType& rhs) const;
    bool operator<(const HashType& rhs) const;
    bool operator>(const HashType& rhs) const;

    static std::set<HashType> types();

private:
    class Impl;
    const Impl* pImpl;
};

std::ostream& operator<<(std::ostream& os, const shax::HashCode& hash_code);

} // namespace shax
