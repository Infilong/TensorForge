#include "tensorforge/cpu_storage.hpp"

#include <utility>

namespace tensorforge {

CPUStorage::CPUStorage(std::size_t size_bytes)
    : size_bytes_{size_bytes}
{
    // Avoid allocating for an empty storage object. A default-constructed
    // unique_ptr remains null, matching the zero-size API contract.
    if (size_bytes_ != 0) {
        data_ = std::make_unique<std::byte[]>(size_bytes_);
    }
}

CPUStorage::CPUStorage(CPUStorage&& other) noexcept
    // Moving unique_ptr transfers ownership and leaves other.data_ null.
    // The separate size field must be reset explicitly to keep the moved-from
    // object in a consistent empty state.
    : data_{std::move(other.data_)}
    , size_bytes_{std::exchange(other.size_bytes_, 0)}
{
}

CPUStorage& CPUStorage::operator=(CPUStorage&& other) noexcept
{
    // The self-check prevents a self-move from discarding the allocation.
    if (this != &other) {
        // unique_ptr move assignment first releases this object's old buffer,
        // then takes ownership of the source buffer. No manual delete is needed.
        data_ = std::move(other.data_);
        size_bytes_ = std::exchange(other.size_bytes_, 0);
    }

    return *this;
}

std::byte* CPUStorage::data() noexcept
{
    // This pointer provides access to the bytes but does not transfer ownership.
    return data_.get();
}

const std::byte* CPUStorage::data() const noexcept
{
    // Const storage exposes read-only access to the same owned allocation.
    return data_.get();
}

std::size_t CPUStorage::size_bytes() const noexcept
{
    return size_bytes_;
}

} // namespace tensorforge
