/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <cstdint>
#include <cstddef>
#include <array>

namespace Stm32Common {
    // Generic registry for enum entries with metadata
    template<typename EntryT, size_t N>
    class EnumRegistry {
    public:
        constexpr EnumRegistry(const EntryT (&entries)[N]) : _entries(entries) {
        }

        template<typename KeyT, auto Extractor>
        constexpr const std::optional<EntryT> find(KeyT key) const {
            for (size_t i = 0; i < N; ++i)
                if (Extractor(_entries[i]) == key)
                    return _entries[i];
            return std::nullopt;
        }

        // --- Metadata ---
        constexpr size_t count() const { return N; }
        constexpr const EntryT *entries() const { return _entries; }

        // --- Safety check: Duplicate values ---
        template<auto Extractor>
        constexpr bool has_duplicate_values() const {
            for (size_t i = 0; i < N; ++i) {
                for (size_t j = i + 1; j < N; ++j) {
                    if (Extractor(_entries[i]) == Extractor(_entries[j]))
                        return true;
                }
            }
            return false;
        }

        // --- Generate name list ---
        constexpr auto names() const {
            std::array<const char *, N> result{};
            for (size_t i = 0; i < N; ++i)
                result[i] = _entries[i].name;
            return result;
        }

        // --- Generate Enum-to-Code map ---
        template<typename EnumT, auto EnumGetter, auto CodeGetter>
        constexpr auto map_enum_to_code() const {
            std::array<decltype(CodeGetter(_entries[0])), N> map{};
            for (const auto &e: _entries) {
                map[static_cast<size_t>(EnumGetter(e))] = CodeGetter(e);
            }
            return map;
        }

    private:
        const EntryT (&_entries)[N];
    };


    template<typename EntryT>
    class RegistryView {
    public:
        constexpr RegistryView() : _entry(nullptr) {}
        constexpr RegistryView(const EntryT* entry) : _entry(entry) {}

        constexpr const EntryT* get() const { return _entry; }

        constexpr explicit operator bool() const { return _entry != nullptr; }
        constexpr bool valid() const { return _entry != nullptr; }

        // Generischer Feldzugriff (nur wenn Felder vorhanden sind)
        constexpr auto id() const { return _entry->id; }
        constexpr auto code() const { return _entry->apiId; }
        constexpr const char* name() const { return _entry->name; }
        constexpr const char* description() const { return _entry->description; }

    private:
        const EntryT* _entry;
    };



}
