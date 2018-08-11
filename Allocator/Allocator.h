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
				((Obj*)_startfree)->_freelistlink = 
			}
			//申请
			size_t bytesToGet = totalbytes * 2 + ROUND_UP(_heapsize >> 4);
			_startfree = (char*)malloc(bytesToGet);
			if (_startfree == NULL)
			{
				for (int i = size; i < __MAX_BYTES; i += __ALIGN)
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

template <bool threads, int inst>
typename __DefaultAllocTemplate<threads, inst>::Obj*;
typename __DefaultAllocTemplate
// 自由链表
static Obj* _freelist[__NFREELISTS];

// 内存池
static char* _startfree;
static char* _endfree;
static size_t _heapsize;
