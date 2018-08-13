#pragma once

typedef void(*HANDLE_FUNC)();

template <int inst> // 预留参数 instance
class __MallocAllocTemplate {
private:

	static HANDLE_FUNC __malloc_alloc_oom_handler;

	static void* OOM_Malloc(size_t n)
	{
		while (1)
		{
			if (__malloc_alloc_oom_handler == 0)
			{
				throw bad_alloc();
			}
			else
			{
				__malloc_alloc_oom_handler();
				void* ret = malloc(n);
				if (ret)
					return ret;
			}
		}
	}
public:
	static void* Allocate(size_t n)
	{
		void *result = malloc(n);
		if (0 == result)
			result = OOM_Malloc(n);

		return result;
	}

	static void Deallocate(void *p, size_t /* n */)
	{
		free(p);
	}

	static HANDLE_FUNC SetMallocHandler(HANDLE_FUNC f)
	{
		HANDLE_FUNC old = f;
		__malloc_alloc_oom_handler = f;
		return old;
	}
};

template<int inst>
HANDLE_FUNC __MallocAllocTemplate<inst>::__malloc_alloc_oom_handler = 0;

void FreeMemory()
{
	cout << "释放内存" << endl;
}

void TestAlloc1()
{
	__MallocAllocTemplate<0>::SetMallocHandler(FreeMemory);

	void* p = __MallocAllocTemplate<0>::Allocate(0x7fffffff);
	__MallocAllocTemplate<0>::Deallocate(p, 40);
}

///////////////////////////////////////////////////////////////////////

template <bool threads, int inst>
class __DefaultAllocTemplate
{
public:
	// 65	72		8
	// 72	79
	static size_t FREELIST_INDEX(size_t n)
	{
		return ((n + __ALIGN - 1) / __ALIGN - 1);
	}

	// 65	72	-> 72
	// 72	79
	static size_t ROUND_UP(size_t bytes)
	{
		return (((bytes)+__ALIGN - 1) & ~(__ALIGN - 1));
	}

	static void* ChunkAlloc(size_t size, size_t& nobjs)
	{
		size_t totalbytes = nobjs * size;
		size_t leftbytes = _endfree - _startfree;

		if (leftbytes >= totalbytes)
		{
			void* ret = _startfree;
			_startfree += totalbytes;
			return ret;
		}
		else if (leftbytes > size)
		{
			nobjs = leftbytes / size;
			totalbytes = size * nobjs;
			void* ret = _startfree;
			_startfree += totalbytes;
			return ret;
		}
		else
		{
			//先处理掉剩余的小块内存
			if (leftbytes > 0)
			{
				size_t index = FREELIST_INDEX(leftbytes);
				((Obj*)_startfree)->_freelistlink = _freelist[index];
				_freelist[index] = (Obj*)_startfree;
			}
			//申请
			size_t bytesToGet = totalbytes * 2 + ROUND_UP(_heapsize >> 4);
			_startfree = (char*)malloc(bytesToGet);

			if (_startfree == NULL)
			{
				//到更大的自由链表中找
				size_t index = FREELIST_INDEX(size);
				for (; index < __NFREELISTS; ++index)
				{
					if (_freelist[index])
					{
						Obj* obj = _freelist[index];
						_freelist[index] = obj->_freelistlink;
						return ChunkAlloc(size, nobjs);
					}
				}
				//山穷水尽，最后一搏
				_startfree = (char*)__MallocAllocTemplate<0>::Allocate(bytesToGet);
			}
			_heapsize += bytesToGet;
			_endfree = _startfree + bytesToGet;
			return ChunkAlloc(size, nobjs);
		}
	}

	static void* Refill(size_t bytes)
	{
		size_t nobjs = 20;             //为了减少并发时锁的力度
		char* chunk = ChunkAlloc(bytes, nobjs);
		if (nobjs == 1)
			return chunk;

		size_t index = FREELIST_INDEX(bytes);

		Obj* cur = (Obj*)(chunk + bytes);
		_freelist[index] = cur;
		for (size_t i = 0; i < nobjs - 2; ++i)
		{
			Obj* next = (Obj*)((char*)cur + bytes);
			cur->_freelistlink = next;

			cur = next;
		}

		cur->_freelistlink = NULL;

		return chunk;
	}

	static void* Allocate(size_t n)
	{
		if (n > __MAX_BYTES)
		{
			return __MallocAllocTemplate<0>::Allocate(n);
		}

		size_t index = FREELIST_INDEX(n);
		if (_freelist[index] == NULL)
		{
			return Refill(ROUND_UP(n));
		}
		else
		{
			Obj* ret = _freelist[index];
			_freelist[index] = ret->_freelistlink;
			return ret;
		}
	}

	static void Deallocate(void *p, size_t  n)
	{
		if (n > __MAX_BYTES)
		{
			__MallocAllocTemplate<0>::Deallocate(p, n);
		}
		else
		{
			size_t index = FREELIST_INDEX(n);
			((Obj*)p)->_freelistlink = _freelist[index];
			_freelist[index] = (Obj*)p;
		}
	}

private:
	enum { __ALIGN = 8 };
	enum { __MAX_BYTES = 128 };
	enum { __NFREELISTS = __MAX_BYTES / __ALIGN };

	union Obj
	{
		union Obj* _freelistlink;
		char client_data[1];    /* The client sees this.        */
	};

	// 自由链表
	static Obj* _freelist[__NFREELISTS];

	// 内存池
	static char* _startfree;
	static char* _endfree;
	static size_t _heapsize;
};

template<bool threads, int inst>
typename __DefaultAllocTemplate<threads, inst>::Obj*;

template<bool threads, int inst>
__DefaultAllocTemplate<threads, inst>::_freelist[__NFREELISTS] = { 0 };

// 内存池
template<bool threads, int inst>
char* __DefaultAllocTemplate<threads, inst>::_startfree = NULL;

template<bool threads, int inst>
char* __DefaultAllocTemplate<threads, inst>::_endfree = NULL;

template<bool threads, int inst>
size_t __DefaultAllocTemplate<threads, inst>::_heapsize = 0;

void TestAlloc2()
{
	for (size_t i = 0; i < 20; ++i)
	{
		__DefaultAllocTemplate<false, 0>::Allocate(6);
	}

	__DefaultAllocTemplate<false, 0>::Allocate(6);
}


#ifdef __USE_MALLOC
typedef __MallocAllocTemplate<0> Alloc;
#else
typedef __DefaultAllocTemplate<false, 0> Alloc;
#endif


template<class T, class Alloc>
class SimpleAlloc {
public:
	static T* Allocate(size_t n)
	{
		return 0 == n ? 0 : (T*)Alloc::Allocate(n * sizeof (T));
	}

	static T* Allocate(void)
	{
		return (T*)Alloc::Allocate(sizeof (T));
	}

	static void Deallocate(T *p, size_t n)
	{
		if (0 != n)
			Alloc::Deallocate(p, n * sizeof (T));
	}

	static void Deallocate(T *p)
	{
		Alloc::Deallocate(p, sizeof (T));
	}
};
