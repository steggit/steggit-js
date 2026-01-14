#include "HybridSteggit.hpp"
#include <steggit.h>
#include <thread>
#include <stdexcept>
#include <cstring>

namespace margelo::nitro::steggit {

std::shared_ptr<Promise<void>>
HybridSteggit::encodeTextPng(const std::string &inputPath,
                              const std::string &outputPath,
                              const std::string &message,
                              const std::optional<std::string> &header) {
  return Promise<void>::async([=]() {
    char *error_message = nullptr;
    const char *header_ptr = header.has_value() ? header.value().c_str() : nullptr;
    
    int result = encode_png(inputPath.c_str(), outputPath.c_str(),
                           message.c_str(), header_ptr, &error_message);
    
    if (result != 0) {
      std::string error = error_message ? error_message : "Unknown error encoding PNG";
      if (error_message) {
        free(error_message);
      }
      throw std::runtime_error(error);
    }
    
    if (error_message) {
      free(error_message);
    }
  });
}

std::shared_ptr<Promise<void>>
HybridSteggit::encodeTextJpeg(const std::string &inputPath,
                               const std::string &outputPath,
                               const std::string &message,
                               const std::optional<std::string> &header) {
  return Promise<void>::async([=]() {
    char *error_message = nullptr;
    const char *header_ptr = header.has_value() ? header.value().c_str() : nullptr;
    
    int result = encode_jpeg(inputPath.c_str(), outputPath.c_str(),
                            message.c_str(), header_ptr, &error_message);
    
    if (result != 0) {
      std::string error = error_message ? error_message : "Unknown error encoding JPEG";
      if (error_message) {
        free(error_message);
      }
      throw std::runtime_error(error);
    }
    
    if (error_message) {
      free(error_message);
    }
  });
}

std::shared_ptr<Promise<std::string>>
HybridSteggit::decodeTextPng(const std::string &inputPath,
                              const std::optional<std::string> &header) {
  return Promise<std::string>::async([=]() -> std::string {
    char *error_message = nullptr;
    const char *header_ptr = header.has_value() ? header.value().c_str() : nullptr;
    
    char *result = decode_png(inputPath.c_str(), header_ptr, &error_message);
    
    if (result == nullptr) {
      std::string error = error_message ? error_message : "Unknown error decoding PNG";
      if (error_message) {
        free(error_message);
      }
      throw std::runtime_error(error);
    }
    
    std::string message_str(result);
    free(result);
    
    if (error_message) {
      free(error_message);
    }
    
    return message_str;
  });
}

std::shared_ptr<Promise<std::string>>
HybridSteggit::decodeTextJpeg(const std::string &inputPath,
                               const std::optional<std::string> &header) {
  return Promise<std::string>::async([=]() -> std::string {
    char *error_message = nullptr;
    const char *header_ptr = header.has_value() ? header.value().c_str() : nullptr;
    
    char *result = decode_jpeg(inputPath.c_str(), header_ptr, &error_message);
    
    if (result == nullptr) {
      std::string error = error_message ? error_message : "Unknown error decoding JPEG";
      if (error_message) {
        free(error_message);
      }
      throw std::runtime_error(error);
    }
    
    std::string message_str(result);
    free(result);
    
    if (error_message) {
      free(error_message);
    }
    
    return message_str;
  });
}

} // namespace margelo::nitro::steggit
