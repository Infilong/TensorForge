#pragma once

#include <cstddef>
#include <memory>

namespace tensorforge {

class CPUStorage {
public:
    explicit CPUStorage(std::size_t size_bytes);
    ~CPUStorage() = default;

    CPUStorage(const CPUStorage&) = delete;
    CPUStorage& operator=(const CPUStorage&) = delete;

    CPUStorage(CPUStorage&& other) noexcept;
    CPUStorage& operator=(CPUStorage&& other) noexcept;

    [[nodiscard]] std::byte* data() noexcept;
    [[nodiscard]] const std::byte* data() const noexcept;
    [[nodiscard]] std::size_t size_bytes() const noexcept;

private:
    std::unique_ptr<std::byte[]> data_;
    std::size_t size_bytes_{0};
};

} // namespace tensorforge
