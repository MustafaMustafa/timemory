// MIT License
//
// Copyright (c) 2019, The Regents of the University of California,
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
//

/** \file auto_list.hpp
 * \headerfile auto_list.hpp "timemory/variadic/auto_list.hpp"
 * Automatic starting and stopping of components. Accept unlimited number of
 * parameters. The constructor starts the components, the destructor stops the
 * components
 *
 * Usage with macros (recommended):
 *    \param TIMEMORY_AUTO_LIST()
 *    \param TIMEMORY_BASIC_AUTO_LIST()
 *    \param auto t = TIMEMORY_AUTO_LIST_OBJ()
 *    \param auto t = TIMEMORY_BASIC_AUTO_LIST_OBJ()
 */

#pragma once

#include <cstdint>
#include <string>

#include "timemory/mpl/filters.hpp"
#include "timemory/utility/macros.hpp"
#include "timemory/utility/utility.hpp"
#include "timemory/variadic/component_list.hpp"
#include "timemory/variadic/macros.hpp"

namespace tim
{
//--------------------------------------------------------------------------------------//

template <typename... Types>
class auto_list
: public tim::counted_object<auto_list<Types...>>
, public tim::hashed_object<auto_list<Types...>>
{
public:
    using component_type = tim::component_list<Types...>;
    using this_type      = auto_list<Types...>;
    using data_type      = typename component_type::data_type;
    using counter_type   = tim::counted_object<this_type>;
    using counter_void   = tim::counted_object<void>;
    using hashed_type    = tim::hashed_object<this_type>;
    using string_t       = std::string;
    using string_hash    = std::hash<string_t>;
    using base_type      = component_type;
    using language_t     = tim::language;
    using tuple_type     = implemented<Types...>;
    using init_func_t    = std::function<void(this_type&)>;

public:
    inline explicit auto_list(const string_t&, const int64_t& lineno = 0,
                              const language_t& lang           = language_t::cxx(),
                              bool              report_at_exit = false);
    inline explicit auto_list(component_type& tmp, const int64_t& lineno = 0,
                              bool report_at_exit = false);
    inline ~auto_list();

    // copy and move
    inline auto_list(const this_type&) = default;
    inline auto_list(this_type&&)      = default;
    inline this_type& operator=(const this_type&) = default;
    inline this_type& operator=(this_type&&) = default;

    static constexpr std::size_t size() { return component_type::size(); }

    static constexpr std::size_t available_size()
    {
        return component_type::available_size();
    }

public:
    // public member functions
    inline component_type&       component_list() { return m_temporary_object; }
    inline const component_type& component_list() const { return m_temporary_object; }

    // partial interface to underlying component_list
    inline void record() { m_temporary_object.record(); }
    inline void start() { m_temporary_object.start(); }
    inline void stop() { m_temporary_object.stop(); }
    inline void push() { m_temporary_object.push(); }
    inline void pop() { m_temporary_object.pop(); }
    inline void reset() { m_temporary_object.reset(); }
    inline void mark_begin() { m_temporary_object.mark_begin(); }
    inline void mark_end() { m_temporary_object.mark_end(); }

    inline void report_at_exit(bool val) { m_report_at_exit = val; }
    inline bool report_at_exit() const { return m_report_at_exit; }

public:
    template <std::size_t _N>
    typename std::tuple_element<_N, data_type>::type& get()
    {
        return m_temporary_object.template get<_N>();
    }

    template <std::size_t _N>
    const typename std::tuple_element<_N, data_type>::type& get() const
    {
        return m_temporary_object.template get<_N>();
    }

    template <typename _Tp>
    auto get() -> decltype(std::declval<component_type>().template get<_Tp>())
    {
        return m_temporary_object.template get<_Tp>();
    }

    template <typename _Tp>
    auto get() const -> decltype(std::declval<const component_type>().template get<_Tp>())
    {
        return m_temporary_object.template get<_Tp>();
    }

    template <typename _Tp, typename... _Args,
              tim::enable_if_t<(is_one_of<_Tp, tuple_type>::value == true), int> = 0>
    void init(_Args&&... _args)
    {
        m_temporary_object.template init<_Tp>(std::forward<_Args>(_args)...);
    }

    template <typename _Tp, typename... _Args,
              tim::enable_if_t<(is_one_of<_Tp, tuple_type>::value == false), int> = 0>
    void init(_Args&&...)
    {
    }

    template <typename _Tp, typename... _Tail,
              tim::enable_if_t<(sizeof...(_Tail) == 0), int> = 0>
    void initialize()
    {
        this->init<_Tp>();
    }

    template <typename _Tp, typename... _Tail,
              tim::enable_if_t<(sizeof...(_Tail) > 0), int> = 0>
    void initialize()
    {
        this->init<_Tp>();
        this->initialize<_Tail...>();
    }

    static init_func_t& get_initializer()
    {
        static init_func_t _instance = [](this_type& al) {
            tim::env::initialize(al, "TIMEMORY_AUTO_LIST_INIT", "");
        };
        return _instance;
    }

public:
    friend std::ostream& operator<<(std::ostream& os, const this_type& obj)
    {
        os << obj.m_temporary_object;
        return os;
    }

private:
    bool            m_enabled        = true;
    bool            m_report_at_exit = false;
    component_type  m_temporary_object;
    component_type* m_reference_object = nullptr;
};

//======================================================================================//

template <typename... Types>
auto_list<Types...>::auto_list(const string_t& object_tag, const int64_t& lineno,
                               const language_t& lang, bool report_at_exit)
: counter_type()
, hashed_type((counter_type::enable())
                  ? (string_hash()(object_tag) + static_cast<int64_t>(lang) +
                     (counter_type::live() + hashed_type::live() + lineno))
                  : 0)
, m_enabled(counter_type::enable() && settings::enabled())
, m_report_at_exit(report_at_exit)
, m_temporary_object(object_tag, lang, counter_type::m_count, hashed_type::m_hash,
                     m_enabled)
{
    if(m_enabled)
    {
        get_initializer()(*this);
        m_temporary_object.start();
    }
}

//======================================================================================//

template <typename... Types>
auto_list<Types...>::auto_list(component_type& tmp, const int64_t& lineno,
                               bool report_at_exit)
: counter_type()
, hashed_type((counter_type::enable())
                  ? (string_hash()(tmp.key()) + static_cast<int64_t>(tmp.lang()) +
                     (counter_type::live() + hashed_type::live() + lineno))
                  : 0)
, m_enabled(true)
, m_report_at_exit(report_at_exit)
, m_temporary_object(tmp.clone(hashed_type::m_hash, true))
, m_reference_object(&tmp)
{
    if(m_enabled)
    {
        get_initializer()(*this);
        m_temporary_object.start();
    }
}

//======================================================================================//

template <typename... Types>
auto_list<Types...>::~auto_list()
{
    if(m_enabled)
    {
        // stop the timer
        m_temporary_object.conditional_stop();

        // report timer at exit
        if(m_report_at_exit)
        {
            std::stringstream ss;
            ss << m_temporary_object;
            if(ss.str().length() > 0)
                std::cout << ss.str() << std::endl;
        }

        if(m_reference_object)
        {
            *m_reference_object += m_temporary_object;
        }
    }
}

//======================================================================================//

}  // namespace tim

