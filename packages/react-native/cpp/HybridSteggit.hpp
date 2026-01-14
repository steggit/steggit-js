#pragma once

#include "HybridSteggitSpec.hpp"

namespace margelo::nitro::steggit {

class HybridSteggit : public HybridSteggitSpec {
public:
  HybridSteggit() : HybridSteggitSpec() {}

  // Method implementations
  std::shared_ptr<Promise<void>>
  encodeTextPng(const std::string &inputPath, const std::string &outputPath,
                const std::string &message,
                const std::optional<std::string> &header) override;

  std::shared_ptr<Promise<void>>
  encodeTextJpeg(const std::string &inputPath, const std::string &outputPath,
                 const std::string &message,
                 const std::optional<std::string> &header) override;

  std::shared_ptr<Promise<std::string>>
  decodeTextPng(const std::string &inputPath,
                const std::optional<std::string> &header) override;

  std::shared_ptr<Promise<std::string>>
  decodeTextJpeg(const std::string &inputPath,
                 const std::optional<std::string> &header) override;
};

} // namespace margelo::nitro::steggit
