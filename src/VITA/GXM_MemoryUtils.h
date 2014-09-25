#pragma once
#include <gxm.h>
#include "../Utilities.h"	

// Callback function to allocate memory for the shader patcher
static void *patcherHostAlloc(void *userData, uint32_t size);

// Callback function to allocate memory for the shader patcher
static void patcherHostFree(void *userData, void *mem);

// Helper function to allocate memory and map it for the GPU
static void *graphicsAlloc(SceKernelMemBlockType type, uint32_t size, uint32_t alignment, uint32_t attribs, SceUID *uid);

// Helper function to free memory mapped to the GPU
static void graphicsFree(SceUID uid);

// Helper function to allocate memory and map it as vertex USSE code for the GPU
static void *vertexUsseAlloc(uint32_t size, SceUID *uid, uint32_t *usseOffset);

// Helper function to free memory mapped as vertex USSE code for the GPU
static void vertexUsseFree(SceUID uid);

// Helper function to allocate memory and map it as fragment USSE code for the GPU
static void *fragmentUsseAlloc(uint32_t size, SceUID *uid, uint32_t *usseOffset);

// Helper function to free memory mapped as fragment USSE code for the GPU
static void fragmentUsseFree(SceUID uid);

static void *patcherHostAlloc(void *userData, uint32_t size)
{
	UNUSED(userData);
	return malloc(size);
}

static void patcherHostFree(void *userData, void *mem)
{
	UNUSED(userData);
	free(mem);
}

static void *graphicsAlloc(SceKernelMemBlockType type, uint32_t size, uint32_t alignment, uint32_t attribs, SceUID *uid)
{
	int err = SCE_OK;
	UNUSED(err);

	/*	Since we are using sceKernelAllocMemBlock directly, we cannot directly
	use the alignment parameter.  Instead, we must allocate the size to the
	minimum for this memblock type, and just assert that this will cover
	our desired alignment.

	Developers using their own heaps should be able to use the alignment
	parameter directly for more minimal padding.
	*/
	if (type == SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RWDATA) {
		// CDRAM memblocks must be 256KiB aligned
		SCE_ASSERT(alignment <= 256 * 1024);
		size = ALIGN(size, 256 * 1024);
	}
	else {
		// LPDDR memblocks must be 4KiB aligned
		SCE_ASSERT(alignment <= 4 * 1024);
		size = ALIGN(size, 4 * 1024);
	}
	UNUSED(alignment);

	// allocate some memory
	*uid = sceKernelAllocMemBlock("basic", type, size, NULL);
	SCE_ASSERT(*uid >= SCE_OK);

	// grab the base address
	void *mem = NULL;
	err = sceKernelGetMemBlockBase(*uid, &mem);
	SCE_ASSERT(err == SCE_OK);

	// map for the GPU
	err = sceGxmMapMemory(mem, size, attribs);
	SCE_ASSERT(err == SCE_OK);

	// done
	return mem;
}

static void graphicsFree(SceUID uid)
{
	int err = SCE_OK;
	UNUSED(err);

	// grab the base address
	void *mem = NULL;
	err = sceKernelGetMemBlockBase(uid, &mem);
	SCE_ASSERT(err == SCE_OK);

	// unmap memory
	err = sceGxmUnmapMemory(mem);
	SCE_ASSERT(err == SCE_OK);

	// free the memory block
	err = sceKernelFreeMemBlock(uid);
	SCE_ASSERT(err == SCE_OK);
}

static void *vertexUsseAlloc(uint32_t size, SceUID *uid, uint32_t *usseOffset)
{
	int err = SCE_OK;
	UNUSED(err);

	// align to memblock alignment for LPDDR
	size = ALIGN(size, 4096);

	// allocate some memory
	*uid = sceKernelAllocMemBlock("basic", SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, size, NULL);
	SCE_ASSERT(*uid >= SCE_OK);

	// grab the base address
	void *mem = NULL;
	err = sceKernelGetMemBlockBase(*uid, &mem);
	SCE_ASSERT(err == SCE_OK);

	// map as vertex USSE code for the GPU
	err = sceGxmMapVertexUsseMemory(mem, size, usseOffset);
	SCE_ASSERT(err == SCE_OK);

	// done
	return mem;
}

static void vertexUsseFree(SceUID uid)
{
	int err = SCE_OK;
	UNUSED(err);

	// grab the base address
	void *mem = NULL;
	err = sceKernelGetMemBlockBase(uid, &mem);
	SCE_ASSERT(err == SCE_OK);

	// unmap memory
	err = sceGxmUnmapVertexUsseMemory(mem);
	SCE_ASSERT(err == SCE_OK);

	// free the memory block
	err = sceKernelFreeMemBlock(uid);
	SCE_ASSERT(err == SCE_OK);
}

static void *fragmentUsseAlloc(uint32_t size, SceUID *uid, uint32_t *usseOffset)
{
	int err = SCE_OK;
	UNUSED(err);

	// align to memblock alignment for LPDDR
	size = ALIGN(size, 4096);

	// allocate some memory
	*uid = sceKernelAllocMemBlock("basic", SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, size, NULL);
	SCE_ASSERT(*uid >= SCE_OK);

	// grab the base address
	void *mem = NULL;
	err = sceKernelGetMemBlockBase(*uid, &mem);
	SCE_ASSERT(err == SCE_OK);

	// map as fragment USSE code for the GPU
	err = sceGxmMapFragmentUsseMemory(mem, size, usseOffset);
	SCE_ASSERT(err == SCE_OK);

	// done
	return mem;
}

static void fragmentUsseFree(SceUID uid)
{
	int err = SCE_OK;
	UNUSED(err);

	// grab the base address
	void *mem = NULL;
	err = sceKernelGetMemBlockBase(uid, &mem);
	SCE_ASSERT(err == SCE_OK);

	// unmap memory
	err = sceGxmUnmapFragmentUsseMemory(mem);
	SCE_ASSERT(err == SCE_OK);

	// free the memory block
	err = sceKernelFreeMemBlock(uid);
	SCE_ASSERT(err == SCE_OK);
}

