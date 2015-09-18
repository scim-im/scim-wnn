/***************************************************************************
 *   Copyright (C) 2004 by TAM(Teppei Tamra)                               *
 *   tam-t@par.odn.ne.jp                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef SCIM_WNN_DEF_H
#define SCIM_WNN_DEF_H

#define SCIM_CONFIG_WNN_KEY_CONVERSION_START        "/IMEngine/Wnn/Key/ConversionStart"
#define SCIM_DEFAULT_WNN_KEY_CONVERSION_START       "space"
#define SCIM_CONFIG_WNN_KEY_CANCEL                  "/IMEngine/Wnn/Key/Cancel"
#define SCIM_DEFAULT_WNN_KEY_CANCEL                 "Escape,Control+g"
#define SCIM_CONFIG_WNN_KEY_DELETE                  "/IMEngine/Wnn/Key/Delete"
#define SCIM_DEFAULT_WNN_KEY_DELETE                 "Delete"
#define SCIM_CONFIG_WNN_KEY_BACKSPACE               "/IMEngine/Wnn/Key/BackSpace"
#define SCIM_DEFAULT_WNN_KEY_BACKSPACE              "BackSpace"
#define SCIM_CONFIG_WNN_KEY_COMMIT                  "/IMEngine/Wnn/Key/Commit"
#define SCIM_DEFAULT_WNN_KEY_COMMIT                 "Return"
#define SCIM_CONFIG_WNN_KEY_CONVERSION_NEXT         "/IMEngine/Wnn/Key/ConversionNext"
#define SCIM_DEFAULT_WNN_KEY_CONVERSION_NEXT        "space,Down"
#define SCIM_CONFIG_WNN_KEY_CONVERSION_PREV         "/IMEngine/Wnn/Key/ConversionPrev"
#define SCIM_DEFAULT_WNN_KEY_CONVERSION_PREV        "Up"
#define SCIM_CONFIG_WNN_KEY_CONVERSION_EXPAND       "/IMEngine/Wnn/Key/ConversionExpand"
#define SCIM_DEFAULT_WNN_KEY_CONVERSION_EXPAND      "Shift+Right,Control+o"
#define SCIM_CONFIG_WNN_KEY_CONVERSION_SHRINK       "/IMEngine/Wnn/Key/ConversionShrink"
#define SCIM_DEFAULT_WNN_KEY_CONVERSION_SHRINK      "Shift+Left,Control+i"
#define SCIM_CONFIG_WNN_KEY_CONVERSION_FORWARD      "/IMEngine/Wnn/Key/ConversionForward"
#define SCIM_DEFAULT_WNN_KEY_CONVERSION_FORWARD     "Right"
#define SCIM_CONFIG_WNN_KEY_CONVERSION_BACKWARD     "/IMEngine/Wnn/Key/ConversionBackward"
#define SCIM_DEFAULT_WNN_KEY_CONVERSION_BACKWARD    "Left"
#define SCIM_CONFIG_WNN_KEY_FORWARD                 "/IMEngine/Wnn/Key/Forward"
#define SCIM_DEFAULT_WNN_KEY_FORWARD                "Right"
#define SCIM_CONFIG_WNN_KEY_BACKWARD                "/IMEngine/Wnn/Key/Backword"
#define SCIM_DEFAULT_WNN_KEY_BACKWARD               "Left"
#define SCIM_CONFIG_WNN_KEY_HOME                    "/IMEngine/Wnn/Key/Home"
#define SCIM_DEFAULT_WNN_KEY_HOME                   "Home"
#define SCIM_CONFIG_WNN_KEY_END                     "/IMEngine/Wnn/Key/End"
#define SCIM_DEFAULT_WNN_KEY_END                    "End"
#define SCIM_CONFIG_WNN_KEY_LOOKUPPOPUP             "/IMEngine/Wnn/Key/LookupPopup"
#define SCIM_DEFAULT_WNN_KEY_LOOKUPPOPUP            "Alt+s"
#define SCIM_CONFIG_WNN_KEY_LOOKUPPAGEUP            "/IMEngine/Wnn/Key/LookupPageUp"
#define SCIM_DEFAULT_WNN_KEY_LOOKUPPAGEUP           "Page_Up"
#define SCIM_CONFIG_WNN_KEY_LOOKUPPAGEDOWN          "/IMEngine/Wnn/Key/LookupPageDown"
#define SCIM_DEFAULT_WNN_KEY_LOOKUPPAGEDOWN         "Page_Down"
#define SCIM_CONFIG_WNN_KEY_CONVERT_HIRAGANA        "/IMEngine/Wnn/Key/ConvertHiragana"
#define SCIM_DEFAULT_WNN_KEY_CONVERT_HIRAGANA       "Alt+h"
#define SCIM_CONFIG_WNN_KEY_CONVERT_KATAKANA        "/IMEngine/Wnn/Key/ConvertKatakana"
#define SCIM_DEFAULT_WNN_KEY_CONVERT_KATAKANA       "Alt+k"
#define SCIM_CONFIG_WNN_KEY_CONVERT_HALF            "/IMEngine/Wnn/Key/ConvertHalf"
#define SCIM_DEFAULT_WNN_KEY_CONVERT_HALF           "F6"
#define SCIM_CONFIG_WNN_KEY_CONVERT_WIDE            "/IMEngine/Wnn/Key/ConvertWide"
#define SCIM_DEFAULT_WNN_KEY_CONVERT_WIDE           "F5"

#define SCIM_CONFIG_WNN_KEY_CONVERSION_IKEIJI       "/IMEngine/Wnn/Key/ConversionIkeiji"
#define SCIM_DEFAULT_WNN_KEY_CONVERSION_IKEIJI      "Shift+F8"
#define SCIM_CONFIG_WNN_KEY_CONVERSION_RENSOU       "/IMEngine/Wnn/Key/ConversionRensou"
#define SCIM_DEFAULT_WNN_KEY_CONVERSION_RENSOU      "Shift+F9"
#define SCIM_CONFIG_WNN_KEY_SELECT_YOSOKU           "/IMEngine/Wnn/Key/SelectYosoku"
#define SCIM_DEFAULT_WNN_KEY_SELECT_YOSOKU          "Tab"
#define SCIM_CONFIG_WNN_KEY_AUTO_CONVERSION         "/IMEngine/Wnn/Key/AutoConversion"
#define SCIM_DEFAULT_WNN_KEY_AUTO_CONVERSION        "Alt+a"

#define SCIM_CONFIG_WNN_ALP                         "/IMEngine/Wnn/AutoLookupPopup"
#define SCIM_DEFAULT_WNN_ALP                        3
#define SCIM_CONFIG_WNN_SERVER                      "/IMEngine/Wnn/Server"
#define SCIM_DEFAULT_WNN_SERVER                     "localhost"
#define SCIM_CONFIG_WNN_RC                          "/IMEngine/Wnn/Rc"
#define SCIM_DEFAULT_WNN_RC                         "/usr/lib/wnn7/ja_JP/wnnenvrc"
#define SCIM_CONFIG_WNN_AUTO_CONVERSION             "/IMEngine/Wnn/AutoConversion"
#define SCIM_DEFAULT_WNN_AUTO_CONVERSION            false
#define SCIM_CONFIG_WNN_NUMKEY_SELECT               "/IMEngine/Wnn/NumkeySelection"
#define SCIM_DEFAULT_WNN_NUMKEY_SELECT              true
#define SCIM_CONFIG_WNN_DEFAULT_PREEDITOR           "/IMEngine/Wnn/DefaultPreEditor"
#define SCIM_DEFAULT_WNN_DEFAULT_PREEDITOR          "Romkan"
#define SCIM_CONFIG_WNN_SERVERTYPE                  "/IMEngine/Wnn/ServerType"
#ifdef HAVE_LIBWNN7
  #define SCIM_DEFAULT_WNN_SERVERTYPE                 "Wnn7"
#else
  #define SCIM_DEFAULT_WNN_SERVERTYPE                 "Wnn4"
#endif

#define SCIM_CONFIG_WNN_MINISTATUS                  "/IMEngine/Wnn/MiniStatus"
#define SCIM_DEFAULT_WNN_MINISTATUS                 false

#define SCIM_CONFIG_WNN_YOSOKU                      "/IMEngine/Wnn/Yosoku"
#ifdef HAVE_LIBWNN7
  #define SCIM_DEFAULT_WNN_YOSOKU                     true
#else
  #define SCIM_DEFAULT_WNN_YOSOKU                     false
#endif

#define SCIM_PROP_WNN_MODESTATUS                    "/IMEngine/Wnn/Property/ModeStatus"
#define SCIM_PROP_WNN_INPUTMODE                     "/IMEngine/Wnn/InputMode"
#define SCIM_PROP_WNN_CONVERSIONMODE                "/IMEngine/Wnn/ConversionMode"


// Romkan

#define SCIM_CONFIG_WNN_ROMKAN_TABLE_FILE           "/IMEngine/Wnn/Romkan/TableFile"
#define SCIM_DEFAULT_WNN_ROMKAN_TABLE_FILE          "scim-wnn-def.rkt"

// KanaInput
#define SCIM_CONFIG_WNN_KANAINPUT_FAKEKANA          "/IMEngine/Wnn/KanaInput/FakeKanaKey"
#define SCIM_DEFAULT_WNN_KANAINPUT_FAKEKANA         true



#endif

