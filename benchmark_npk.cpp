#include <benchmark/benchmark.h>
#include <cmath>
#include <set>
#include <vector>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>

// ============================================================
//  Core function (kopija iz glavnega programa)
// ============================================================
long calculateNumOfPINs(long N, long K, long P) {
    uint64_t numOfOptions = (uint64_t)pow((double)N, (double)P);
    uint64_t count = 0;

    for (uint64_t i = 0; i < numOfOptions; i++) {
        uint64_t temp = i;
        std::vector<int> pin(P);
        for (int j = 0; j < P; j++) {
            pin[j] = temp % N;
            temp = temp / N;
        }
        std::set<int> razlicne(pin.begin(), pin.end());
        if ((long)razlicne.size() >= K) count++;
    }

    long modul = (long)pow(10, 9) - 7;
    return (long)(count % modul);
}

// ============================================================
//  Benchmark: variiramo N (K in P sta fiksna)
// ============================================================
static void BM_VaryN(benchmark::State& state) {
    long N = state.range(0);
    long K = 2;   // fiksirano
    long P = 4;   // fiksirano
    for (auto _ : state) {
        benchmark::DoNotOptimize(calculateNumOfPINs(N, K, P));
    }
    state.SetLabel("K=" + std::to_string(K) + ",P=" + std::to_string(P));
}
BENCHMARK(BM_VaryN)->DenseRange(2, 8, 1)->Unit(benchmark::kMillisecond);

// ============================================================
//  Benchmark: variiramo K (N in P sta fiksna)
// ============================================================
static void BM_VaryK(benchmark::State& state) {
    long N = 5;   // fiksirano
    long K = state.range(0);
    long P = 4;   // fiksirano
    for (auto _ : state) {
        benchmark::DoNotOptimize(calculateNumOfPINs(N, K, P));
    }
    state.SetLabel("N=" + std::to_string(N) + ",P=" + std::to_string(P));
}
BENCHMARK(BM_VaryK)->DenseRange(1, 5, 1)->Unit(benchmark::kMillisecond);

// ============================================================
//  Benchmark: variiramo P (N in K sta fiksna)
// ============================================================
static void BM_VaryP(benchmark::State& state) {
    long N = 5;   // fiksirano
    long K = 2;   // fiksirano
    long P = state.range(0);
    for (auto _ : state) {
        benchmark::DoNotOptimize(calculateNumOfPINs(N, K, P));
    }
    state.SetLabel("N=" + std::to_string(N) + ",K=" + std::to_string(K));
}
BENCHMARK(BM_VaryP)->DenseRange(2, 7, 1)->Unit(benchmark::kMillisecond);

// ============================================================
//  Custom main: požene benchmarke + zapiše CSV
// ============================================================
int main(int argc, char** argv) {
    // --- 1. Poženi Google Benchmark normalno ---
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;

    // Preusmerimo izhod v spomin (benchmark piše v cout/stderr)
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();

    // --- 2. Ročno izmeri in zapiši CSV ---
    std::ofstream csv("benchmark_results.csv");
    csv << "benchmark,parameter,value,fixed_params,time_ms\n";

    auto measure = [](long N, long K, long P) -> double {
        auto start = std::chrono::high_resolution_clock::now();
        benchmark::DoNotOptimize(calculateNumOfPINs(N, K, P));
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count();
    };

    // Variacaja N (K=2, P=4)
    for (long N = 2; N <= 8; N++) {
        double t = measure(N, 2, 4);
        csv << "VaryN,N," << N << ",\"K=2,P=4\"," << t << "\n";
        std::cout << "[VaryN] N=" << N << " K=2 P=4  -> " << t << " ms\n";
    }

    // Variacija K (N=5, P=4)
    for (long K = 1; K <= 5; K++) {
        double t = measure(5, K, 4);
        csv << "VaryK,K," << K << ",\"N=5,P=4\"," << t << "\n";
        std::cout << "[VaryK] N=5 K=" << K << " P=4  -> " << t << " ms\n";
    }

    // Variacija P (N=5, K=2)
    for (long P = 2; P <= 7; P++) {
        double t = measure(5, 2, P);
        csv << "VaryP,P," << P << ",\"N=5,K=2\"," << t << "\n";
        std::cout << "[VaryP] N=5 K=2 P=" << P << "  -> " << t << " ms\n";
    }

    csv.close();
    std::cout << "\nRezultati zapisani v: benchmark_results.csv\n";
    return 0;
}
