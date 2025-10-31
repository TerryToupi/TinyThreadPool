#include <TinyThreadPool.h>

#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>
#include <random>

using namespace TinyThreadPool;

int main() {
    std::cout << "=== ThreadPool Test Start ===\n";

    Initialize();

    // --- 1. Basic execution test ---
    {
        std::atomic<int> counter = 0;
        const int taskCount = 100;

        for (int i = 0; i < taskCount; ++i) {
            Execute([&counter]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                ++counter;
                });
        }

        Wait();

        if (counter == taskCount)
            std::cout << "[PASS] All jobs executed (" << counter << ")\n";
        else
            std::cout << "[FAIL] Missing jobs (" << counter << "/" << taskCount << ")\n";
    }

    // --- 2. Parallelism check ---
    {
        using namespace std::chrono;
        const int taskCount = 8;
        std::atomic<int> counter = 0;

        auto start = steady_clock::now();
        for (int i = 0; i < taskCount; ++i) {
            Execute([&counter]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                ++counter;
                });
        }

        Wait();
        auto duration = duration_cast<milliseconds>(steady_clock::now() - start).count();

        // If tasks ran serially, time would be ~800ms. Parallel → much smaller.
        if (duration < 400)
            std::cout << "[PASS] Tasks executed in parallel (" << duration << "ms)\n";
        else
            std::cout << "[WARN] Tasks took long (" << duration << "ms) — check thread count\n";
    }

    // --- 3. Busy() / Wait() behavior ---
    {
        std::atomic<bool> running = false;
        Execute([&running]() {
            running = true;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            running = false;
            });

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (Busy())
            std::cout << "[PASS] Busy() reports active work\n";
        else
            std::cout << "[FAIL] Busy() did not detect running job\n";

        Wait();
        if (!Busy())
            std::cout << "[PASS] Busy() false after Wait()\n";
        else
            std::cout << "[FAIL] Busy() true after Wait()\n";
    }

    // --- 4. Stress test ---
    {
        constexpr int totalBatches = 50;
        constexpr int tasksPerBatch = 1000;
        std::atomic<size_t> totalExecuted = 0;

        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> delayDist(0, 3); // small random work duration (ms)

        auto start = std::chrono::steady_clock::now();

        for (int batch = 0; batch < totalBatches; ++batch) {
            for (int i = 0; i < tasksPerBatch; ++i) {
                Execute([&]() {
                    // Simulate random lightweight work
                    int d = delayDist(rng);
                    if (d)
                        std::this_thread::sleep_for(std::chrono::milliseconds(d));
                    totalExecuted.fetch_add(1, std::memory_order_relaxed);
                    });
            }

            // Occasionally call Wait() mid-stream to flush
            if (batch % 10 == 0) {
                Wait();
                std::cout << "[Batch " << batch << "] partial flush complete\n";
            }

            // Random short sleep between submissions
            if (batch % 5 == 0)
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        // Wait for all remaining jobs
        Wait();

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - start
        ).count();

        const size_t expected = static_cast<size_t>(totalBatches) * tasksPerBatch;
        bool success = (totalExecuted == expected);

        std::cout << "\n=== Results ===\n";
        std::cout << "Expected tasks: " << expected << "\n";
        std::cout << "Executed tasks: " << totalExecuted << "\n";
        std::cout << "Total time: " << elapsed << " ms\n";

        if (success)
            std::cout << "[PASS] All tasks executed correctly under stress.\n";
        else
            std::cout << "[FAIL] Some tasks were dropped or not executed.\n";
    }

    // --- 4. Shutdown test ---
    Shutdown();
    std::cout << "[INFO] Shutdown completed successfully.\n";
    std::cout << "=== ThreadPool Test Finished ===\n";

    return 0;
}
