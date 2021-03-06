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
    return *(unsigned *) &f >> 31u;
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
    char *buff = new char[64];
    sprintf(buff, "%f", f);
    bool ret = buff[0] == '-';
    delete[] buff;
    return ret;
}

static void bench_read_unrolled(benchmark::State &state)
{
    auto floats = ring_buff<float>();

    for (auto _ : state) {
        float tmp[8];
        floats.get_8(tmp);
        benchmark::DoNotOptimize(tmp);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(sizeof(float)) * 8);
    state.SetItemsProcessed(int64_t(state.iterations()) * 8);
}

BENCHMARK(bench_read_unrolled);

static void bench_branch_unrolled(benchmark::State &state)
{
    auto floats = ring_buff<float>(static_cast<float>(state.range(0)), static_cast<float>(state.range(1)));

    for (auto _ : state) {
        float tmp[8];
        floats.get_8(tmp);
        bool result[8] = {is_negative_with_branch(tmp[0]),
                          is_negative_with_branch(tmp[1]),
                          is_negative_with_branch(tmp[2]),
                          is_negative_with_branch(tmp[3]),
                          is_negative_with_branch(tmp[4]),
                          is_negative_with_branch(tmp[5]),
                          is_negative_with_branch(tmp[6]),
                          is_negative_with_branch(tmp[7]),};
        benchmark::DoNotOptimize(result);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(sizeof(float)) * 8);
    state.SetItemsProcessed(int64_t(state.iterations()) * 8);
}

BENCHMARK(bench_branch_unrolled)
        ->ArgPair(-1000000, -1024)
        ->ArgPair(1024, 1000000)
        ->ArgPair(-1000000, 1000000);

static void bench_ub_unrolled(benchmark::State &state)
{
    auto floats = ring_buff<float>(static_cast<float>(state.range(0)), static_cast<float>(state.range(1)));

    for (auto _ : state) {
        float tmp[8];
        floats.get_8(tmp);
        bool result[8] = {is_negative_ub(tmp[0]),
                          is_negative_ub(tmp[1]),
                          is_negative_ub(tmp[2]),
                          is_negative_ub(tmp[3]),
                          is_negative_ub(tmp[4]),
                          is_negative_ub(tmp[5]),
                          is_negative_ub(tmp[6]),
                          is_negative_ub(tmp[7]),};
        benchmark::DoNotOptimize(result);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(sizeof(float)) * 8);
    state.SetItemsProcessed(int64_t(state.iterations()) * 8);
}

BENCHMARK(bench_ub_unrolled)
        ->ArgPair(-1000000, -1024)
        ->ArgPair(1024, 1000000)
        ->ArgPair(-1000000, 1000000);

static void bench_nub_unrolled(benchmark::State &state)
{
    auto floats = ring_buff<float>(static_cast<float>(state.range(0)), static_cast<float>(state.range(1)));

    for (auto _ : state) {
        float tmp[8];
        floats.get_8(tmp);
        bool result[8] = {is_negative_sb(tmp[0]),
                          is_negative_sb(tmp[1]),
                          is_negative_sb(tmp[2]),
                          is_negative_sb(tmp[3]),
                          is_negative_sb(tmp[4]),
                          is_negative_sb(tmp[5]),
                          is_negative_sb(tmp[6]),
                          is_negative_sb(tmp[7]),};
        benchmark::DoNotOptimize(result);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(sizeof(float)) * 8);
    state.SetItemsProcessed(int64_t(state.iterations()) * 8);
}

BENCHMARK(bench_nub_unrolled)
        ->ArgPair(-1000000, -1024)
        ->ArgPair(1024, 1000000)
        ->ArgPair(-1000000, 1000000);

static void bench_signbit_unrolled(benchmark::State &state)
{
    auto floats = ring_buff<float>(static_cast<float>(state.range(0)), static_cast<float>(state.range(1)));

    for (auto _ : state) {
        float tmp[8];
        floats.get_8(tmp);
        bool result[8] = {std::signbit(tmp[0]),
                          std::signbit(tmp[1]),
                          std::signbit(tmp[2]),
                          std::signbit(tmp[3]),
                          std::signbit(tmp[4]),
                          std::signbit(tmp[5]),
                          std::signbit(tmp[6]),
                          std::signbit(tmp[7]),};
        benchmark::DoNotOptimize(result);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(sizeof(float)) * 8);
    state.SetItemsProcessed(int64_t(state.iterations()) * 8);
}

BENCHMARK(bench_signbit_unrolled)
        ->ArgPair(-1000000, -1024)
        ->ArgPair(1024, 1000000)
        ->ArgPair(-1000000, 1000000);

static void bench_check_with_sprintf_unrolled(benchmark::State &state)
{
    auto floats = ring_buff<float>(static_cast<float>(state.range(0)), static_cast<float>(state.range(1)));

    for (auto _ : state) {
        float tmp[8];
        floats.get_8(tmp);
        bool result[8] = {check_with_sprintf(tmp[0]),
                          check_with_sprintf(tmp[1]),
                          check_with_sprintf(tmp[2]),
                          check_with_sprintf(tmp[3]),
                          check_with_sprintf(tmp[4]),
                          check_with_sprintf(tmp[5]),
                          check_with_sprintf(tmp[6]),
                          check_with_sprintf(tmp[7]),};
        benchmark::DoNotOptimize(result);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(sizeof(float)) * 8);
    state.SetItemsProcessed(int64_t(state.iterations()) * 8);
}

BENCHMARK(bench_check_with_sprintf_unrolled)
        ->ArgPair(-1000000, -1024)
        ->ArgPair(1024, 1000000)
        ->ArgPair(-1000000, 1000000);

static void bench_check_with_sprintf_new_unrolled(benchmark::State &state)
{
    auto floats = ring_buff<float>(static_cast<float>(state.range(0)), static_cast<float>(state.range(1)));

    for (auto _ : state) {
        float tmp[8];
        floats.get_8(tmp);
        bool result[8] = {check_with_sprintf_new(tmp[0]),
                          check_with_sprintf_new(tmp[1]),
                          check_with_sprintf_new(tmp[2]),
                          check_with_sprintf_new(tmp[3]),
                          check_with_sprintf_new(tmp[4]),
                          check_with_sprintf_new(tmp[5]),
                          check_with_sprintf_new(tmp[6]),
                          check_with_sprintf_new(tmp[7]),};
        benchmark::DoNotOptimize(result);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(sizeof(float)) * 8);
    state.SetItemsProcessed(int64_t(state.iterations()) * 8);
}

BENCHMARK(bench_check_with_sprintf_new_unrolled)
    ->ArgPair(-1000000, -1024)
    ->ArgPair(1024, 1000000)
    ->ArgPair(-1000000, 1000000);
