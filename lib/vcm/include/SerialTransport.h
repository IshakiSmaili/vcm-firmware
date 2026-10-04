#pragma once

#include "IVcmTransport.h"

namespace vcm {

class SerialTransport : public IVcmTransport {
public:
  void send(const std::vector<uint8_t> &data) override;

  std::vector<uint8_t> receive() override;

private:
  std::vector<uint8_t> buffer;
};

} // namespace vcm
