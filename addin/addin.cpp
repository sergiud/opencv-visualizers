// Copyright (c) 2012 Sergiu Dotenco
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <windows.h>
#include <tchar.h>

#include <cstring>
#include <cstddef>
#include <cstdio>

struct DEBUGHELPER
{
	DWORD dwVersion;
	BOOL (WINAPI *ReadDebuggeeMemory)(DEBUGHELPER *pThis, DWORD dwAddr,
		DWORD nWant, VOID* pWhere, DWORD *nGot);
	// from here only when dwVersion >= 0x20000
	DWORDLONG (WINAPI *GetRealAddress)(DEBUGHELPER *pThis);
	BOOL (WINAPI *ReadDebuggeeMemoryEx)(DEBUGHELPER *pThis,
		DWORDLONG qwAddr, DWORD nWant, VOID* pWhere, DWORD *nGot);
	int (WINAPI *GetProcessorType)(DEBUGHELPER *pThis);
};

namespace {

struct Mat
{
    int flags;
    //! the matrix dimensionality, >= 2
    int dims;
    //! the number of rows and columns or (-1, -1) when the matrix has more than 2 dimensions
    int rows, cols;
    //! pointer to the data
    BYTE* data;

    //! pointer to the reference counter;
    // when matrix points to user-allocated data, the pointer is NULL
    int* refcount;

    //! helper fields used in locateROI and adjustROI
	BYTE* datastart;
	BYTE* dataend;
	BYTE* datalimit;

    //! custom allocator
    void* allocator;

    struct MSize
    {
        int* p;
    };

    struct MStep
    {
        size_t* p;
        size_t buf[2];
    };

    MSize size;
    MStep step;

	size_t total() const
	{
		if (dims <= 2)
			return rows * cols;

		size_t p = 1;

		for (int i = 0; i < dims; ++i)
			p *= size.p[i];

		return p;
	}
};

template<typename T> HRESULT getObject(DEBUGHELPER *pHelper, DWORD address,
	T& object)
{
	DWORDLONG qwRealAddress;
	DWORD dwGot;

	HRESULT result;

	if (pHelper->dwVersion < 0x20000) {
		// Visual C++ 6.0
		qwRealAddress = address;
		result = pHelper->ReadDebuggeeMemory(pHelper, address, sizeof(T),
			&object, &dwGot);
	}
	else {
		qwRealAddress = pHelper->GetRealAddress(pHelper);
		result = pHelper->ReadDebuggeeMemoryEx(pHelper, qwRealAddress,
			sizeof(T), &object, &dwGot);
	}

	if (SUCCEEDED(result) && sizeof(T) != dwGot)
		result = E_FAIL;

	return result;
}

} // namespace

HRESULT WINAPI cvMat_preview(DWORD dwAddress, DEBUGHELPER *pHelper,
	int nBase, BOOL bUniStrings, char *pResult, size_t maxCount, DWORD)
{
	Mat mat;
	HRESULT result = getObject(pHelper, dwAddress, mat);

	if (SUCCEEDED(result)) {
		sprintf_s(pResult, maxCount, "%u elements", mat.total());
	}

	return result;
}

BOOL WINAPI DllMain(HINSTANCE, DWORD reason, LPVOID)
{
	return TRUE;
}
