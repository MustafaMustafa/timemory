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
 * \file timemory/components/timing/types.hpp
 * \brief Declare the component types
 */

#pragma once

#include "timemory/components/macros.hpp"
#include "timemory/enum.h"
#include "timemory/mpl/type_traits.hpp"
#include "timemory/mpl/types.hpp"

//======================================================================================//
//
TIMEMORY_DECLARE_COMPONENT(wall_clock)
TIMEMORY_DECLARE_COMPONENT(system_clock)
TIMEMORY_DECLARE_COMPONENT(user_clock)
TIMEMORY_DECLARE_COMPONENT(cpu_clock)
TIMEMORY_DECLARE_COMPONENT(monotonic_clock)
TIMEMORY_DECLARE_COMPONENT(monotonic_raw_clock)
TIMEMORY_DECLARE_COMPONENT(thread_cpu_clock)
TIMEMORY_DECLARE_COMPONENT(process_cpu_clock)
TIMEMORY_DECLARE_COMPONENT(cpu_util)
TIMEMORY_DECLARE_COMPONENT(process_cpu_util)
TIMEMORY_DECLARE_COMPONENT(thread_cpu_util)
//
//======================================================================================//
//
//                              TYPE-TRAITS
//
//======================================================================================//
//
//                              STATISTICS
//
//--------------------------------------------------------------------------------------//
//
TIMEMORY_STATISTICS_TYPE(component::wall_clock, double)
TIMEMORY_STATISTICS_TYPE(component::system_clock, double)
TIMEMORY_STATISTICS_TYPE(component::user_clock, double)
TIMEMORY_STATISTICS_TYPE(component::cpu_clock, double)
TIMEMORY_STATISTICS_TYPE(component::monotonic_clock, double)
TIMEMORY_STATISTICS_TYPE(component::monotonic_raw_clock, double)
TIMEMORY_STATISTICS_TYPE(component::thread_cpu_clock, double)
TIMEMORY_STATISTICS_TYPE(component::process_cpu_clock, double)
TIMEMORY_STATISTICS_TYPE(component::cpu_util, double)
TIMEMORY_STATISTICS_TYPE(component::process_cpu_util, double)
TIMEMORY_STATISTICS_TYPE(component::thread_cpu_util, double)
//
//--------------------------------------------------------------------------------------//
//
//                              THREAD SCOPE ONLY
//
//--------------------------------------------------------------------------------------//
//
TIMEMORY_DEFINE_CONCRETE_TRAIT(thread_scope_only, component::thread_cpu_clock, true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(thread_scope_only, component::thread_cpu_util, true_type)
//
//--------------------------------------------------------------------------------------//
//
//                              IS TIMING CATEGORY
//
//--------------------------------------------------------------------------------------//
//
TIMEMORY_DEFINE_CONCRETE_TRAIT(is_timing_category, component::wall_clock, true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(is_timing_category, component::system_clock, true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(is_timing_category, component::user_clock, true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(is_timing_category, component::cpu_clock, true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(is_timing_category, component::monotonic_clock, true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(is_timing_category, component::monotonic_raw_clock,
                               true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(is_timing_category, component::thread_cpu_clock, true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(is_timing_category, component::process_cpu_clock,
                               true_type)
//
//--------------------------------------------------------------------------------------//
//
//                              USES TIMING UNITS
//
//--------------------------------------------------------------------------------------//
//
TIMEMORY_DEFINE_CONCRETE_TRAIT(uses_timing_units, component::wall_clock, true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(uses_timing_units, component::system_clock, true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(uses_timing_units, component::user_clock, true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(uses_timing_units, component::cpu_clock, true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(uses_timing_units, component::monotonic_clock, true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(uses_timing_units, component::monotonic_raw_clock,
                               true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(uses_timing_units, component::thread_cpu_clock, true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(uses_timing_units, component::process_cpu_clock, true_type)
//
//--------------------------------------------------------------------------------------//
//
//                              USES PERCENT UNITS
//
//--------------------------------------------------------------------------------------//
//
TIMEMORY_DEFINE_CONCRETE_TRAIT(uses_percent_units, component::cpu_util, true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(uses_percent_units, component::process_cpu_util, true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(uses_percent_units, component::thread_cpu_util, true_type)
//
//--------------------------------------------------------------------------------------//
//
//                              SUPPORTS FLAMEGRAPH
//
//--------------------------------------------------------------------------------------//
//
TIMEMORY_DEFINE_CONCRETE_TRAIT(supports_flamegraph, component::wall_clock, true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(supports_flamegraph, component::system_clock, true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(supports_flamegraph, component::user_clock, true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(supports_flamegraph, component::cpu_clock, true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(supports_flamegraph, component::monotonic_clock, true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(supports_flamegraph, component::monotonic_raw_clock,
                               true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(supports_flamegraph, component::thread_cpu_clock,
                               true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(supports_flamegraph, component::process_cpu_clock,
                               true_type)
//
//--------------------------------------------------------------------------------------//
//
//                                  DERIVATION
//
//--------------------------------------------------------------------------------------//
//
namespace tim
{
namespace trait
{
//
//--------------------------------------------------------------------------------------//
//
template <>
struct derivation_types<component::cpu_util>
{
    using type = std::tuple<
        type_list<component::wall_clock, component::cpu_clock>,
        type_list<component::wall_clock, component::user_clock, component::system_clock>>;
};
//
//--------------------------------------------------------------------------------------//
//
template <>
struct derivation_types<component::process_cpu_util>
{
    using type =
        std::tuple<type_list<component::wall_clock, component::process_cpu_clock>>;
};
//
//--------------------------------------------------------------------------------------//
//
template <>
struct derivation_types<component::thread_cpu_util>
{
    using type =
        std::tuple<type_list<component::wall_clock, component::thread_cpu_clock>>;
};
//
//--------------------------------------------------------------------------------------//
//
}  // namespace trait
}  // namespace tim
//
//======================================================================================//
//
//                              PROPERTIES
//
//======================================================================================//
//
TIMEMORY_PROPERTY_SPECIALIZATION(wall_clock, WALL_CLOCK, "wall_clock", "real_clock",
                                 "virtual_clock")

TIMEMORY_PROPERTY_SPECIALIZATION(system_clock, SYS_CLOCK, "system_clock", "sys_clock")

TIMEMORY_PROPERTY_SPECIALIZATION(user_clock, USER_CLOCK, "user_clock", "")

TIMEMORY_PROPERTY_SPECIALIZATION(cpu_clock, CPU_CLOCK, "cpu_clock", "")

TIMEMORY_PROPERTY_SPECIALIZATION(monotonic_clock, MONOTONIC_CLOCK, "monotonic_clock", "")

TIMEMORY_PROPERTY_SPECIALIZATION(monotonic_raw_clock, MONOTONIC_RAW_CLOCK,
                                 "monotonic_raw_clock", "")

TIMEMORY_PROPERTY_SPECIALIZATION(thread_cpu_clock, THREAD_CPU_CLOCK, "thread_cpu_clock",
                                 "")
TIMEMORY_PROPERTY_SPECIALIZATION(process_cpu_clock, PROCESS_CPU_CLOCK,
                                 "process_cpu_clock", "")

TIMEMORY_PROPERTY_SPECIALIZATION(cpu_util, CPU_UTIL, "cpu_util", "")

TIMEMORY_PROPERTY_SPECIALIZATION(process_cpu_util, PROCESS_CPU_UTIL, "process_cpu_util",
                                 "")

TIMEMORY_PROPERTY_SPECIALIZATION(thread_cpu_util, THREAD_CPU_UTIL, "thread_cpu_util", "")
