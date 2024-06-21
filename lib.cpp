#include <iostream>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <thread>
#include <cmath>

void heavy_computation(int index)
{
    // Simulate a heavy computation task
    double result = 0.0;
    for (int i = 0; i < 10000; ++i)
    {
        result+= sin(index) * std::cos(i);
        std::cout<<i<<std::endl;
    }
    std::cout << "Index: " << index << ", Result: " << result << std::endl;
}
/*
void thread_task(int start, int end, int thread_id) {
    for (int i = start; i < end; ++i) {
        heavy_computation(i);
        std::cout << "Thread index: " << thread_id << std::endl;
    }
}

void parallel_task() 
{   
    std::cout<<"step0"<<std::endl;
    // Get the number of concurrent threads supported by the hardware
    auto concurrency = std::thread::hardware_concurrency();
    
    // Determine the range of values each thread will process
    int total_tasks = 20;
    int tasks_per_thread = total_tasks / concurrency;
    int remaining_tasks = total_tasks % concurrency;
    
    // Vector to hold the threads
    std::vector<std::thread> threads;

    int start = 0;
    std::cout<<"step1"<<std::endl;
    threads.resize(concurrency);
    YZJ:The number of allocated threads is consistent with the thread pool size specified in the compilation options.
    for (unsigned int i = 0; i < 4; ++i) 
    {   
        std::cout<<"thid"<<i<<std::endl;
        int end = start + tasks_per_thread;
        if (i < remaining_tasks) {
            ++end;  // Distribute remaining tasks
        }
        // threads.emplace_back(thread_task, start, end, i);
        threads[i] =std::thread(thread_task, start, end, i);
        start = end;
    }
    std::cout<<"step2"<<std::endl;
    // Join all threads
    for (auto& thread : threads) {
        std::cout<<"step3"<<std::endl;
        thread.join();
    }
}
*/
void parallel_task() {
    // Get the number of concurrent threads supported by the hardware
    auto concurrency = std::thread::hardware_concurrency();
    
    // Create a task arena with the specified concurrency level and high priority
    //YZJ:The number of allocated threads is consistent with the thread pool size specified in the compilation options.
    tbb::task_arena arena(4, 1, tbb::task_arena::priority::high);
    
    // Execute the parallel task within the arena
    arena.execute([&] {
        // Define the range and the body of the parallel loop
        tbb::parallel_for(tbb::blocked_range<int>(0, 100),
                          [](const tbb::blocked_range<int>& range) {
            for (int i = range.begin(); i < range.end(); ++i) {
                // Get the current thread index
                int tid = tbb::this_task_arena::current_thread_index();
                
                // Perform heavy computation
                heavy_computation(i);
                
                // Output the thread index
                std::cout << "Thread index: " << tid << std::endl;
            }
        });
    });
}

/*
int main() {
    std::cout << "Running oneTBB with Emscripten!" << std::endl;
    parallel_task();
    return 0;
}
*/

#include <emscripten/bind.h>
#include <emscripten/val.h>
using namespace emscripten;
EMSCRIPTEN_BINDINGS(my_module)
{
    function("parallel_task", &parallel_task);
}
