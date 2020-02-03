//
// fast_float_sign_extract
// (c) 2019 Andrzej Budzanowski <kontakt@andrzej.budzanowski.pl>
//

#include <benchmark/benchmark.h>
#include <memory.h>
#include "./ring_buffer.hpp"

inline bool is_negative_with_branch(float f)
{
    return f < 0;
}

inline bool is_negative_ub(float f)
{
    return *(unsigned *)&f >> 31u;
}

inline bool is_negative_sb(float f)
{
    int buff = 0;
    memcpy(&buff, &f, sizeof(f));
    return buff >> 31;
}

inline bool is_efficient(float f)
{
    char buff[65];
    sprintf(buff, "%f", f);
    return buff[0] == '-';
}

static void bench_read_one(benchmark::State& state)
{
    auto floats = ring_buff<float>(static_cast<float>(state.range(0)), static_cast<float>(state.range(1)));

    for (auto _ : state) {
        float tmp = floats.get();
        benchmark::DoNotOptimize(tmp);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(sizeof(float)));
    state.SetItemsProcessed(int64_t(state.iterations()));
}
BENCHMARK(bench_read_one)
        ->ArgPair(-1000000, -1024)
        ->ArgPair(1024, 1000000)
        ->ArgPair(-1000000, 1000000);


static void bench_branch_one(benchmark::State& state)
{
    auto floats = ring_buff<float>(static_cast<float>(state.range(0)), static_cast<float>(state.range(1)));

    for (auto _ : state) {
        bool tmp = is_negative_with_branch(floats.get());
        benchmark::DoNotOptimize(tmp);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(sizeof(float)));
    state.SetItemsProcessed(int64_t(state.iterations()));
}
BENCHMARK(bench_branch_one)
        ->ArgPair(-1000000, -1024)
        ->ArgPair(1024, 1000000)
        ->ArgPair(-1000000, 1000000);

static void bench_ub_one(benchmark::State& state)
{
    auto floats = ring_buff<float>(static_cast<float>(state.range(0)), static_cast<float>(state.range(1)));

    for (auto _ : state) {
        bool tmp = is_negative_ub(floats.get());
        benchmark::DoNotOptimize(tmp);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(sizeof(float)));
    state.SetItemsProcessed(int64_t(state.iterations()));
}
BENCHMARK(bench_ub_one)
        ->ArgPair(-1000000, -1024)
        ->ArgPair(1024, 1000000)
        ->ArgPair(-1000000, 1000000);

static void bench_nub_one(benchmark::State& state)
{
    auto floats = ring_buff<float>(static_cast<float>(state.range(0)), static_cast<float>(state.range(1)));

    for (auto _ : state) {
        bool tmp = is_negative_sb(floats.get());
        benchmark::DoNotOptimize(tmp);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(sizeof(float)));
    state.SetItemsProcessed(int64_t(state.iterations()));
}
BENCHMARK(bench_nub_one)
        ->ArgPair(-1000000, -1024)
        ->ArgPair(1024, 1000000)
        ->ArgPair(-1000000, 1000000);

static void bench_signbit_one(benchmark::State& state)
{
    auto floats = ring_buff<float>(static_cast<float>(state.range(0)), static_cast<float>(state.range(1)));

    for (auto _ : state) {
        bool tmp = std::signbit(floats.get());
        benchmark::DoNotOptimize(tmp);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(sizeof(float)));
    state.SetItemsProcessed(int64_t(state.iterations()));
}
BENCHMARK(bench_signbit_one)
        ->ArgPair(-1000000, -1024)
        ->ArgPair(1024, 1000000)
        ->ArgPair(-1000000, 1000000);

static void bench_efficient_one(benchmark::State& state)
{
    auto floats = ring_buff<float>(static_cast<float>(state.range(0)), static_cast<float>(state.range(1)));

    for (auto _ : state) {
        bool tmp = is_efficient(floats.get());
        benchmark::DoNotOptimize(tmp);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(sizeof(float)));
    state.SetItemsProcessed(int64_t(state.iterations()));
}
BENCHMARK(bench_efficient_one)
        ->ArgPair(-1000000, -1024)
        ->ArgPair(1024, 1000000)
        ->ArgPair(-1000000, 1000000);
