#pragma once

#include <cstdint>
#include <vector>

namespace vcm {

class IVcmTransport {
public:
  virtual ~IVcmTransport() = default;

  virtual void send(const std::vector<uint8_t> &data) = 0;

  virtual std::vector<uint8_t> receive() = 0;
};

} // namespace vcm
