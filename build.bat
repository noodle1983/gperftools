mkdir build_win
cd build_win
cmake .. -T v142 -Dgperftools_build_minimal=OFF -DGPERFTOOLS_BUILD_HEAP_PROFILER=ON
