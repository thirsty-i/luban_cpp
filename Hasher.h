#pragma once

#include <iostream>
#include <tuple>
#include <functional>

namespace luban
{
    // Custom hash function
    template <typename... Args>
    struct Hasher {
        // FNV prime and offset basis constants for 32-bit and 64-bit systems
        static constexpr std::size_t _FNV_offset_basis = sizeof(std::size_t) == 4 ? 2166136261U : 14695981039346656037ULL;
        static constexpr std::size_t _FNV_prime = sizeof(std::size_t) == 4 ? 16777619 : 1099511628211;
        
        std::size_t operator()(const std::tuple<Args...>& t) const
        {
            return hashTuple(t, std::index_sequence_for<Args...>());
        }

    private:
#if __cplusplus > 201103L
        template <std::size_t... Is>
        std::size_t hashTuple(const std::tuple<Args...>& t, std::index_sequence<Is...>) const {
            std::size_t hashValue = _FNV_offset_basis;
            ((hashValue = (hashValue ^ std::hash<std::decay_t<std::tuple_element_t<Is, std::tuple<Args...>>>>{}(std::get<Is>(t))) * _FNV_prime), ...);
            return hashValue;
        }
#else
        template <std::size_t I, typename... Ts>
        inline typename std::enable_if<I == sizeof...(Ts), void>::type
        hashCombine(std::size_t& seed, const std::tuple<Ts...>&) const { }

        template <std::size_t I, typename... Ts>
        inline typename std::enable_if<I < sizeof...(Ts), void>::type
        hashCombine(std::size_t& seed, const std::tuple<Ts...>& t) const {
            seed ^= std::hash<typename std::tuple_element<I, std::tuple<Ts...>>::type>{}(std::get<I>(t)) * _FNV_prime;
            hashCombine<I + 1, Ts...>(seed, t);
        }

        template <std::size_t... Is>
        std::size_t hashTuple(const std::tuple<Args...>& t, std::index_sequence<Is...>) const {
            std::size_t hashValue = _FNV_offset_basis;
            hashCombine<0, Args...>(hashValue, t);
            return hashValue;
        }
#endif
    };
}

