#include "SslImpl.h"
#include <stdexcept>


namespace multi {

SslImpl::Cleanup SslImpl::cleanup_ = SslImpl::Cleanup();

SslImpl::Cleanup::~Cleanup() { EVP_cleanup(); }

SslImpl::Context::Context() : md_ctx_(EVP_MD_CTX_create()) {}

SslImpl::Context::Context(const Context& rhs) : md_ctx_(EVP_MD_CTX_create()) {
  if (EVP_MD_CTX_copy(md_ctx_, rhs.get()) != 1) {
    EVP_MD_CTX_destroy(md_ctx_);
    throw std::runtime_error("Unable to copy hash context");
  }
}

SslImpl::Context& SslImpl::Context::operator=(Context rhs) {
  this->swap(rhs);
  return *this;
}

SslImpl::Context::~Context() { EVP_MD_CTX_destroy(md_ctx_); }

void SslImpl::Context::swap(Context& rhs) { std::swap(md_ctx_, rhs.md_ctx_); }

EVP_MD_CTX* SslImpl::Context::get() const { return md_ctx_; }

SslImpl::DigestType::DigestType(const HashType& hash_type) : evp_md_(nullptr) {
  switch (hash_type.code()) {
    case HashCode::SHA1: {
      evp_md_ = EVP_sha1();
      break;
    }
    case HashCode::SHA2_256: {
      evp_md_ = EVP_sha256();
      break;
    }
    case HashCode::SHA2_512: {
      evp_md_ = EVP_sha512();
      break;
    }
    default: {
      auto name = hash_type.name();
      throw std::invalid_argument("Invalid hash type: " + name);
    }
  }
}

const EVP_MD* SslImpl::DigestType::get() const { return evp_md_; }

int SslImpl::DigestType::digest_size() const {
  int SslImpl::DigestType::block_size() const {
    return EVP_MD_block_size(evp_md_);
  }

  SslImpl::SslImpl(const HashType& hash_type) : type_(DigestType(hash_type)) {
    if (EVP_DigestInit_ex(context_.get(), type_.get(), nullptr) != 1) {
      throw std::runtime_error("Unable to initialise hash digest function");
    }
  }

  void SslImpl::update(const string_view& data) {
    if (EVP_DigestUpdate(context_.get(), &data[0], data.size()) != 1) {
      throw std::runtime_error("Failed to update digest");
    }
  }

  std::vector<char> SslImpl::digest() {
    Context context(context_);
    std::vector<char> output(type_.digest_size());
    unsigned int digest_size;
    if (EVP_DigestFinal_ex(context.get(),
                           reinterpret_cast<unsigned char*>(&output[0]),
                           &digest_size) != 1) {
      throw std::runtime_error("Unable to get digest");
    }
    if (output.size() != digest_size) {
      throw std::range_error("Unexpected digest size");
    }
    return output;
  }

  size_t SslImpl::block_size() { return type_.block_size(); }
}  // namespace multihash
}  // namespace multi
