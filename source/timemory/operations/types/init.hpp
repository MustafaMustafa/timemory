// MIT License
//
// Copyright (c) 2020, The Regents of the University of California,
// through Lawrence Berkeley National Laboratory (subject to receipt of any
// required approvals from the U.S. Dept. of Energy).  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a init
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, init, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above initright notice and this permission notice shall be included in all
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
 * \file timemory/operations/types/init.hpp
 * \brief Definition for global and thread-local initialzation functions for a component
 */

#pragma once

#include "timemory/operations/declaration.hpp"
#include "timemory/operations/macros.hpp"
#include "timemory/operations/types.hpp"

namespace tim
{
namespace operation
{
//
//--------------------------------------------------------------------------------------//
//
///
/// \class operation::init
/// \brief This operation class is used for invoking the static initializer and
/// thread-local initializer of a component
///
//
template <typename Tp>
struct init
{
    using type = Tp;

    TIMEMORY_DELETED_OBJECT(init)

    template <typename Up = Tp, int StateT, typename StorageT,
              enable_if_t<(trait::is_available<Up>::value), char> = 0>
    explicit init(StorageT _storage, mode_constant<StateT>)
    {
        sfinae<type, StateT>(_storage, 0, 0);
    }

    template <typename Up = Tp, int StateT, typename StorageT,
              enable_if_t<!(trait::is_available<Up>::value), char> = 0>
    explicit init(StorageT, mode_constant<StateT>)
    {}

private:
    template <typename Up, int StateT, typename StorageT,
              enable_if_t<(StateT == init_mode::global), char> = 0>
    auto sfinae(StorageT _storage, int, int)
        -> decltype(std::declval<Up>().global_init(_storage), void())
    {
        if(was_executed<StateT>())
            return;
        type::global_init(_storage);
        was_executed<StateT>() = true;
    }

    template <typename Up, int StateT, typename StorageT,
              enable_if_t<(StateT == init_mode::global), int> = 0>
    auto sfinae(StorageT, int, long) -> decltype(std::declval<Up>().global_init(), void())
    {
        if(was_executed<StateT>())
            return;
        type::global_init();
        was_executed<StateT>() = true;
    }

    template <typename Up, int StateT, typename StorageT,
              enable_if_t<(StateT == init_mode::thread), char> = 0>
    auto sfinae(StorageT _storage, int, int)
        -> decltype(std::declval<Up>().thread_init(_storage), void())
    {
        if(was_executed<StateT>())
            return;
        type::thread_init(_storage);
        was_executed<StateT>() = true;
    }

    template <typename Up, int StateT, typename StorageT,
              enable_if_t<(StateT == init_mode::thread), int> = 0>
    auto sfinae(StorageT, int, long) -> decltype(std::declval<Up>().thread_init(), void())
    {
        if(was_executed<StateT>())
            return;
        type::thread_init();
        was_executed<StateT>() = true;
    }

    template <typename Up, int StateT, typename StorageT>
    void sfinae(StorageT, long, long)
    {}

private:
    template <int StateT, enable_if_t<(StateT == init_mode::global), char> = 0>
    static bool& was_executed()
    {
        static bool _instance = false;
        return _instance;
    }

    template <int StateT, enable_if_t<(StateT == init_mode::thread), char> = 0>
    static bool& was_executed()
    {
        static thread_local bool _instance = false;
        return _instance;
    }
};
//
//--------------------------------------------------------------------------------------//
//
}  // namespace operation
}  // namespace tim