//======================================================================================//

#define TIMEMORY_BLANK_AUTO_LIST(auto_list_type, ...)                                    \
    TIMEMORY_BLANK_OBJECT(auto_list_type, __VA_ARGS__)

#define TIMEMORY_BASIC_AUTO_LIST(auto_list_type, ...)                                    \
    TIMEMORY_BASIC_OBJECT(auto_list_type, __VA_ARGS__)

#define TIMEMORY_AUTO_LIST(auto_list_type, ...)                                          \
    TIMEMORY_OBJECT(auto_list_type, __VA_ARGS__)

//--------------------------------------------------------------------------------------//
// caliper versions

#define TIMEMORY_BLANK_AUTO_LIST_CALIPER(id, auto_list_type, ...)                        \
    TIMEMORY_BLANK_CALIPER(id, auto_list_type, __VA_ARGS__)

#define TIMEMORY_BASIC_AUTO_LIST_CALIPER(id, auto_list_type, ...)                        \
    TIMEMORY_BASIC_CALIPER(id, auto_list_type, __VA_ARGS__)

#define TIMEMORY_AUTO_LIST_CALIPER(id, auto_list_type, ...)                              \
    TIMEMORY_CALIPER(id, auto_list_type, __VA_ARGS__)

//--------------------------------------------------------------------------------------//
// instance versions

#define TIMEMORY_BLANK_AUTO_LIST_INSTANCE(auto_list_type, ...)                           \
    TIMEMORY_BLANK_INSTANCE(auto_list_type, __VA_ARGS__)

#define TIMEMORY_BASIC_AUTO_LIST_INSTANCE(auto_list_type, ...)                           \
    TIMEMORY_BASIC_INSTANCE(auto_list_type, __VA_ARGS__)

#define TIMEMORY_AUTO_LIST_INSTANCE(auto_list_type, ...)                                 \
    TIMEMORY_INSTANCE(auto_list_type, __VA_ARGS__)

//--------------------------------------------------------------------------------------//
// debug versions

#define TIMEMORY_DEBUG_BASIC_AUTO_LIST(auto_list_type, ...)                              \
    TIMEMORY_DEBUG_BASIC_OBJECT(auto_list_type, __VA_ARGS__)

#define TIMEMORY_DEBUG_AUTO_LIST(auto_list_type, ...)                                    \
    TIMEMORY_DEBUG_OBJECT(auto_list_type, __VA_ARGS__)

//--------------------------------------------------------------------------------------//
// variadic versions

#define TIMEMORY_VARIADIC_BASIC_AUTO_LIST(tag, ...)                                      \
    using _AUTO_TYPEDEF(__LINE__) = tim::auto_list<__VA_ARGS__>;                         \
    TIMEMORY_BASIC_AUTO_LIST(_AUTO_TYPEDEF(__LINE__), tag);

#define TIMEMORY_VARIADIC_AUTO_LIST(tag, ...)                                            \
    using _AUTO_TYPEDEF(__LINE__) = tim::auto_list<__VA_ARGS__>;                         \
    TIMEMORY_AUTO_LIST(_AUTO_TYPEDEF(__LINE__), tag);

//======================================================================================//