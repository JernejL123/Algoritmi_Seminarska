#pragma once
#include <cmath>
#include <vector>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <algorithm>

// ==============================================================
//  NASTAVI POT DO CSV
// ==============================================================
static const std::string CSV_PATH = "C:/Users/jerne/Documents/ATURP_Seminarska/benchmark_results.csv";

// ==============================================================
//  RANGES
// ==============================================================
static const long N_MIN = 2,  N_MAX = 1000;
static const long K_MIN = 1,  K_MAX = 1000;
static const long P_MIN = 2,  P_MAX = 1000;

static const long FIXED_N = 7;
static const long FIXED_K = 3;
static const long FIXED_P = 7;

static const double MAX_SECONDS = 6000.0;  // preskoči če dlje

// ==============================================================
//  Deklaracija — pride iz tvojega .cpp
// ==============================================================
long calculateNumOfPINs(long N, long K, long P);

// ==============================================================
//  CSV
// ==============================================================
static void csv_write_header() {
    std::ofstream csv(CSV_PATH, std::ios::trunc);
    csv << "timestamp,benchmark,parameter,value,fixed_params,time_ms\n";
    csv.close();
    std::cout << "CSV inicializiran: " << CSV_PATH << "\n\n";
}

static void csv_append(const std::string& bm, const std::string& param,
                        long value, const std::string& fixed, double time_ms) {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    char ts[32];
    std::strftime(ts, sizeof(ts), "%Y-%m-%dT%H:%M:%S", std::localtime(&t));
    std::ofstream csv(CSV_PATH, std::ios::app);
    csv << ts << "," << bm << "," << param << "," << value
        << ",\"" << fixed << "\"," << time_ms << "\n";
    csv.flush();
    csv.close();
}

// ==============================================================
//  Meritev + ocena
// ==============================================================
static double measure_once(long N, long K, long P) {
    auto start = std::chrono::high_resolution_clock::now();
    volatile long res = calculateNumOfPINs(N, K, P);
    (void)res;
    return std::chrono::duration<double, std::milli>(
        std::chrono::high_resolution_clock::now() - start).count();
}

static double iters_per_ms = 0;

static void calibrate() {
    double t = measure_once(4, 2, 4);  // 4^4 = 256 iteracij
    iters_per_ms = 256.0 / std::max(t, 0.001);
    std::cout << "Kalibracija: " << (long)iters_per_ms << " iter/ms\n\n";
}

static double estimate_ms(long N, long P) {
    uint64_t est = (uint64_t)pow((double)N, (double)P);
    return est / iters_per_ms;
}

// ==============================================================
//  Job struct
// ==============================================================
struct Job {
    std::string bm, param, fixed;
    long value, N, K, P;
    bool done = false;
    bool skipped = false;
};

// ==============================================================
//  MAIN
// ==============================================================
int main(int argc, char** argv) {
    csv_write_header();
    calibrate();

    // Sestavi vse jobe
    std::vector<Job> jobs;

    for (long N = N_MIN; N <= N_MAX; N++)
        jobs.push_back({"VaryN", "N",
            "K=" + std::to_string(FIXED_K) + " P=" + std::to_string(FIXED_P),
            N, N, FIXED_K, FIXED_P});

    for (long K = K_MIN; K <= K_MAX; K++)
        jobs.push_back({"VaryK", "K",
            "N=" + std::to_string(FIXED_N) + " P=" + std::to_string(FIXED_P),
            K, FIXED_N, K, FIXED_P});

    for (long P = P_MIN; P <= P_MAX; P++)
        jobs.push_back({"VaryP", "P",
            "N=" + std::to_string(FIXED_N) + " K=" + std::to_string(FIXED_K),
            P, FIXED_N, FIXED_K, P});

    int total = jobs.size();
    std::cout << "Skupaj jobov: " << total << "\n";
    std::cout << "Vrstni red: round-robin (N[0], K[0], P[0], N[1], K[1], P[1], ...)\n\n";

    // Round-robin indeksi po skupinah
    std::vector<std::vector<int>> groups(3);
    for (int i = 0; i < (int)jobs.size(); i++) {
        if (jobs[i].bm == "VaryN") groups[0].push_back(i);
        else if (jobs[i].bm == "VaryK") groups[1].push_back(i);
        else                             groups[2].push_back(i);
    }

    // Interleave: vzemi enega iz vsake grupe izmenično
    std::vector<int> order;
    size_t maxLen = std::max({groups[0].size(), groups[1].size(), groups[2].size()});
    for (size_t i = 0; i < maxLen; i++) {
        for (int g = 0; g < 3; g++)
            if (i < groups[g].size())
                order.push_back(groups[g][i]);
    }

    int done = 0, skipped = 0;
    for (int idx : order) {
        Job& j = jobs[idx];
        double est = estimate_ms(j.N, j.P);

        std::cout << "[" << j.bm << "] " << j.param << "=" << j.value
                  << " | " << j.fixed
                  << " | ocena: ";

        if (est >= 1000.0)
            std::cout << (long)(est/1000.0) << "s";
        else
            std::cout << (long)est << "ms";

        if (est / 1000.0 > MAX_SECONDS) {
            std::cout << " -> PRESKOK\n";
            csv_append(j.bm, j.param, j.value, j.fixed, -1.0);
            skipped++;
            continue;
        }

        std::cout << " -> " << std::flush;
        double t = measure_once(j.N, j.K, j.P);
        std::cout << t << " ms [zapisano]\n";
        csv_append(j.bm, j.param, j.value, j.fixed, t);
        done++;
    }

    std::cout << "\n========================================\n";
    std::cout << "Izmerjeno: " << done << " | Preskočeno: " << skipped << "\n";
    std::cout << "CSV: " << CSV_PATH << "\n";
    return 0;
}