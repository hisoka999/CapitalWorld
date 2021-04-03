/*
 * dispatcher.h
 *
 *  Created on: 09.08.2016
 *      Author: stefan
 */

#ifndef CORE_DISPATCHER_H_
#define CORE_DISPATCHER_H_

#include <utility>
#include <functional>
#include <typeinfo>
#include <typeindex>
#include <vector>
#include <any>
#include <iostream>
#include <exception>

namespace core
{
    template <typename... Args>
    struct dispatcher
    {
        template <typename F>
        dispatcher(F f) : _f(std::move(f))
        {
        }

        void operator()(std::vector<std::any> const &v)
        {
            if (v.size() < sizeof...(Args))
            {
                std::cout << "Bad arity!" << std::endl; // Throw if you prefer
                return;
            }

            do_call(v, std::make_integer_sequence<int, sizeof...(Args)>());
        }

    private:
        template <int... Is>
        void do_call(std::vector<std::any> const &v, std::integer_sequence<int, Is...>)
        {
            try
            {
                return _f((get_ith<Args>(v, Is))...);
            }
            catch (std::bad_any_cast const &)
            {
                std::cerr << "Bad argument!" << std::endl; // Throw if you prefer
                throw std::runtime_error("bad argument");
            }
        }

        template <typename T>
        T get_ith(std::vector<std::any> const &v, int i)
        {
            return std::any_cast<T>(v[i]);
        }

    private:
        std::function<void(Args...)> _f;
    };

    typedef std::function<void(std::vector<std::any> const &)> dispatcher_type;

    template <typename T>
    struct function_traits;

    template <typename R, typename C, typename... Args>
    struct function_traits<R (C::*)(Args...)>
    {
        using args_type = std::tuple<Args...>;
    };

    template <typename R, typename C, typename... Args>
    struct function_traits<R (C::*)(Args...) const>
    {
        using args_type = std::tuple<Args...>;
    };

    template <typename T>
    struct dispatcher_maker;

    template <typename... Args>
    struct dispatcher_maker<std::tuple<Args...>>
    {
        template <typename F>
        dispatcher_type make(F &&f)
        {
            return dispatcher<Args...>{std::forward<F>(f)};
        }
    };

    template <typename F>
    std::function<void(std::vector<std::any> const &)> make_dispatcher(F &&f)
    {
        using f_type = decltype(&F::operator());

        using args_type = typename function_traits<f_type>::args_type;

        return dispatcher_maker<args_type>{}.make(std::forward<F>(f));
    }

    template <typename... Args>
    std::function<void(std::vector<std::any> const &)> make_dispatcher(
        void (*f)(Args...))
    {
        return dispatcher_maker<std::tuple<Args...>>{}.make(f);
    }
} // namespace core

#endif /* CORE_DISPATCHER_H_ */
