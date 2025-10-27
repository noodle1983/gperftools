#ifndef AUTO_PROFILE_H
#define AUTO_PROFILE_H

#include <gperftools/malloc_extension.h>
#include "base/spinlock.h"
#include "gperftools/heap-profiler.h"

#include <atomic>
#include <stdio.h>

static std::atomic<size_t> auto_tick = 0;
static std::atomic<time_t> pre_check_time = 0;
static SpinLock auto_start_lock;
void check_start_profile()
{
	if (auto_tick++ < 100) { return; }
	auto_tick = 0;

	if (pre_check_time == 0) {
		pre_check_time = time(0);
		return;
	}

	time_t now = time(0);
	if (now < pre_check_time + 60) {
		return;
	}
    SpinLockHolder l(&auto_start_lock);
	if (now < pre_check_time + 60) {
		return;
	}

	pre_check_time = now;

	char buf[512] = { 0 };
	MallocExtension::instance()->GetStats(buf, sizeof(buf) - 1);
	printf("Heap Stat: %s\n", buf);

	size_t heap_size;
	MallocExtension::instance()->GetNumericProperty("generic.heap_size", &heap_size);
	PERFTOOLS_DLL_DECL int IsHeapProfilerRunning();
	if (heap_size >  (1 << 30) // 1GB
		&& IsHeapProfilerRunning())
	{
		HeapProfilerStart("heap_profile");
	}
}

#endif /* AUTO_PROFILE_H */
