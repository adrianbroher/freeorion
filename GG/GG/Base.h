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

/** \file Base.h \brief Contains enums, utility classes, and free functions
    used throughout GG. */

#ifndef _GG_Base_h_
#define _GG_Base_h_

#include <GG/Export.h>

// GL headers
#include <GL/glew.h>

// other headers
#include <GG/Enum.h>
#include <GG/Clr.h>
#include <GG/PtRect.h>

#include <memory>
#include <functional>

/** \namespace GG \brief The namespace that encloses all GG classes,
    functions, typedefs, enums, etc. */
namespace GG {

extern GG_API const bool INSTRUMENT_ALL_SIGNALS;

template <typename FlagType>
class Flags;
class ModKey;

template <typename T>
class ScopedAssign
{
public:
    ScopedAssign(T& t, T val) :
        m_old_val(t),
        m_t(t)
        { m_t = val; }
    ~ScopedAssign()
        { m_t = m_old_val; }

private:
    T m_old_val;
    T& m_t;
};

/** Apply \p process to each weak_ptr in \p container and then remove any that are expired.*/

// vector
template <typename Container>
void ProcessThenRemoveExpiredPtrs(
    Container& container,
    const std::function<void(std::shared_ptr<typename Container::value_type::element_type>&)>& process,
    typename std::add_pointer<decltype(std::declval<Container>().at(std::declval<typename Container::size_type>()))>::type = nullptr)
{
    // Process
    for (const auto& weak : container)
        if (auto wnd = weak.lock())
            process(wnd);

    // Remove if the process caused the pointer to expire.
    Container not_expired;
    for (auto& weak : container) {
        if (!weak.expired()) {
            // Swap them to avoid another reference count check
            not_expired.push_back(std::shared_ptr<typename Container::value_type::element_type>());
            not_expired.back().swap(weak);
        }
    }
    container.swap(not_expired);
}

// set, unordered_set
template <typename Container>
void ProcessThenRemoveExpiredPtrs(
    Container& container,
    const std::function<void(std::shared_ptr<typename Container::value_type::element_type>&)>& process,
    decltype(std::declval<Container>().erase(std::declval<typename Container::iterator>()))* = nullptr,
    decltype(std::declval<Container>().equal_range(std::declval<
                                                   typename std::add_const<
                                                   typename std::add_lvalue_reference<
                                                   typename Container::key_type>::type>::type>()))* = nullptr)
{
    auto it = container.begin();
    while (it != container.end()) {
        // Process
        if (auto wnd = it->lock())
            process(wnd);

        // Remove if the process caused the pointer to expire.
        if (!it->expired())
            ++it;
        else
            it = container.erase(it);
    }
}

// list types
template <typename Container>
void ProcessThenRemoveExpiredPtrs(
    Container& container,
    const std::function<void(std::shared_ptr<typename Container::value_type::element_type>&)>& process,
    decltype(std::declval<Container>().erase(std::declval<typename Container::iterator>()))* = nullptr,
    decltype(std::declval<Container>().splice(std::declval<typename Container::const_iterator>(),
                                              std::declval<typename std::add_lvalue_reference<Container>::type>()))* = nullptr)
{
    auto it = container.begin();
    while (it != container.end()) {
        // Process
        if (auto wnd = it->lock())
            process(wnd);

        // Remove if the process caused the pointer to expire.
        if (!it->expired())
            ++it;
        else
            it = container.erase(it);
    }
}

/** "Regions" of a window; used e.g. to determine direction(s) of drag when a
    window that has a drag-frame is clicked. */
GG_ENUM(
    (WndRegion),
    ((WR_NONE, "WR_NONE", -1))
    ((WR_TOPLEFT, "WR_TOPLEFT", 0))
    ((WR_TOP))
    ((WR_TOPRIGHT))
    ((WR_MIDLEFT))
    ((WR_MIDDLE))
    ((WR_MIDRIGHT))
    ((WR_BOTTOMLEFT))
    ((WR_BOTTOM))
    ((WR_BOTTOMRIGHT))
)

/** The orientations for scrollbars, sliders, etc. */
GG_ENUM(
    (Orientation),
    ((VERTICAL))   ///< Vertical orientation.
    ((HORIZONTAL)) ///< Horizontal orientation.
)

/// Represents true keys on a keyboard (scancodes).  The values of this enum
/// are based on the Universal Serial BUS HID Usage Table 0x07
/// (Keyboard/Keypad page).
///
/// https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf
GG_ENUM(
    (Key),
    ((GGK_NONE,  "GGK_NONE",  0x00))
    ((GGK_a,     "GGK_a",     0x04))
    ((GGK_b))
    ((GGK_c))
    ((GGK_d))
    ((GGK_e))
    ((GGK_f))
    ((GGK_g))
    ((GGK_h))
    ((GGK_i))
    ((GGK_j))
    ((GGK_k))
    ((GGK_l))
    ((GGK_m))
    ((GGK_n))
    ((GGK_o))
    ((GGK_p))
    ((GGK_q))
    ((GGK_r))
    ((GGK_s))
    ((GGK_t))
    ((GGK_u))
    ((GGK_v))
    ((GGK_w))
    ((GGK_x))
    ((GGK_y))
    ((GGK_z))
    ((GGK_1))
    ((GGK_2))
    ((GGK_3))
    ((GGK_4))
    ((GGK_5))
    ((GGK_6))
    ((GGK_7))
    ((GGK_8))
    ((GGK_9))
    ((GGK_0))
    ((GGK_RETURN))
    ((GGK_ESCAPE))
    ((GGK_BACKSPACE))
    ((GGK_TAB))
    ((GGK_SPACE))
    ((GGK_MINUS))
    ((GGK_EQUALS))
    ((GGK_LBRACKET))
    ((GGK_RBRACKET))
    ((GGK_BACKSLASH))
    ((GGK_NONUSHASH))
    ((GGK_SEMICOLON))
    ((GGK_APOSTROPHE))
    ((GGK_GRAVE))
    ((GGK_COMMA))
    ((GGK_PERIOD))
    ((GGK_SLASH))
    ((GGK_CAPSLOCK))
    ((GGK_F1))
    ((GGK_F2))
    ((GGK_F3))
    ((GGK_F4))
    ((GGK_F5))
    ((GGK_F6))
    ((GGK_F7))
    ((GGK_F8))
    ((GGK_F9))
    ((GGK_F10))
    ((GGK_F11))
    ((GGK_F12))
    ((GGK_PRINTSCREEN))
    ((GGK_SCROLLLOCK))
    ((GGK_PAUSE))
    ((GGK_INSERT))
    ((GGK_HOME))
    ((GGK_PAGEUP))
    ((GGK_DELETE))
    ((GGK_END))
    ((GGK_PAGEDOWN))
    ((GGK_RIGHT))
    ((GGK_LEFT))
    ((GGK_DOWN))
    ((GGK_UP))
    ((GGK_KP_NUMLOCK))
    ((GGK_KP_DIVIDE))
    ((GGK_KP_MULTIPLY))
    ((GGK_KP_MINUS))
    ((GGK_KP_PLUS))
    ((GGK_KP_ENTER))
    ((GGK_KP1))
    ((GGK_KP2))
    ((GGK_KP3))
    ((GGK_KP4))
    ((GGK_KP5))
    ((GGK_KP6))
    ((GGK_KP7))
    ((GGK_KP8))
    ((GGK_KP9))
    ((GGK_KP0))
    ((GGK_KP_PERIOD))
    ((GGK_NONUSBACKSLASH))
    ((GGK_APPLICATION))
    ((GGK_POWER)) /// only a status flag according to USB HID
    ((GGK_KP_EQUALS))
    ((GGK_F13))
    ((GGK_F14))
    ((GGK_F15))
    ((GGK_F16))
    ((GGK_F17))
    ((GGK_F18))
    ((GGK_F19))
    ((GGK_F20))
    ((GGK_F21))
    ((GGK_F22))
    ((GGK_F23))
    ((GGK_F24))
    ((GGK_EXECUTE))
    ((GGK_HELP))
    ((GGK_MENU))
    ((GGK_SELECT))
    ((GGK_STOP))
    ((GGK_AGAIN))
    ((GGK_UNDO))
    ((GGK_CUT))
    ((GGK_COPY))
    ((GGK_PASTE))
    ((GGK_FIND))
    ((GGK_MUTE))
    ((GGK_VOLUMEUP))
    ((GGK_VOLUMEDOWN))
    ((GGK_LOCKINGCAPS))
    ((GGK_LOCKINGNUM))
    ((GGK_LOCKINGSCROLL))
    ((GGK_KP_COMMA))
    ((GGK_KP_EQUALSAS400))
    ((GGK_INTERNATIONAL1))
    ((GGK_INTERNATIONAL2))
    ((GGK_INTERNATIONAL3))
    ((GGK_INTERNATIONAL4))
    ((GGK_INTERNATIONAL5))
    ((GGK_INTERNATIONAL6))
    ((GGK_INTERNATIONAL7))
    ((GGK_INTERNATIONAL8))
    ((GGK_INTERNATIONAL9))
    ((GGK_LANGUAGE1))
    ((GGK_LANGUAGE2))
    ((GGK_LANGUAGE3))
    ((GGK_LANGUAGE4))
    ((GGK_LANGUAGE5))
    ((GGK_LANGUAGE6))
    ((GGK_LANGUAGE7))
    ((GGK_LANGUAGE8))
    ((GGK_LANGUAGE9))
    ((GGK_ERASE_ALT))
    ((GGK_SYSTEMREQUEST))
    ((GGK_CANCEL))
    ((GGK_CLEAR))
    ((GGK_PRIOR))
    ((GGK_RETURN_ALT))
    ((GGK_SEPARATOR))
    ((GGK_OUT))
    ((GGK_OPER))
    ((GGK_CLEARAGAIN))
    ((GGK_CRSEL))
    ((GGK_EXSEL))
    // Reserved block     0xa5 - 0xaf
    ((GGK_KP00, "GGK_KP00", 0xb0))
    ((GGK_KP000))
    ((GGK_THOUSANDSSEP))
    ((GGK_DECIMALSEP))
    ((GGK_CURRENCYUNIT))
    ((GGK_CURRENCYSUBUNIT))
    ((GGK_KP_LPARENTHESIS))
    ((GGK_KP_RPARENTHESIS))
    ((GGK_KP_LBRACE))
    ((GGK_KP_RBRACE))
    ((GGK_KP_TAB))
    ((GGK_KP_BACKSPACE))
    ((GGK_KP_A))
    ((GGK_KP_B))
    ((GGK_KP_C))
    ((GGK_KP_D))
    ((GGK_KP_E))
    ((GGK_KP_F))
    ((GGK_KP_XOR))
    ((GGK_KP_POWER))
    ((GGK_KP_PERCENT))
    ((GGK_KP_LESS))
    ((GGK_KP_GREATER))
    ((GGK_KP_AMPERSAND))
    ((GGK_KP_DBLAMPERSAND))
    ((GGK_KP_BAR))
    ((GGK_KP_DBLBAR))
    ((GGK_KP_COLON))
    ((GGK_KP_HASH))
    ((GGK_KP_SPACE))
    ((GGK_KP_AT))
    ((GGK_KP_EXCLAMATION))
    ((GGK_KP_MEMSTORE))
    ((GGK_KP_MEMRECALL))
    ((GGK_KP_MEMCLEAR))
    ((GGK_KP_MEMADD))
    ((GGK_KP_MEMSUBTRACT))
    ((GGK_KP_MEMMULTIPLY))
    ((GGK_KP_MEMDIVIDE))
    ((GGK_KP_PLUSMINUS))
    ((GGK_KP_CLEAR))
    ((GGK_KP_CLEARENTRY))
    ((GGK_KP_BINARY))
    ((GGK_KP_OCTAL))
    ((GGK_KP_DECIMAL))
    ((GGK_KP_HEXADECIMAL))
    // Reserved block     0xde - 0xdf
    ((GGK_LCONTROL, "GGK_LCONTROL", 0xe0))
    ((GGK_LSHIFT))
    ((GGK_LALT))
    ((GGK_LGUI))
    ((GGK_RCONTROL))
    ((GGK_RSHIFT))
    ((GGK_RALT))
    ((GGK_RGUI))
    // Reserved block     0xe8 - 0xffff
    ((GGK_LAST))
)

} // namespace GG

#endif
