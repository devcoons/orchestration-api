#pragma once

	#include <mutex>
	#include <sys/ipc.h>
	#include <sys/shm.h>
	#include <sys/stat.h>
	#include <string>
	#include <string.h>
	#include <iostream>

	template <class T> class ShmList
	{
		private:
			std::mutex mux;
			int elementsCount;
			
		public:
			int sharedMemoryID;
			ShmList();
			~ShmList();
			ShmList(int);
			int getSize();	
			void getElements(T *);
			void insertElement();
			void removeElement(int);
			unsigned int numberOfProcesses();
	};

	template <class T> int ShmList<T>::getSize()
	{
		mux.lock();
		auto auto_var = elementsCount;
		mux.unlock();
		return auto_var;
	}

	template <class T> ShmList<T>::~ShmList()
	{
		if (sharedMemoryID == 0)
			return;
		shmctl(sharedMemoryID, IPC_RMID, 0);
	}

	template <class T> void ShmList<T>::removeElement(int index)
	{
		if (index > elementsCount) return;
		elementsCount--;
		int ids = shmget(IPC_PRIVATE, elementsCount*sizeof(T), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
		T * newspace = (T*)shmat(ids, 0, 0);
		T* ptr = (T*)shmat(sharedMemoryID, 0, 0);
		memcpy(newspace, ptr, index*sizeof(T));
		memcpy(newspace + index, ptr + 1 + index, (elementsCount - index)*sizeof(T));
		shmdt(ptr);	
		shmdt(newspace);
		shmctl(sharedMemoryID, IPC_RMID, 0);
		sharedMemoryID = ids;
	}

	template <class T> ShmList<T>::ShmList()
	{
		elementsCount = 0;
		sharedMemoryID = shmget(IPC_PRIVATE, elementsCount*sizeof(T), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	}

	template <class T> ShmList<T>::ShmList(int ID)
	{
		sharedMemoryID = shmget(ID, elementsCount*sizeof(T), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
		struct shmid_ds shm_segment;
		int ret = shmctl(sharedMemoryID, IPC_STAT, &shm_segment);
		elementsCount = shm_segment.shm_segsz / sizeof(T);
	}

	template <class T> void ShmList<T>::insertElement()
	{
		elementsCount++;
		int ids = shmget(IPC_PRIVATE, elementsCount*sizeof(T), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
		T * newspace = (T*)shmat(ids, 0, 0);
		T * sharedMemoryPtr = (T*)shmat(sharedMemoryID, 0, 0);
		if (elementsCount > 1)
		{
			struct shmid_ds shm_segment;
			int ret = shmctl(sharedMemoryID, IPC_STAT, &shm_segment);
			memcpy(newspace, sharedMemoryPtr, shm_segment.shm_segsz);
			shmctl(sharedMemoryID, IPC_RMID, 0);
		}
		shmdt(newspace);
		shmdt(sharedMemoryPtr);	
		sharedMemoryID = ids;
	}

	template <class T> void ShmList<T>::getElements(T * sman)
	{
		sman = (T * )shmat(sharedMemoryID, 0, 0);
	}

	template <class T> unsigned int ShmList<T>::numberOfProcesses() 
	{
		struct shmid_ds shm_segment;
		int ret = shmctl(sharedMemoryID, IPC_STAT, &shm_segment);
		return shm_segment.shm_nattch;
	}