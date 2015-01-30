#pragma once

#include "rgl_addresses.h"

#define rgl_new_func ((void *(__cdecl *)(size_t))rgl::addresses::New)
#define rgl_delete_func ((void (__cdecl *)(void *))rgl::addresses::Delete)

#define MAX_RECURSIVE_CALLS 512

namespace rgl
{
	struct x86_register_set
	{
		unsigned int eax;
		unsigned int ebx;
		unsigned int ecx;
		unsigned int edx;
		unsigned int edi;
		unsigned int esi;
		unsigned int ebp;
	};

	struct register_store
	{
		static unsigned int esp;
		static x86_register_set sets[MAX_RECURSIVE_CALLS]; // safe for up to MAX_RECURSIVE_CALLS recursive calls.
		static x86_register_set *current_set;
	};

	struct register_store_net
	{
		static unsigned int esp;
		static x86_register_set sets[MAX_RECURSIVE_CALLS];
		static x86_register_set *current_set;
	};

	struct register_store_gfx
	{
		static unsigned int esp;
		static x86_register_set sets[MAX_RECURSIVE_CALLS];
		static x86_register_set *current_set;
	};

	void *_alloc(size_t size);
	void _free(void *ptr);
	
	template <typename T>
	T *_allocate()
	{
		return (T *)rgl_new_func(sizeof(T));
	};

	template <typename T>
	T *_allocate(size_t size)
	{
		return (T *)rgl_new_func(size * sizeof(T));
	};
	
	template <typename T>
	void _deallocate(T *ptr)
	{
		rgl_delete_func(ptr);
	};

	template <typename T>
	void _construct(T *ptr)
	{
		new(ptr) T;
	};

	template <typename T>
	void _construct(T *ptr, const T &val)
	{
		new(ptr) T(val);
	};

	template <typename T>
	void _destroy(T *ptr)
	{
		ptr->~T();
	};

	template <typename T>
	T *_new()
	{
		T *buf = _allocate<T>();
		_construct<T>(buf);

		return buf;
	};

	template <typename T>
	T *_new(size_t size)
	{
		if (!size)
			return nullptr;

		T *buf = _allocate<T>(size);

		for (size_t i = 0; i < size; ++i)
		{
			_construct<T>(buf + i);
		}

		return buf;
	};

	template <typename T>
	T *_new_vec(size_t size)
	{
		if (!size)
			return nullptr;

		T *buf = (T *)rgl_new_func(size * sizeof(T) + 4);

		*((size_t *)buf) = size;
		buf = (T *)(((size_t *)buf) + 1);

		for (size_t i = 0; i < size; ++i)
		{
			_construct<T>(buf + i);
		}

		return buf;
	};
	
	template <typename T>
	void _delete(T *ptr)
	{
		if (!ptr)
			return;
		
		_destroy<T>(ptr);
		_deallocate<T>(ptr);
	};
	
	template <typename T>
	void _delete(T *ptr, size_t size)
	{
		if (!ptr)
			return;

		for (size_t i = 0; i < size; ++i)
		{
			_destroy<T>(&ptr[i]);
		}
		
		_deallocate<T>(ptr);
	};
	
	template <typename T>
	void _delete_vec(T *ptr)
	{
		if (!ptr)
			return;

		size_t *vec_ptr = ((size_t *)ptr) - 1;
		size_t size = *vec_ptr;
		
		for (size_t i = 0; i < size; ++i)
		{
			_destroy<T>(&ptr[i]);
		}

		*vec_ptr = 0;
		_deallocate<T>((T *)vec_ptr);
	};
}

#define CDECL0(func) \
_asm	call [func]

#define CDECL1(func, arg1) \
_asm	push arg1 \
_asm	call [func] \
_asm	add esp, 4

#define CDECL2(func, arg1, arg2) \
_asm	push arg1 \
_asm	push arg2 \
_asm	call [func] \
_asm	add esp, 8

#define CDECL3(func, arg1, arg2, arg3) \
_asm	push arg1 \
_asm	push arg2 \
_asm	push arg3 \
_asm	call [func] \
_asm	add esp, 12

#define THISCALLRETN0(retval, func, obj) \
_asm	mov ecx, obj \
_asm	call [func] \
_asm	mov retval, eax

#define THISCALL0(func, obj) \
_asm	mov ecx, obj \
_asm	call [func]

#define THISCALL1(func, obj, arg1) \
_asm	push arg1 \
_asm	mov ecx, obj \
_asm	call [func]

#define THISCALL2(func, obj, arg1, arg2) \
_asm	push arg2 \
_asm	push arg1 \
_asm	mov ecx, obj \
_asm	call [func]

#define THISCALL3(func, obj, arg1, arg2, arg3) \
_asm	push arg3 \
_asm	push arg2 \
_asm	push arg1 \
_asm	mov ecx, obj \
_asm	call [func]

#define THISCALL4(func, obj, arg1, arg2, arg3, arg4) \
_asm	push arg4 \
_asm	push arg3 \
_asm	push arg2 \
_asm	push arg1 \
_asm	mov ecx, obj \
_asm	call [func]

#define THISCALL5(func, obj, arg1, arg2, arg3, arg4, arg5) \
_asm	push arg5 \
_asm	push arg4 \
_asm	push arg3 \
_asm	push arg2 \
_asm	push arg1 \
_asm	mov ecx, obj \
_asm	call [func]

#define THISCALL6(func, obj, arg1, arg2, arg3, arg4, arg5, arg6) \
_asm	push arg6 \
_asm	push arg5 \
_asm	push arg4 \
_asm	push arg3 \
_asm	push arg2 \
_asm	push arg1 \
_asm	mov ecx, obj \
_asm	call [func]

