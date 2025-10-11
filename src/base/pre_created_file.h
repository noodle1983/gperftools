#ifndef PRE_CREATED_FILE_H
#define PRE_CREATED_FILE_H

#include "logging.h"
#include "spinlock.h"

class PreCreateFile {
public:
	PreCreateFile()
		:fd_(kIllegalRawFD), filename_{0}
	{
	}

	~PreCreateFile() {
		SpinLockHolder l(&lock_);
		if (fd_ != kIllegalRawFD) {
			RawClose(fd_);
		}
	}

	inline bool Prepared()
	{
		return fd_ != kIllegalRawFD;
	}

	inline void CheckPrepare()
	{
		if (fd_ != kIllegalRawFD) { return; }
		if (strlen(filename_) == 0) { return; }
		if (lock_.TryLock()) {
			if ((fd_ == kIllegalRawFD) && (strlen(filename_) > 0)) {
				fd_ = RawOpenForWriting(filename_);
			}
			lock_.Unlock();
		}
	}

	// not response for the close of the raw fd!!!
	inline RawFD GetCurrentFd(const char* nextfilename) {
		if (lock_.TryLock()) {
			snprintf(filename_, sizeof(filename_) - 1, "%s", nextfilename);
			RawFD ret = fd_;

			fd_ = kIllegalRawFD;
			lock_.Unlock();
			return ret;
		}
		return kIllegalRawFD;
	}

	inline void Close()
	{
		if (fd_ != kIllegalRawFD) {
			SpinLockHolder l(&lock_);
			if (fd_ != kIllegalRawFD) {
				RawClose(fd_);
				fd_ = kIllegalRawFD;
			}
		}
	}

private:
	volatile RawFD fd_;
	SpinLock lock_;
	char filename_[256];
};

#endif