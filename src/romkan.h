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
#ifndef ROMKAN_H
#define ROMKAN_H

#define Uses_SCIM_ICONV
#define Uses_SCIM_CONFIG_BASE
#include <vector>
#include <set>
#include <map>
#include <scim.h>
#include <scim_iconv.h>
#include <scim_event.h>
#include "preeditor.h"

using namespace std;
using namespace scim;

/**
@author TAM(Teppei Tamra)
*/
namespace ScimWnn {
struct RomkanTableEntry {
    String key;
    WideString kana;
};

class RomkanKeyEventList : public PreEditorKeyEventList
{
public:
    WideString p;
};


class Romkan : public PreEditor {
public:
    Romkan(ConfigPointer cfg);

    ~Romkan();
    virtual void clear();
    virtual void setPos(int p);
    virtual void reset();
    virtual WideString getText(bool hosei = false);
    virtual void backspace();
    virtual void del();
    virtual void hiraKata();
    virtual void kataHira();
    virtual void toWide();
    virtual void toHalf();
    virtual bool keyEventHook(const KeyEvent &key);
    virtual String getModeName();
    virtual bool cancelEvent();
    virtual bool inputEvent(const KeyEvent &key);
    virtual String getName();
    virtual String getPropertyName() {return(String("Romkan"));};
    virtual void setText(const WideString &t);
    virtual AttributeList getAttributeList();

protected:
    WideString eval();
    WideString insert(char k);
    void init();
    void loadTable(const String &filename, bool inc = false);
    bool string2bool(const String &s);

protected:
    String buf;
    String modeName;
    enum inputMode {ROMA,HROMA,KROMA,ASCII,WASCII};
    inputMode mode;
    bool nnMode;
    bool asciiCancel;
    bool removeRemainder;
    PreEditorKeyEventList key_ascii_mode,key_wascii_mode,key_toggle_hw,key_toggle_hk;
    map<String,WideString> RomkanTable;
    set<String> keepTable;
    map<String,String> tableConfig;
    String hookp;
    vector<RomkanKeyEventList> hookKey;
};
}
#endif
