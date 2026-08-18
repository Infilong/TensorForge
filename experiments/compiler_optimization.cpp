#include <charconv>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <system_error>
#include <vector>

namespace tensorforge::experiments {

void dead_work(std::size_t n)
{
    std::uint64_t value{1};

    for (std::size_t i = 0; i < n; ++i) {
        value = value * 1'664'525ULL
            + static_cast<std::uint64_t>(i)
            + 1'013'904'223ULL;
    }

    (void)value;
}

std::uint64_t sum_values(std::size_t n)
{
    std::uint64_t sum{0};

    for (std::size_t i = 0; i < n; ++i) {
        sum += static_cast<std::uint64_t>(i);
    }

    return sum;
}

void vector_add(
    const float* a,
    const float* b,
    float* c,
    std::size_t n)
{
    for (std::size_t i = 0; i < n; ++i) {
        c[i] = a[i] + b[i];
    }
}

} // namespace tensorforge::experiments

namespace {

constexpr std::size_t default_element_count{5'000'000};

std::size_t parse_element_count(int argc, char* argv[])
{
    if (argc == 1) {
        return default_element_count;
    }

    if (argc != 2) {
        throw std::invalid_argument("expected zero or one argument");
    }

    const char* const input_begin = argv[1];
    const char* const input_end = input_begin + std::strlen(input_begin);
    std::size_t count{0};
    const auto result = std::from_chars(
        input_begin,
        input_end,
        count);

    if (result.ec != std::errc{}
        || result.ptr != input_end
        || count == 0) {
        throw std::invalid_argument("element count must be a positive integer");
    }

    return count;
}

int run_experiment(int argc, char* argv[])
{
    const std::size_t n = parse_element_count(argc, argv);

    std::vector<float> a(n);
    std::vector<float> b(n);
    std::vector<float> c(n);

    for (std::size_t i = 0; i < n; ++i) {
        a[i] = static_cast<float>(i % 1'024) * 0.5F;
        b[i] = static_cast<float>(i % 2'048) * 0.25F;
    }

    const auto dead_start = std::chrono::steady_clock::now();
    tensorforge::experiments::dead_work(n);
    const auto dead_end = std::chrono::steady_clock::now();

    const auto sum_start = std::chrono::steady_clock::now();
    const std::uint64_t sum = tensorforge::experiments::sum_values(n);
    const auto sum_end = std::chrono::steady_clock::now();

    const auto vector_start = std::chrono::steady_clock::now();
    tensorforge::experiments::vector_add(
        a.data(),
        b.data(),
        c.data(),
        n);
    const auto vector_end = std::chrono::steady_clock::now();

    double checksum{0.0};
    for (const float value : c) {
        checksum += static_cast<double>(value);
    }

    const auto dead_ms = std::chrono::duration<double, std::milli>(
        dead_end - dead_start);
    const auto sum_ms = std::chrono::duration<double, std::milli>(
        sum_end - sum_start);
    const auto vector_ms = std::chrono::duration<double, std::milli>(
        vector_end - vector_start);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "elements: " << n << '\n';
    std::cout << "dead_work: " << dead_ms.count() << " ms\n";
    std::cout << "sum_values: " << sum << " (" << sum_ms.count() << " ms)\n";
    std::cout << "vector_add checksum: " << checksum
              << " (" << vector_ms.count() << " ms)\n";

    return 0;
}

} // namespace

int main(int argc, char* argv[])
{
    try {
        return run_experiment(argc, argv);
    } catch (const std::exception& error) {
        std::cerr << "error: " << error.what() << '\n';
        std::cerr << "usage: tensorforge_compiler_optimization [element_count]\n";
        return 1;
    }
}
