// -*- C++ -*-
/* GG is a GUI for OpenGL.
   Copyright (C) 2003-2008 T. Zachary Laine

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1
   of the License, or (at your option) any later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
    
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA

   If you do not wish to comply with the terms of the LGPL please
   contact the author as other terms are available for a fee.
    
   Zach Laine
   whatwasthataddress@gmail.com */
   
/** \file Enum.h \brief Contains the utility classes and macros that allow for
    easy conversion to and from an enum value and its textual
    representation. */

#ifndef _GG_Enum_h_
#define _GG_Enum_h_

#include <iostream>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/transform.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/push_back.hpp>
#include <boost/preprocessor/tuple/size.hpp>


/** @brief Implementation detail for GG_ENUM */
#define GG_DEF_ENUM_VALUE(r, data, elem) \
    BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_TUPLE_SIZE(elem), 3), \
        BOOST_PP_TUPLE_ELEM(0, elem) = BOOST_PP_TUPLE_ELEM(2, elem), \
        BOOST_PP_TUPLE_ELEM(0, elem)),

/** @brief Implementation detail for GG_ENUM */
#define GG_DEF_ENUM(typeName, values) \
enum \
BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_TUPLE_SIZE(typeName), 2), \
    BOOST_PP_TUPLE_ELEM(1, typeName), \
    BOOST_PP_TUPLE_ELEM(0, typeName)) \
: int { \
    BOOST_PP_SEQ_FOR_EACH(GG_DEF_ENUM_VALUE, _, values) \
};

/** @brief Implementation detail for GG_ENUM */
#define GG_DEF_ENUM_OSTREAM_CASE(r, data, elem) \
    case BOOST_PP_TUPLE_ELEM(0, elem): \
        stream << BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_TUPLE_SIZE(elem), 1), \
                      BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(0, elem)), \
                      BOOST_PP_TUPLE_ELEM(1, elem)); \
        break;

/** @brief Implementation detail for GG_ENUM */
#define GG_DEF_ENUM_OSTREAM(typeName, values) \
inline \
BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_TUPLE_SIZE(typeName), 2), \
        friend, \
        BOOST_PP_EMPTY()) \
std::ostream& operator <<(std::ostream& stream, \
BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_TUPLE_SIZE(typeName), 2), \
    BOOST_PP_TUPLE_ELEM(1, typeName), \
    BOOST_PP_TUPLE_ELEM(0, typeName))& value) \
{ \
    switch(value) \
    { \
        BOOST_PP_SEQ_FOR_EACH(GG_DEF_ENUM_OSTREAM_CASE, _, values) \
        default: \
            stream.setstate(std::ios::failbit); \
            break; \
    } \
 \
    return stream; \
}

/** @brief Implementation detail for GG_ENUM */
#define GG_DEF_ENUM_ISTREAM_CASE(r, data, elem) \
    else if( \
BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_TUPLE_SIZE(elem), 1), \
    BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(0, elem)), \
    BOOST_PP_TUPLE_ELEM(1, elem)) == token) \
        value = BOOST_PP_TUPLE_ELEM(0, elem);

/** @brief Implementation detail for GG_ENUM */
#define GG_DEF_ENUM_ISTREAM(typeName, values) \
inline \
BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_TUPLE_SIZE(typeName), 2), \
        friend, \
        BOOST_PP_EMPTY()) \
std::istream& operator >>(std::istream& stream, \
BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_TUPLE_SIZE(typeName), 2), \
    BOOST_PP_TUPLE_ELEM(1, typeName), \
    BOOST_PP_TUPLE_ELEM(0, typeName))& value) \
{ \
    std::string token; \
 \
    stream >> token; \
 \
    if(false) \
        ; \
    BOOST_PP_SEQ_FOR_EACH(GG_DEF_ENUM_ISTREAM_CASE, _, values) \
    else \
        stream.setstate(std::ios::failbit); \
 \
    return stream; \
}

/** @brief Implementation detail for GG_ENUM */
#define GG_DEF_ENUM_ADD_STRING_REPR(s, data, elem) \
    BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_TUPLE_SIZE(elem), 1), \
        BOOST_PP_TUPLE_PUSH_BACK(elem, BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(0, elem))), \
        elem)

/** @brief Define an enumeration
 *
 * Defines an enumeration named @p typeName with the enumeration @p values in
 * the namespace where this macro is used. Also defines << and >> operators for
 * iostream usage.
 *
 * Use a tuple containing only the enumeration name when the enum is located
 * outside of a class as @p typeName.  Prepend the class name to the @p typeName
 * tuple when using it inside a class.
 *
 * Use a sequence of 1 to 3 element tuples as values.  The first tuple value
 * should contain the enum value symbol, the optional second one the string
 * representation.  IF no second value is set the stringified version of the
 * enum value symbol is used. The optional third tuple element should contain
 * the assigned enum element value.  If no third tuple value is given the
 * default enum element value assignment is used.
 *
 * When using the << operator the string representation of the enumeration is
 * written to the ostream.
 *
 * When using the >> operator and one string representation matches the input
 * value the output will be set to the associated enum value.  If the input
 * value doesn't match any string representation the istream failbit will be
 * set.
 *
 * Examples:
 *
 * Free enumeration:
 * @code
 * GG_ENUM(
 *     (Animal),
 *     ((CAT))
 *     ((DOG, "Dog"))
 *     ((COW, "Cow", 5))
 * )
 * @endcode
 *
 * Class member enumeration:
 * @code
 * class AutomaticGearBox
 * {
 * public:
 *     GG_ENUM(
 *         (AutomaticGearBox, Gear),
 *         ((PARK,    "P", -5))
 *         ((REVERSE, "R", -1))
 *         ((NEUTRAL, "N",  0))
 *         ((LOW,     "L"))
 *         ((DRIVE,   "D"))
 *         ((SPRINT,  "S"))
 *     )
 * }
 * @endcode
 */
#define GG_ENUM(typeName, values) \
    GG_DEF_ENUM(typeName, BOOST_PP_SEQ_TRANSFORM(GG_DEF_ENUM_ADD_STRING_REPR, _, values)) \
    GG_DEF_ENUM_OSTREAM(typeName, BOOST_PP_SEQ_TRANSFORM(GG_DEF_ENUM_ADD_STRING_REPR, _, values)) \
    GG_DEF_ENUM_ISTREAM(typeName, BOOST_PP_SEQ_TRANSFORM(GG_DEF_ENUM_ADD_STRING_REPR, _, values))

#endif
