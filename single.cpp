//
// fast_float_sign_extract
// (c) 2019 Andrzej Budzanowski <kontakt@andrzej.budzanowski.pl>
//

#include <benchmark/benchmark.h>
#include <memory.h>
#include "./ring_buffer.hpp"

static inline bool is_negative_with_branch(float f)
{
    return f < 0;
}

static inline bool is_negative_ub(float f)
{
    return *(unsigned *)&f >> 31u;
}

static inline bool is_negative_sb(float f)
{
    int buff = 0;
    memcpy(&buff, &f, sizeof(f));
    return buff >> 31u;
}

static inline bool check_with_sprintf(float f)
{
    char buff[64];
    sprintf(buff, "%f", f);
    return buff[0] == '-';
}

static inline bool check_with_sprintf_new(float f)
{
    char * buff = new char[64];
    sprintf(buff, "%f", f);
    bool ret = buff[0] == '-';
    delete[] buff;
    return ret;
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

static void bench_check_with_sprintf(benchmark::State& state)
{
    auto floats = ring_buff<float>(static_cast<float>(state.range(0)), static_cast<float>(state.range(1)));

    for (auto _ : state) {
        bool tmp = check_with_sprintf(floats.get());
        benchmark::DoNotOptimize(tmp);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(sizeof(float)));
    state.SetItemsProcessed(int64_t(state.iterations()));
}
BENCHMARK(bench_check_with_sprintf)
        ->ArgPair(-1000000, -1024)
        ->ArgPair(1024, 1000000)
        ->ArgPair(-1000000, 1000000);

static void bench_check_with_sprintf_null(benchmark::State& state)
{
    auto floats = ring_buff<float>(static_cast<float>(state.range(0)), static_cast<float>(state.range(1)));

    for (auto _ : state) {
        bool tmp = check_with_sprintf_new(floats.get());
        benchmark::DoNotOptimize(tmp);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(sizeof(float)));
    state.SetItemsProcessed(int64_t(state.iterations()));
}
BENCHMARK(bench_check_with_sprintf_null)
        ->ArgPair(-1000000, -1024)
        ->ArgPair(1024, 1000000)
        ->ArgPair(-1000000, 1000000);
