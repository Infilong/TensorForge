#include "tensorforge/cpu_storage.hpp"

#include <cstddef>
#include <iostream>
#include <type_traits>
#include <utility>

namespace {

class TestContext {
public:
    void expect(bool condition, const char* message)
    {
        if (!condition) {
            std::cerr << "FAIL: " << message << '\n';
            ++failures_;
        }
    }

    [[nodiscard]] int result() const
    {
        if (failures_ == 0) {
            std::cout << "All CPUStorage tests passed\n";
            return 0;
        }

        std::cerr << failures_ << " CPUStorage test(s) failed\n";
        return 1;
    }

private:
    int failures_{0};
};

void test_non_zero_allocation(TestContext& context)
{
    tensorforge::CPUStorage storage{64};

    // A positive size must produce an accessible contiguous byte allocation
    // while preserving the requested size.
    context.expect(storage.data() != nullptr, "non-zero storage has data");
    context.expect(storage.size_bytes() == 64, "non-zero storage keeps its size");

    // Writing through the mutable overload and reading through the const
    // overload verifies that both accessors refer to the same allocation.
    storage.data()[0] = std::byte{0x2A};
    const tensorforge::CPUStorage& const_storage = storage;
    context.expect(
        const_storage.data()[0] == std::byte{0x2A},
        "const data access observes stored bytes");
}

void test_zero_byte_allocation(TestContext& context)
{
    const tensorforge::CPUStorage storage{0};

    // Empty storage owns no allocation and reports a matching zero size.
    context.expect(storage.data() == nullptr, "zero-byte storage has null data");
    context.expect(storage.size_bytes() == 0, "zero-byte storage has zero size");
}

void test_move_construction(TestContext& context)
{
    tensorforge::CPUStorage source{32};
    std::byte* const original_data = source.data();

    tensorforge::CPUStorage destination{std::move(source)};

    // Pointer identity demonstrates that ownership moved without copying the
    // allocation. The source must become observably empty.
    context.expect(destination.data() == original_data, "move construction transfers data");
    context.expect(destination.size_bytes() == 32, "move construction transfers size");
    context.expect(source.data() == nullptr, "move-constructed source has null data");
    context.expect(source.size_bytes() == 0, "move-constructed source has zero size");
}

void test_move_assignment(TestContext& context)
{
    tensorforge::CPUStorage source{48};
    tensorforge::CPUStorage destination{16};
    std::byte* const original_data = source.data();

    destination = std::move(source);

    // Assignment must release destination's previous buffer, take the source
    // allocation, and leave the source in the same empty moved-from state.
    context.expect(destination.data() == original_data, "move assignment transfers data");
    context.expect(destination.size_bytes() == 48, "move assignment transfers size");
    context.expect(source.data() == nullptr, "move-assigned source has null data");
    context.expect(source.size_bytes() == 0, "move-assigned source has zero size");
}

} // namespace

int main()
{
    // These properties are part of CPUStorage's ownership contract and are
    // checked by the compiler rather than only at runtime.
    static_assert(!std::is_copy_constructible_v<tensorforge::CPUStorage>);
    static_assert(!std::is_copy_assignable_v<tensorforge::CPUStorage>);
    static_assert(std::is_nothrow_move_constructible_v<tensorforge::CPUStorage>);
    static_assert(std::is_nothrow_move_assignable_v<tensorforge::CPUStorage>);

    TestContext context;
    test_non_zero_allocation(context);
    test_zero_byte_allocation(context);
    test_move_construction(context);
    test_move_assignment(context);
    return context.result();
}
