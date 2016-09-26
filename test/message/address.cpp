/**
 * Copyright (c) 2011-2013 libbitcoin developers (see AUTHORS)
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
#include <boost/test/unit_test.hpp>
#include <boost/iostreams/stream.hpp>
#include <bitcoin/bitcoin.hpp>

using namespace bc;

bool equal(const message::address& left, const message::address& right)
{
    const auto left_addresses = left.addresses();
    const auto right_addresses = right.addresses();

    bool same = (left_addresses.size() == right_addresses.size());

    for (size_t i = 0; (i < left_addresses.size()) && same; i++)
    {
        same = (left_addresses[i] == right_addresses[i])
            && (left_addresses[i].timestamp() == right_addresses[i].timestamp());
    }

    return same;
}

BOOST_AUTO_TEST_SUITE(address_tests)

BOOST_AUTO_TEST_CASE(address__constructor_1__always__invalid)
{
    message::address instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
}

BOOST_AUTO_TEST_CASE(address__constructor_2__always__equals_params)
{
    const message::network_address::list addresses = {
        message::network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        message::network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        message::network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)
    };

    message::address instance(addresses);

    BOOST_REQUIRE_EQUAL(true, instance.is_valid());
    BOOST_REQUIRE(addresses == instance.addresses());
}

BOOST_AUTO_TEST_CASE(address__constructor_3__always__equals_params)
{
    const message::network_address::list addresses = {
        message::network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        message::network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        message::network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)
    };

    auto dup_addresses = addresses;

    message::address instance(std::move(dup_addresses));

    BOOST_REQUIRE_EQUAL(true, instance.is_valid());
    BOOST_REQUIRE(addresses == instance.addresses());
}

BOOST_AUTO_TEST_CASE(address__constructor_4__always__equals_params)
{
    const message::network_address::list addresses = {
        message::network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        message::network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        message::network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)
    };

    message::address value(addresses);
    message::address instance(value);

    BOOST_REQUIRE_EQUAL(true, instance.is_valid());
    BOOST_REQUIRE(value == instance);
    BOOST_REQUIRE(addresses == instance.addresses());
}

BOOST_AUTO_TEST_CASE(address__constructor_5__always__equals_params)
{
    const message::network_address::list addresses = {
        message::network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        message::network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        message::network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)
    };

    message::address value(addresses);
    message::address instance(std::move(value));

    BOOST_REQUIRE_EQUAL(true, instance.is_valid());
    BOOST_REQUIRE(addresses == instance.addresses());
}

BOOST_AUTO_TEST_CASE(address__from_data__insufficient_bytes__failure)
{
    data_chunk raw{ 0xab };
    message::address instance;

    BOOST_REQUIRE_EQUAL(false, instance.from_data(message::version::level::minimum, raw));
}

BOOST_AUTO_TEST_CASE(address__factory_from_data_1__roundtrip__success)
{
    const message::address expected({
        {
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u
        }
    });

    const auto data = expected.to_data(message::version::level::minimum);
    const auto result = message::address::factory_from_data(
        message::version::level::minimum, data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(equal(expected, result));
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::minimum), result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(address__factory_from_data_2__roundtrip__success)
{
    const message::address expected({
        {
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u
        }
    });

    const auto data = expected.to_data(message::version::level::minimum);
    data_source istream(data);
    const auto result = message::address::factory_from_data(
        message::version::level::minimum, istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(equal(expected, result));
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::minimum), result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(address__factory_from_data_3__roundtrip__success)
{
    const message::address expected({
        {
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u
        }
    });

    const data_chunk data = expected.to_data(message::version::level::minimum);
    data_source istream(data);
    istream_reader source(istream);
    const auto result = message::address::factory_from_data(
        message::version::level::minimum, source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(equal(expected, result));
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::minimum), result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(address__addresses_setter_1__roundtrip__success)
{
    const message::network_address::list value = {
        message::network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        message::network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        message::network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)
    };

    message::address instance;
    BOOST_REQUIRE(instance.addresses() != value);
    instance.set_addresses(value);
    BOOST_REQUIRE(value == instance.addresses());
}

BOOST_AUTO_TEST_CASE(address__addresses_setter_2__roundtrip__success)
{
    const message::network_address::list value = {
        message::network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        message::network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        message::network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)
    };

    auto dup_value = value;
    message::address instance;
    BOOST_REQUIRE(instance.addresses() != value);
    instance.set_addresses(std::move(dup_value));
    BOOST_REQUIRE(value == instance.addresses());
}

BOOST_AUTO_TEST_CASE(address__operator_assign_equals__always__matches_equivalent)
{
    const message::network_address::list addresses = {
        message::network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        message::network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        message::network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)
    };

    message::address value(addresses);

    BOOST_REQUIRE(value.is_valid());

    message::address instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(addresses == instance.addresses());
}

BOOST_AUTO_TEST_CASE(address__operator_boolean_equals__duplicates__returns_true)
{
    const message::address expected({
        message::network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        message::network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        message::network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)
    });

    message::address instance(expected);
    BOOST_REQUIRE_EQUAL(true, instance == expected);
}

BOOST_AUTO_TEST_CASE(address__operator_boolean_equals__differs__returns_false)
{
    const message::address expected({
        message::network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        message::network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        message::network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)
    });

    message::address instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(address__operator_boolean_not_equals__duplicates__returns_false)
{
    const message::address expected({
        message::network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        message::network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        message::network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)
    });

    message::address instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(address__operator_boolean_not_equals__differs__returns_true)
{
    const message::address expected({
        message::network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        message::network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        message::network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)
    });

    message::address instance;
    BOOST_REQUIRE_EQUAL(true, instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()
