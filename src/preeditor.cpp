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
#include "preeditor.h"

void PreEditorKeyEventList::append(const KeyEvent &k) {
    push_back(k);
}

void PreEditorKeyEventList::append(int code,int mask) {
    KeyEvent k;
    k.code = code;
    k.mask = mask;
    append(k);
}

bool PreEditorKeyEventList::comp(const KeyEvent &k) {
    int kc = k.get_ascii_code();
    if ((kc > 0x60) && (kc < 0x7b)) kc -= 0x20;
    else if ((kc > 0x40) && (kc < 0x5b)) kc += 0x20;
    for (unsigned int i = 0;i < size();i ++) {
        if ((at(i).code == k.code) && (at(i).mask == k.mask)) {
            return(true);
        }
        if (kc && (at(i).code == kc) && (at(i).mask == k.mask)) {
            return(true);
        }
    }
    return(false);
}

PreEditor::PreEditor(ConfigPointer cfg)
{
    iconvert.set_encoding ("EUC-JP");
    config = cfg;
    if (!convChars.size())
        iconvert.convert(convChars,String("ぁんァン!~！｝んっ"));
    if (!hKanaChars.size()) {
        // 強引だけどこれ以外方法思いつかないよー
        const String hKana[] = {
            "ｧ","ｱ","ｨ","ｲ","ｩ","ｳ","ｪ","ｴ","ｫ","ｵ","ｶ","ｶﾞ","ｷ","ｷﾞ","ｸ",
            "ｸﾞ","ｹ","ｹﾞ","ｺ","ｺﾞ","ｻ","ｻﾞ","ｼ","ｼﾞ","ｽ","ｽﾞ","ｾ","ｾﾞ","ｿ",
            "ｿﾞ","ﾀ","ﾀﾞ","ﾁ","ﾁﾞ","ｯ","ﾂ","ﾂﾞ","ﾃ","ﾃﾞ","ﾄ","ﾄﾞ","ﾅ","ﾆ","ﾇ",
            "ﾈ","ﾉ","ﾊ","ﾊﾞ","ﾊﾟ","ﾋ","ﾋﾞ","ﾋﾟ","ﾌ","ﾌﾞ","ﾌﾟ","ﾍ","ﾍﾞ","ﾍﾟ",
            "ﾎ","ﾎﾞ","ﾎﾟ","ﾏ","ﾐ","ﾑ","ﾒ","ﾓ","ｬ","ﾔ","ｭ","ﾕ","ｮ","ﾖ","ﾗ",
            "ﾘ","ﾙ","ﾚ","ﾛ","ﾜ","ﾜ","ｲ","ｴ","ｦ","ﾝ","","","","","","","",
            "ﾞ","ﾟ","","","","",
            "ｧ","ｱ","ｨ","ｲ","ｩ","ｳ","ｪ","ｴ","ｫ","ｵ","ｶ","ｶﾞ","ｷ","ｷﾞ","ｸ",
            "ｸﾞ","ｹ","ｹﾞ","ｺ","ｺﾞ","ｻ","ｻﾞ","ｼ","ｼﾞ","ｽ","ｽﾞ","ｾ","ｾﾞ","ｿ",
            "ｿﾞ","ﾀ","ﾀﾞ","ﾁ","ﾁﾞ","ｯ","ﾂ","ﾂﾞ","ﾃ","ﾃﾞ","ﾄ","ﾄﾞ","ﾅ","ﾆ","ﾇ",
            "ﾈ","ﾉ","ﾊ","ﾊﾞ","ﾊﾟ","ﾋ","ﾋﾞ","ﾋﾟ","ﾌ","ﾌﾞ","ﾌﾟ","ﾍ","ﾍﾞ","ﾍﾟ",
            "ﾎ","ﾎﾞ","ﾎﾟ","ﾏ","ﾐ","ﾑ","ﾒ","ﾓ","ｬ","ﾔ","ｭ","ﾕ","ｮ","ﾖ","ﾗ",
            "ﾘ","ﾙ","ﾚ","ﾛ","ﾜ","ﾜ","ｲ","ｴ","ｦ","ﾝ","ｳﾞ","ｶ","ｹ","","","","",
            "","-","ｵﾜﾘﾀﾞｺﾞﾙｧ"
            };
        for(unsigned int i = 0;hKana[i] != "ｵﾜﾘﾀﾞｺﾞﾙｧ";i ++) {
            if (hKana[i] != "") {
                WideString k;
                iconvert.convert(k,hKana[i]);
                hKanaChars.insert(pair<ucs4_t,WideString>(convChars[HIRA_START] + i,k));
            }
        }
    }
}


PreEditor::~PreEditor()
{
}

WideString PreEditor::text = WideString();
WideString PreEditor::convChars = WideString();
map<ucs4_t,WideString> PreEditor::hKanaChars;
int PreEditor::pos = 0;
IConvert PreEditor::iconvert;

/*!
    \fn PreEditor::getPos()
 */
int PreEditor::getPos()
{
    return(pos);
}


/*!
    \fn PreEditor::getTextLength()
 */
