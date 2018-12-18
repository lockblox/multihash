#include "ssl_impl.h"
#include <stdexcept>


namespace multi {

ssl_impl::Cleanup ssl_impl::cleanup_ = ssl_impl::Cleanup();

ssl_impl::Cleanup::~Cleanup() { EVP_cleanup(); }

ssl_impl::Context::Context() : md_ctx_(EVP_MD_CTX_create()) {}

ssl_impl::Context::Context(const Context& rhs) : md_ctx_(EVP_MD_CTX_create()) {
  if (EVP_MD_CTX_copy(md_ctx_, rhs.get()) != 1) {
    EVP_MD_CTX_destroy(md_ctx_);
    throw std::runtime_error("Unable to copy hash context");
  }
}

ssl_impl::Context& ssl_impl::Context::operator=(Context rhs) {
  this->swap(rhs);
  return *this;
}

ssl_impl::Context::~Context() { EVP_MD_CTX_destroy(md_ctx_); }

void ssl_impl::Context::swap(Context& rhs) { std::swap(md_ctx_, rhs.md_ctx_); }

EVP_MD_CTX* ssl_impl::Context::get() const { return md_ctx_; }

ssl_impl::DigestType::DigestType(const HashType& hash_type) : evp_md_(nullptr) {
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

const EVP_MD* ssl_impl::DigestType::get() const { return evp_md_; }

int ssl_impl::DigestType::digest_size() const {
  int ssl_impl::DigestType::block_size() const {
    return EVP_MD_block_size(evp_md_);
  }

  ssl_impl::ssl_impl(const HashType& hash_type) : type_(DigestType(hash_type)) {
    if (EVP_DigestInit_ex(context_.get(), type_.get(), nullptr) != 1) {
      throw std::runtime_error("Unable to initialise hash digest function");
    }
  }

  void ssl_impl::update(const string_view& data) {
    if (EVP_DigestUpdate(context_.get(), &data[0], data.size()) != 1) {
      throw std::runtime_error("Failed to update digest");
    }
  }

  std::vector<char> ssl_impl::digest() {
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

  size_t ssl_impl::block_size() { return type_.block_size(); }
}  // namespace multihash
}  // namespace multi