#define THISCALL7(func, obj, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
_asm	push arg7 \
_asm	push arg6 \
_asm	push arg5 \
_asm	push arg4 \
_asm	push arg3 \
_asm	push arg2 \
_asm	push arg1 \
_asm	mov ecx, obj \
_asm	call [func]

#define THISCALL8(func, obj, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
_asm	push arg8 \
_asm	push arg7 \
_asm	push arg6 \
_asm	push arg5 \
_asm	push arg4 \
_asm	push arg3 \
_asm	push arg2 \
_asm	push arg1 \
_asm	mov ecx, obj \
_asm	call [func]

#define THISCALL9(func, obj, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
_asm	push arg9 \
_asm	push arg8 \
_asm	push arg7 \
_asm	push arg6 \
_asm	push arg5 \
_asm	push arg4 \
_asm	push arg3 \
_asm	push arg2 \
_asm	push arg1 \
_asm	mov ecx, obj \
_asm	call [func]

#define FREEZE_REGS \
_asm	mov rgl::register_store::esp, esp \
_asm	mov esp, rgl::register_store::current_set \
_asm	push ebp \
_asm	push esi \
_asm	push edi \
_asm	push edx \
_asm	push ecx \
_asm	push ebx \
_asm	push eax \
_asm	mov rgl::register_store::current_set, esp \
_asm	mov esp, rgl::register_store::esp

#define RESTORE_REGS \
_asm	mov rgl::register_store::esp, esp \
_asm	mov esp, rgl::register_store::current_set \
_asm	pop eax \
_asm	pop ebx \
_asm	pop ecx \
_asm	pop edx \
_asm	pop edi \
_asm	pop esi \
_asm	pop ebp \
_asm	mov rgl::register_store::current_set, esp \
_asm	mov esp, rgl::register_store::esp

#define RESTORE_REGS_NO_POP \
_asm	mov rgl::register_store::esp, esp \
_asm	mov esp, rgl::register_store::current_set \
_asm	pop eax \
_asm	pop ebx \
_asm	pop ecx \
_asm	pop edx \
_asm	pop edi \
_asm	pop esi \
_asm	pop ebp \
_asm	mov esp, rgl::register_store::esp

#define RESTORE_REGS_NO_EAX \
_asm	mov rgl::register_store::esp, esp \
_asm	mov esp, rgl::register_store::current_set \
_asm	pop ebx \
_asm	pop ebx \
_asm	pop ecx \
_asm	pop edx \
_asm	pop edi \
_asm	pop esi \
_asm	pop ebp \
_asm	mov rgl::register_store::current_set, esp \
_asm	mov esp, rgl::register_store::esp

#define FREEZE_REGS_GFX \
	_asm	mov rgl::register_store_gfx::esp, esp \
_asm	mov esp, rgl::register_store_gfx::current_set \
_asm	push ebp \
_asm	push esi \
_asm	push edi \
_asm	push edx \
_asm	push ecx \
_asm	push ebx \
_asm	push eax \
_asm	mov rgl::register_store_gfx::current_set, esp \
_asm	mov esp, rgl::register_store_gfx::esp

#define RESTORE_REGS_GFX \
_asm	mov rgl::register_store_gfx::esp, esp \
_asm	mov esp, rgl::register_store_gfx::current_set \
_asm	pop eax \
_asm	pop ebx \
_asm	pop ecx \
_asm	pop edx \
_asm	pop edi \
_asm	pop esi \
_asm	pop ebp \
_asm	mov rgl::register_store_gfx::current_set, esp \
_asm	mov esp, rgl::register_store_gfx::esp

#define FREEZE_REGS_GFX \
	_asm	mov rgl::register_store_gfx::esp, esp \
_asm	mov esp, rgl::register_store_gfx::current_set \
_asm	push ebp \
_asm	push esi \
_asm	push edi \
_asm	push edx \
_asm	push ecx \
_asm	push ebx \
_asm	push eax \
_asm	mov rgl::register_store_gfx::current_set, esp \
_asm	mov esp, rgl::register_store_gfx::esp

#define RESTORE_REGS_GFX \
_asm	mov rgl::register_store_gfx::esp, esp \
_asm	mov esp, rgl::register_store_gfx::current_set \
_asm	pop eax \
_asm	pop ebx \
_asm	pop ecx \
_asm	pop edx \
_asm	pop edi \
_asm	pop esi \
_asm	pop ebp \
_asm	mov rgl::register_store_gfx::current_set, esp \
_asm	mov esp, rgl::register_store_gfx::esp

#define FREEZE_REGS_NET \
	_asm	mov rgl::register_store_net::esp, esp \
_asm	mov esp, rgl::register_store_net::current_set \
_asm	push ebp \
_asm	push esi \
_asm	push edi \
_asm	push edx \
_asm	push ecx \
_asm	push ebx \
_asm	push eax \
_asm	mov rgl::register_store_net::current_set, esp \
_asm	mov esp, rgl::register_store_net::esp

#define RESTORE_REGS_NET \
_asm	mov rgl::register_store_net::esp, esp \
_asm	mov esp, rgl::register_store_net::current_set \
_asm	pop eax \
_asm	pop ebx \
_asm	pop ecx \
_asm	pop edx \
_asm	pop edi \
_asm	pop esi \
_asm	pop ebp \
_asm	mov rgl::register_store_net::current_set, esp \
_asm	mov esp, rgl::register_store_net::esp
