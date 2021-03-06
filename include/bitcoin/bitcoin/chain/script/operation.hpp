/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_CHAIN_OPERATION_HPP
#define LIBBITCOIN_CHAIN_OPERATION_HPP

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>
#include <bitcoin/bitcoin/chain/script/opcode.hpp>
#include <bitcoin/bitcoin/chain/script/script_pattern.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/utility/data.hpp>
#include <bitcoin/bitcoin/utility/reader.hpp>
#include <bitcoin/bitcoin/utility/writer.hpp>

namespace libbitcoin {
namespace chain {

class BC_API operation
{
public:
    typedef std::vector<operation> sequence;
    typedef std::vector<operation>::const_iterator const_iterator;

    // Constructors.
    //-------------------------------------------------------------------------

    operation();

    operation(operation&& other);
    operation(const operation& other);

    operation(opcode code);

    operation(data_chunk&& uncoded, bool minimal=true);
    operation(const data_chunk& uncoded, bool minimal=true);

    // Operators.
    //-------------------------------------------------------------------------

    operation& operator=(operation&& other);
    operation& operator=(const operation& other);

    bool operator==(const operation& other) const;
    bool operator!=(const operation& other) const;

    // Deserialization.
    //-------------------------------------------------------------------------

    static operation factory_from_data(const data_chunk& encoded);
    static operation factory_from_data(std::istream& stream);
    static operation factory_from_data(reader& source);

    bool from_data(const data_chunk& encoded);
    bool from_data(std::istream& stream);
    bool from_data(reader& source);

    bool from_string(const std::string& mnemonic);

    bool is_valid() const;

    // Serialization.
    //-------------------------------------------------------------------------

    void to_data(std::ostream& stream) const;
    void to_data(writer& sink) const;
    data_chunk to_data() const;

    std::string to_string(uint32_t active_forks) const;

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    uint64_t serialized_size() const;

    /// Get the op code [0..255], if is_valid is consistent with data.
    opcode code() const;

    /// Get the data, empty if not a push code or if invalid.
    const data_chunk& data() const;

    // Utilities.
    //-------------------------------------------------------------------------

    /// Compute the minimal data opcode based on size alone.
    static opcode opcode_from_size(size_t size);

    /// Compute the minimal data opcode for a given chunk of data.
    /// If a numeric code is used then corresponding data must be set to empty.
    static opcode opcode_from_data(const data_chunk& data);

    /// Convert the [1..16] value to the corresponding opcode (or undefined).
    static opcode opcode_from_positive(uint8_t value);

    /// Convert the opcode to the corresponding [1..16] value (or undefined).
    static uint8_t opcode_to_positive(opcode code);

    /// Types of opcodes.
    static bool is_push(opcode code);
    static bool is_counted(opcode code);
    static bool is_numeric(opcode code);
    static bool is_positive(opcode code);
    static bool is_disabled(opcode code);
    static bool is_conditional(opcode code);

    // Validation.
    //-------------------------------------------------------------------------

    bool is_push() const;
    bool is_counted() const;
    bool is_positive() const;
    bool is_disabled() const;
    bool is_conditional() const;
    bool is_oversized() const;

protected:
    operation(opcode code, data_chunk&& data, bool valid);
    operation(opcode code, const data_chunk& data, bool valid);
    static uint32_t read_data_size(opcode code, reader& source);
    void reset();

private:
    opcode code_;
    data_chunk data_;
    bool valid_;
};

} // end chain
} // end libbitcoin

#endif
