// MIT License
//
// Copyright (c) 2020, The Regents of the University of California,
// through Lawrence Berkeley National Laboratory (subject to receipt of any
// required approvals from the U.S. Dept. of Energy).  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

/**
 * \file timemory/storage/macros.hpp
 * \brief Include the macros for storage
 */

#pragma once

#include "timemory/dll.hpp"

//======================================================================================//
//
// Define macros for storage
//
//======================================================================================//
//
#if defined(TIMEMORY_STORAGE_SOURCE)
#    define TIMEMORY_STORAGE_DLL tim_dll_export
#    define TIMEMORY_STORAGE_LINKAGE
#elif defined(TIMEMORY_USE_EXTERN) || defined(TIMEMORY_USE_STORAGE_EXTERN)
#    define TIMEMORY_STORAGE_DLL tim_dll_import
#    define TIMEMORY_STORAGE_LINKAGE
#    define TIMEMORY_STORAGE_HIDE_DEFINITION
#else
#    define TIMEMORY_STORAGE_DLL
#    define TIMEMORY_STORAGE_LINKAGE inline
#endif
//
//--------------------------------------------------------------------------------------//
