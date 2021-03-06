// Copyright (c) 2016 - 2017 Settlers Freaks (sf-team at siedler25.org)
//
// This file is part of Return To The Roots.
//
// Return To The Roots is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Return To The Roots is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Return To The Roots. If not, see <http://www.gnu.org/licenses/>.

#include "commonDefines.h" // IWYU pragma: keep
#include "helpers/containerUtils.h"
#include "helpers/reverse.h"
#include <rttr/test/random.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/test/unit_test.hpp>
#include <vector>

BOOST_AUTO_TEST_SUITE(ContainerUtils)

BOOST_AUTO_TEST_CASE(MakeUnique)
{
    // Empty vector -> Not modified
    std::vector<int> vec;
    helpers::makeUnique(vec);
    BOOST_REQUIRE(vec.empty());
    // 1 el -> Not modified
    vec.push_back(1);
    helpers::makeUnique(vec);
    BOOST_REQUIRE_EQUAL(vec.at(0), 1);
    // 2 same els -> Only 1 remains
    vec.push_back(1);
    helpers::makeUnique(vec);
    BOOST_REQUIRE_EQUAL(vec.size(), 1u);
    BOOST_REQUIRE_EQUAL(vec[0], 1);
    // 2 different els -> Both remain
    vec.push_back(-1);
    helpers::makeUnique(vec);
    BOOST_REQUIRE_EQUAL(vec.size(), 2u);
    BOOST_REQUIRE_EQUAL(vec[0], 1);
    BOOST_REQUIRE_EQUAL(vec[1], -1);
    // More mixed elements
    vec.clear();
    using namespace boost::assign;
    vec += 5, 6, 5, 5, 2, 6, 1, 5, 7, 7, 3, -1, 3;
    std::vector<int> expectedVec;
    expectedVec += 5, 6, 2, 1, 7, 3, -1;
    helpers::makeUnique(vec);
    BOOST_REQUIRE_EQUAL_COLLECTIONS(vec.begin(), vec.end(), expectedVec.begin(), expectedVec.end());
}

BOOST_AUTO_TEST_CASE(IndexOf)
{
    std::vector<int> vec;
    // Empty vector
    BOOST_REQUIRE_EQUAL(helpers::indexOf(vec, 1), -1);
    // 1 el
    vec.push_back(1);
    BOOST_REQUIRE_EQUAL(helpers::indexOf(vec, 1), 0);
    BOOST_REQUIRE_EQUAL(helpers::indexOf(vec, 2), -1);
    // 2 els
    vec.push_back(0);
    BOOST_REQUIRE_EQUAL(helpers::indexOf(vec, 1), 0);
    BOOST_REQUIRE_EQUAL(helpers::indexOf(vec, 0), 1);
    BOOST_REQUIRE_EQUAL(helpers::indexOf(vec, 2), -1);

    // Pointer vector
    std::vector<int*> ptrVec;
    BOOST_REQUIRE_EQUAL(helpers::indexOf(ptrVec, (int*)1337), -1);       //-V566
    BOOST_REQUIRE_EQUAL(helpers::indexOf(ptrVec, (const int*)1337), -1); //-V566
    ptrVec.push_back((int*)1336);                                        //-V566
    ptrVec.push_back((int*)1337);                                        //-V566
    ptrVec.push_back((int*)1338);                                        //-V566
    BOOST_REQUIRE_EQUAL(helpers::indexOf(ptrVec, (int*)1337), 1);        //-V566
    BOOST_REQUIRE_EQUAL(helpers::indexOf(ptrVec, (const int*)1337), 1);  //-V566
}

BOOST_AUTO_TEST_CASE(Reverse)
{
    std::vector<int> vecIn, vecOut;
    for(int i : helpers::reverse(vecIn))
    {
        RTTR_UNUSED(i);                                             // LCOV_EXCL_LINE
        BOOST_TEST_FAIL("Reverse of empty vector should be empty"); // LCOV_EXCL_LINE
    }

    vecIn.resize(rttr::test::randomValue(1, 100));
    for(int& i : vecIn)
        i = rttr::test::randomValue<int>();

    for(int i : helpers::reverse(vecIn))
        vecOut.push_back(i);
    std::reverse(vecIn.begin(), vecIn.end());
    BOOST_TEST(vecIn == vecOut, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(Remove)
{
    std::vector<int> vecIn = {1, 2, 3, 4, 5}, vecExp;
    helpers::remove(vecIn, 42);
    BOOST_TEST(vecIn == vecIn, boost::test_tools::per_element());
    helpers::remove(vecIn, 2);
    vecExp = {1, 3, 4, 5};
    BOOST_TEST(vecIn == vecExp, boost::test_tools::per_element());
    helpers::remove(vecIn, 1);
    vecExp = {3, 4, 5};
    BOOST_TEST(vecIn == vecExp, boost::test_tools::per_element());
    helpers::remove(vecIn, 5);
    vecExp = {3, 4};
    BOOST_TEST(vecIn == vecExp, boost::test_tools::per_element());
    helpers::remove(vecIn, 4);
    vecExp = {3};
    BOOST_TEST(vecIn == vecExp, boost::test_tools::per_element());
    helpers::remove(vecIn, 3);
    vecExp = {};
    BOOST_TEST(vecIn == vecExp, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(RemoveIf)
{
    std::vector<int> vecIn = {1, 2, 3, 4, 5, 6}, vecExp = {1, 3, 5};
    helpers::remove_if(vecIn, [](int i) { return i % 2 == 0; });
    BOOST_TEST(vecIn == vecExp, boost::test_tools::per_element());

    vecIn = {1, 2, 3, 4, 5, 6}, vecExp = {2, 4, 6};
    helpers::remove_if(vecIn, [](int i) { return i % 2 != 0; });
    BOOST_TEST(vecIn == vecExp, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_SUITE_END()
