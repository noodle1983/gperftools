#ifndef AUTO_PROFILE_H
#define AUTO_PROFILE_H

#include <gperftools/malloc_extension.h>
#include "base/spinlock.h"
#include "gperftools/heap-profiler.h"

#include <atomic>
#include <fstream>
#include <stdio.h>

static std::atomic<bool> is_inited = false;
static std::atomic<bool> enabled = true;
static std::atomic<size_t> auto_tick = 0;
static std::atomic<time_t> pre_check_time = 0;
static SpinLock auto_start_lock;
static size_t START_PROFILE_SIZE = (1 << 30); // 1GB
#include <stdio.h>

static inline std::string my_trim(const std::string& str) {
	size_t start = str.find_first_not_of(" \t");
	if (start == std::string::npos) return "";

	size_t end = str.find_last_not_of(" \t");
	return str.substr(start, end - start + 1);
}

static inline std::string my_to_upper(const std::string& str) {
	std::string result = str;
	std::transform(result.begin(), result.end(), result.begin(), ::toupper);
	return result;
}

int file_exists(const char* filename) {
	FILE* file = fopen(filename, "r");
	if (file) {
		fclose(file);
		return 1;
	}
	return 0;
}

void check_start_profile()
{
	if (auto_tick++ < 100) { return; }
	auto_tick = 0;

	if (pre_check_time == 0) {
		pre_check_time = time(0);
		return;
	}

	if (!is_inited) {
		enabled = file_exists("tcmalloc.profile.enable");
		is_inited = true;

		std::ifstream file("tcmalloc.profile.enable");
		std::string line;
		while (std::getline(file, line)) {
			if (line.empty() || line[0] == '#') {
				continue;
			}

			size_t pos = line.find('=');
			if (pos != std::string::npos) {
				std::string key = line.substr(0, pos);
				std::string value = line.substr(pos + 1);

				key.erase(0, key.find_first_not_of(" \t"));
				key.erase(key.find_last_not_of(" \t") + 1);
				value.erase(0, value.find_first_not_of(" \t"));
				value.erase(value.find_last_not_of(" \t") + 1);

				if (key == "START_PROFILE_SIZE") {
					std::string num_str;
					std::string unit_str;
					size_t i = 0;
					while (i < value.length() && (std::isdigit(value[i]) || value[i] == '.')) { i++; }
					if (i > 0) {
						num_str = value.substr(0, i);
						unit_str = my_to_upper(my_trim(value.substr(i)));
					}

					double d = std::stod(num_str);
					if (!unit_str.empty()) {
						if (unit_str == "KB" || unit_str == "K") {
							d *= 1024;
						}
						else if (unit_str == "MB" || unit_str == "M") {
							d *= 1024 * 1024;
						}
						else if (unit_str == "GB" || unit_str == "G") {
							d *= 1024 * 1024 * 1024;
						}
						else if (unit_str == "TB" || unit_str == "T") {
							d *= 1024 * 1024 * 1024 * 1024ULL;
						}
					}
					START_PROFILE_SIZE = (size_t)d;
				} else {
					printf("profile enabled. unknown config:%s = %s\n", key.c_str(), value.c_str());
				}
			}
		}
		file.close();
		printf("profile enabled. START_PROFILE_SIZE:%zu\n", START_PROFILE_SIZE);
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
	printf("profile enabled:%d, Heap Stat: %s\n", (int)enabled, buf);

	size_t heap_size;
	MallocExtension::instance()->GetNumericProperty("generic.heap_size", &heap_size);
	PERFTOOLS_DLL_DECL int IsHeapProfilerRunning();
	if (enabled 
		&& heap_size > START_PROFILE_SIZE
		&& !IsHeapProfilerRunning())
	{
		char fname[512] = { 0 };
#ifdef _WIN32
	    snprintf(fname, sizeof(fname) - 1, "heap_profile_%d", _getpid());
#else
        snprintf(fname, sizeof(fname) - 1, "heap_profile_%d", getpid());
#endif
		HeapProfilerStart(fname);
	}
}

#endif /* AUTO_PROFILE_H */
