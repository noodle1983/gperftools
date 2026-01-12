#include <stdint.h>
extern "C" int IsHeapProfilerRunning(){return 0;}
extern "C" void HeapProfilerStart(const char * prefix){}
extern "C" void HeapProfilerStop(){}
extern "C" void SetHeapProfilerIUseInterval(int64_t interval){}
extern "C" int64_t GetHeapProfilerIUseInterval(){return 0;}

