#pragma once
#include <mutex>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>

template <class T> class ShmObject 
{
private:
public:
	int sharedMemoryID;
	T* sharedMemoryPtr;
	ShmObject();
	ShmObject(int);
	void Delete();
	~ShmObject();  
	unsigned int numberOfProcesses();
};

template <class T> ShmObject<T>::ShmObject() 
{    
	sharedMemoryID = shmget(IPC_PRIVATE, sizeof(T), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	sharedMemoryPtr = (T*) shmat(sharedMemoryID, 0, 0);
}

template <class T> ShmObject<T>::ShmObject(int sharedMemoryIDToAttach)
{
	sharedMemoryID = sharedMemoryIDToAttach;
	sharedMemoryPtr = (T*) shmat(sharedMemoryID, 0, 0);
}

template <class T> ShmObject<T>::~ShmObject()
{
	shmdt(sharedMemoryPtr);
	shmctl(sharedMemoryID, IPC_RMID, 0);
}

 template <class T> void ShmObject<T>::Delete()
{
	shmdt(sharedMemoryPtr);
	shmctl(sharedMemoryID, IPC_RMID, 0);
}


template <class T> unsigned int ShmObject<T>::numberOfProcesses() 
{
	struct shmid_ds shm_segment;
	int ret = shmctl(sharedMemoryID, IPC_STAT, &shm_segment);
	return shm_segment.shm_nattch;
}