int PreEditor::getTextLength()
{
    return(text.length());
}


/*!
    \fn PreEditor::textIsEmpty()
 */
bool PreEditor::textIsEmpty()
{
    return(text.empty());
}


/*!
    \fn PreEditor::setPos(int p)
 */
void PreEditor::setPos(int p)
{
    if (p < 0) p = 0;
    else if (p > getTextLength()) p = getTextLength();
    pos = p;
}


/*!
    \fn PreEditor::clear()
 */
void PreEditor::clear()
{
    text.clear();
}



/*!
    \fn PreEditor::reset()
 */
void PreEditor::reset()
{
    clear();
    pos = 0;
}


/*!
    \fn PreEditor::getText(bool hosei)
 */
WideString PreEditor::getText(bool hosei)
{
    return(text);
}


/*!
    \fn PreEditor::backspace()
 */
void PreEditor::backspace()
{
    if (getPos() == 0) return;
    text = text.substr(0,pos - 1) + text.substr(pos);
    setPos(pos - 1);
}



/*!
    \fn PreEditor::del()
 */
void PreEditor::del()
{
    if (getPos() == getTextLength()) return;
    text = text.substr(0,pos) + text.substr(pos + 1);
}


/*!
    \fn PreEditor::convHiraKata(WideString &t)
 */
void PreEditor::convHiraKata(WideString &t)
{
    for(unsigned int i = 0;i < t.size();i ++) {
        if ((t[i] >= convChars[HIRA_START]) && (t[i] <= convChars[HIRA_END]))
            t[i] = t[i] - convChars[HIRA_START] + convChars[KATA_START];
    }
    return;
}


/*!
    \fn PreEditor::convKataHira(WideString &t)
 */
void PreEditor::convKataHira(WideString &t)
{
    for(unsigned int i = 0;i < t.size();i ++) {
        if ((t[i] >= convChars[KATA_START]) && (t[i] <= convChars[KATA_END]))
            t[i] = t[i] - convChars[KATA_START] + convChars[HIRA_START];
    }
    return;

}


/*!
    \fn PreEditor::convHanZen(WideString &t,int p)
 */
int PreEditor::convHanZen(WideString &t,int p)
{
    for(unsigned int i = 0;i < t.size();i ++) {
        if ((t[i] >= convChars[ASCII_START]) && (t[i] <= convChars[ASCII_END]))
            t[i] = t[i] - convChars[ASCII_START] + convChars[WASCII_START];
    }
    return(p);

}


/*!
    \fn PreEditor::convZenHan(WideString &t,int p)
 */
int PreEditor::convZenHan(WideString &t,int p)
{
    for(unsigned int i = 0;i < t.size();i ++) {
        if ((t[i] >= convChars[WASCII_START]) && (t[i] <= convChars[WASCII_END]))
            t[i] = t[i] - convChars[WASCII_START] + convChars[ASCII_START];

        // 超絶半角カナ変換
        map<ucs4_t,WideString>::iterator it = hKanaChars.find(t[i]);
        if (it != hKanaChars.end()) {
            WideString m1 = t.substr(0,i);
            WideString m2 = t.substr(i + 1);
            t = m1 + it->second + m2;
            if (p > i) p += it->second.size() - 1;
            i += it->second.length() - 1;
        }
    }
    return(p);

}


/*!
    \fn PreEditor::hiraKata()
 */
void PreEditor::hiraKata()
{
    convHiraKata(text);
}


/*!
    \fn PreEditor::kataHira()
 */
void PreEditor::kataHira()
{
    convKataHira(text);
}


/*!
    \fn PreEditor::toHalf()
 */
void PreEditor::toHalf()
{
    convZenHan(text,pos);
}

/*!
    \fn PreEditor::toWide()
 */
void PreEditor::toWide()
{
    convHanZen(text,pos);
}


/*!
    \fn PreEditor::keyEventHook(const KeyEvent &key)
 */
bool PreEditor::keyEventHook(const KeyEvent &key)
{
    return(false);
}



/*!
    \fn PreEditor::getModeName()
 */
String PreEditor::getModeName()
{
    String s;
    return(s);
}


/*!
    \fn PreEditor::cancelEvent()
 */
bool PreEditor::cancelEvent()
{
    return(false);
}



/*!
    \fn PreEditor::inputEvent(const KeyEvent &key)
 */
bool PreEditor::inputEvent(const KeyEvent &key)
{
    if (isprint(key.code)){
        String s;
        s = key.get_ascii_code();
        text = text.substr(0,pos) + utf8_mbstowcs(s) + text.substr(pos);
        pos ++;
        return(true);
    }
    return(false);
}


/*!
    \fn PreEditor::getName()
 */
String PreEditor::getName()
{
    return(String("PreEditor"));
}


/*!
    \fn PreEditor::setText(const WideString &t)
 */
void PreEditor::setText(const WideString &t)
{
    text = t;
}



/*!
    \fn PreEditor::getAttributeList()
 */
AttributeList PreEditor::getAttributeList()
{
    return(AttributeList());
}
