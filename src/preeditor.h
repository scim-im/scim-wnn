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
#ifndef PREEDITOR_H
#define PREEDITOR_H

#define Uses_SCIM_ICONV
#define Uses_SCIM_CONFIG_BASE
#include <vector>
#include <map>
#include <scim.h>
#include <scim_iconv.h>
#include <scim_event.h>
#include <scim_attribute.h>

#define HIRA_START      0
#define HIRA_END        1
#define KATA_START      2
#define KATA_END        3
#define ASCII_START     4
#define ASCII_END       5
#define WASCII_START    6
#define WASCII_END      7
#define KANA_N          8
#define KANA_XTU        9

using namespace std;
using namespace scim;


// ひらがな/かたかな等、preeditに文字を入力する仕組みのベースクラス。
// ローマ字かな変換以外にも対応する為の準備。


/**
@author TAM(Teppei Tamra)
*/
namespace ScimWnn {
class PreEditorKeyEventList : public KeyEventList
{
    // リストにキーイベントを追加して、comp(KeyEvent)で判別。
public:
    void append(const KeyEvent &k);
    void append(int code,int mask);
    bool comp(const KeyEvent &k);
};

class PreEditor{
public:
    PreEditor(ConfigPointer cfg);

    ~PreEditor();
    virtual void clear();
    int getPos();
    virtual void setPos(int p);
    int getTextLength();
    bool textIsEmpty();
    virtual void reset();
    virtual WideString getText(bool hosei = false);
    virtual void backspace();
    virtual void del();
    virtual void hiraKata();
    virtual void kataHira();
    virtual bool keyEventHook(const KeyEvent &key);
    virtual String getModeName();
    static void convHiraKata(WideString &t);
    static void convKataHira(WideString &t);
    static int convHanZen(WideString &t,int p);
    static int convZenHan(WideString &t,int p);
    virtual bool cancelEvent();
    virtual bool inputEvent(const KeyEvent &key);
    virtual String getName();
    virtual String getPropertyName() {return(String("PreEditor"));};
    virtual void setText(const WideString &t);
    virtual void toWide();
    virtual void toHalf();
    virtual AttributeList getAttributeList();

protected:
    static WideString text;
    static int pos;
    static IConvert iconvert;
    ConfigPointer config;
    static WideString convChars;
    static map<ucs4_t,WideString> hKanaChars;

};
}

using namespace ScimWnn;
#endif
