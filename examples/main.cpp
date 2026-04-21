#define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "../include/apespeed.h"
#include <string>
#include <iostream>
// #include <chrono>
// #include <omp.h>
#include "../src/parsers/ApeF.h"

int main(int argc, char const *argv[])
{
    // Validate arguments
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <palette_file> [output_name]" << std::endl;
        return 1;
    }

    ApeF apef;
    std::string input_file = argv[1];
    std::string palette_file = argv[2];
    int num_threads = 1;
    std::string output_name = "output";
    std::cout << "Opening " << input_file << std::endl;
    std::cout << "Using palette " << palette_file << std::endl;

    // set number of threads
    // if (num_threads >= 4) {
    //     num_threads = std::stoi(argv[3]);
    //     std::cout << "Number of threads set to " << num_threads << std::endl;
    // } else {
    //     std::cout << "Number of threads set to 1" << std::endl;
    //     num_threads = 1;
    // }

    // set output name
    if (argc >= 4) {
        output_name = argv[3];
        std::cout << "Output name set to " << output_name << std::endl;
    } else {
        std::cout << "Output name set to default" << std::endl;
    }

    // omp_set_num_threads(num_threads);

    // // Start test 
    // double total_time = 0.0;
    // auto start = std::chrono::high_resolution_clock::now();

    if (!apef.load(input_file, 0, palette_file)) {
        std::cerr << "Failed to load file: " << input_file << std::endl;
        return 1;
    }
    // auto end = std::chrono::high_resolution_clock::now();


    // std::chrono::duration<double> runtime = end - start;
    // total_time += runtime.count();
    // std::cout << "Time taken to decode " << apef.getFrameCount() << " frames: " << total_time << " seconds" << std::endl;
    // std::cout << "Average time per frame: " << total_time / apef.getFrameCount() << " seconds" << std::endl;

    // NOTE: UNCOMMENT THIS TO WRITE PNGs
    int numBuffers = apef.getFrameCount();
    std::cout << "Writing PNGs" << std::endl;
    int successCount = 0;
    for (int i = 0; i < numBuffers; i++) {
        int error = apef.exportToPng(output_name + std::to_string(i) + ".png", apef.getFrameBuffer()[i]);
        if (error < 0) {
            std::cout << "Failed to write frame " << i << " to png" << std::endl;
        } else {
            successCount++;
        }
    }

    // std::cout << "Finished writing " << successCount << " frames to PNG with " << numBuffers - successCount << " errors." << std::endl;
    
    return 0;
}