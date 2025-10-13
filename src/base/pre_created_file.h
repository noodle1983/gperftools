#ifndef PRE_CREATED_FILE_H
#define PRE_CREATED_FILE_H

#include "logging.h"
#include "spinlock.h"
class PreCreateFile {
public:
	typedef const char* (*MakeFileNameFunc)();
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

	inline void CheckPrepare(MakeFileNameFunc make_filename)
	{
		if (fd_ != kIllegalRawFD) { return; }
		if (lock_.TryLock()) {
			if (fd_ == kIllegalRawFD) {
				const char* filename = make_filename();
				size_t namelen = strlen(filename);
				if (namelen == 0 || namelen >= sizeof(filename_)) { return; }
				strcpy(filename_, filename);

				fd_ = RawOpenForWriting(filename_);
			}
			lock_.Unlock();
		}
	}

	// not response for the close of the raw fd!!!
	inline RawFD GetCurrentFd() {
		if (lock_.TryLock()) {
			RawFD ret = fd_;

			fd_ = kIllegalRawFD;
			lock_.Unlock();
			return ret;
		}
		return kIllegalRawFD;
	}

	inline const char* GetCurrentFilename() {
		return filename_;
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
	char filename_[1024];
};

#endif