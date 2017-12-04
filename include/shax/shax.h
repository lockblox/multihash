#pragma once

#include <exception>
#include <experimental/string_view>
#include <istream>
#include <memory>
#include <set>
#include <stdint.h>
#include <string>
#include <vector>

namespace x
{

namespace hashi
{

using string_view = std::experimental::fundamentals_v1::string_view;

/** Base exception for the hashi library */
class Exception : public std::exception
{
public:
    explicit Exception(std::string message);
    const char* what() const noexcept override;

private:
    std::string message_;
};

/** Thrown when an invalid hash name or type is used */
struct InvalidHashException : public Exception
{
    explicit InvalidHashException(const std::string& message);
};

typedef std::vector<char> Bytes;

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

    static std::set<HashType> types();

private:
    class Impl;
    const Impl* pImpl;
};

/** Interface for a hash digest with encapsulated type */
class Hash
{
public:
    Hash();
    Hash(HashType type, const Bytes& digest);
    Hash& operator=(const Hash& rhs) noexcept;
    Hash(const Hash& rhs);
    ~Hash();

    const HashType& type() const;
    const Bytes& digest() const;

    bool operator==(const Hash& rhs) const;
    bool operator!=(const Hash& rhs) const;
    bool operator<(const Hash& rhs) const;

private:
    class Impl;
    std::shared_ptr<Impl> pImpl;
};

/** Functor which hashes an input stream with a given hash type */
class HashFunction
{
public:
    using result_type = Hash;
    using argument_type = string_view;

    explicit HashFunction(HashCode code = HashCode::SHA2_256);
    explicit HashFunction(const HashType& hash_type);
    explicit HashFunction(const std::string& hash_type);
    HashFunction(const HashFunction& other);
    HashFunction& operator=(const HashFunction& rhs);
    ~HashFunction();

    bool operator==(const HashFunction& rhs) const;

    Hash operator()(std::istream& input) const;
    Hash operator()(const string_view input) const;
    const HashType& type() const;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

/** Functor which converts a hash to and from raw bytes */
class HashBytesCodec
{
public:
    using encoded_type = Bytes;
    using value_type = Hash;

    HashBytesCodec();
    HashBytesCodec(const HashBytesCodec& other);
    HashBytesCodec& operator=(const HashBytesCodec& rhs);
    ~HashBytesCodec();

    Bytes operator()(const Hash& hash) const;
    Hash operator()(const Bytes& input) const;

private:
    class Impl;
    const Impl* pImpl;
};

} // namespace hashi

} // namespace x;

std::ostream&
operator<<(std::ostream& os, const x::hashi::HashCode& hash_code);
std::ostream&
operator<<(std::ostream& os, const x::hashi::HashType& hash_type);
std::ostream& operator<<(std::ostream& os, const x::hashi::Hash& hash);
