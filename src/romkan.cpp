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
#include "romkan.h"
#include "scim_wnn_def.h"

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

// 国際化のおまじない。
#ifdef HAVE_GETTEXT
  #include <libintl.h>
  #define _(String) dgettext(GETTEXT_PACKAGE,String)
  #define N_(String) (String)
#else
  #define _(String) (String)
  #define N_(String) (String)
  #define bindtextdomain(Package,Directory)
  #define textdomain(domain)
  #define bind_textdomain_codeset(domain,codeset)
#endif


Romkan::Romkan(ConfigPointer cfg) : PreEditor(cfg)
{
    reset();
    iconvert.set_encoding ("EUC-JP");
    mode = ROMA;
    init();
}


Romkan::~Romkan()
{
}


/*!
    \fn Romkan::loadTable(const String &filename, bool inc)
 */
void Romkan::loadTable(const String &filename, bool inc)
{
    FILE *f = fopen(filename.c_str(),"r");
    if (!f) {
        #ifdef SCIM_WNN_DATADIR
        String fn = SCIM_WNN_DATADIR;
        fn += "/" + filename;
        f = fopen(fn.c_str(),"r");
        if (!f) return;
        #else
        return;
        #endif
    }
    
    while(-1) {
        char s[256];
        if(fgets(s,256,f) == NULL) break;
        String k,r;
        unsigned int i = 0;
        bool ex = false;
        for(;s[i];i ++) {
            if (s[i] == ' ') {
                if (k.size()) break;
                else continue;
            }
            if (s[i] == '\n') break;
            if ((s[i] == '!') || (s[i] == '$') ||(s[i] == '%')) {
                if (!k.size()) ex = true;
            }
            if (s[i] == '#') break;
            if ((s[i] == '\\') && (s[i + 1] != 0)) {
                i ++;
            }
            k += s[i];
        }
        if ((!k.size()) || (s[i] == '#')) continue;
        if (s[i] != 0) {
            i ++;
            for(;s[i];i ++) {
                if ((s[i] == '#') || (s[i] == '\n')) break;
                if (s[i] == ' ') {
                    if (r.size()) break;
                    else continue;
                }
                if ((s[i] == '\\') && (s[i + 1] != 0)) {
                    i ++;
                }
                r += s[i];
            }
        }
        if (ex && (k[0] == '%')) {
            loadTable(k.substr(1),true);
            continue;
        }
        if ((k[0] == '$') && (!inc) && ex) {
            tableConfig.insert(pair<String,String>(k.substr(1),r));
            continue;
        }
        if ((k[0] == '!') && (!inc) && ex) {
            RomkanKeyEventList kev;
            scim_string_to_key_list(kev,k.substr(1));
            kev.p = utf8_mbstowcs(r);
            hookKey.push_back(kev);
            continue;
        }
        if (!r.size()) continue;
        RomkanTable.insert(pair<String,WideString>(k,utf8_mbstowcs(r)));
        
        for(unsigned int j = k.length();j > 1;j --) {
            String keep = k.substr(0,j - 1);
            if ((keepTable.find(keep) == keepTable.end()) && (RomkanTable.find(keep) == RomkanTable.end())) {
                keepTable.insert(keep);
            }
        }
    }
    fclose(f);
        
}


/*!
    \fn Romkan::string2bool(const String &s)
 */
bool Romkan::string2bool(const String &s)
{
    if ((s == "FALSE") || (s == "false") || (s == "False") || (s == "0") || (!s.size())) return(false);
    else return(true);
}


/*!
    \fn Romkan::init()
 */
void Romkan::init()
{
    RomkanTable.clear();
    keepTable.clear();
    tableConfig.clear();
    
    String rk_table = config->read(SCIM_CONFIG_WNN_ROMKAN_TABLE_FILE,String(SCIM_DEFAULT_WNN_ROMKAN_TABLE_FILE));
    
    if (rk_table.size()) {
        loadTable(rk_table);
    }
    
    if (tableConfig.find("nnMode") != tableConfig.end())
        nnMode = string2bool(tableConfig["nnMode"]);
    else nnMode = false;
    if (tableConfig.find("AsciiModeCancel") != tableConfig.end())
        asciiCancel = string2bool(tableConfig["AsciiModeCancel"]);
    else asciiCancel = true;
    if (tableConfig.find("RemoveRemainder") != tableConfig.end())
        removeRemainder = string2bool(tableConfig["RemoveRemainder"]);
    else removeRemainder = false;
    if (tableConfig.find("Key/Ascii") != tableConfig.end())
        scim_string_to_key_list(key_ascii_mode,tableConfig["Key/Ascii"]);
    if (tableConfig.find("Key/WideAscii") != tableConfig.end())
        scim_string_to_key_list(key_wascii_mode,tableConfig["Key/WideAscii"]);
    if (tableConfig.find("Key/ToggleHalfOrWide") != tableConfig.end())
        scim_string_to_key_list(key_toggle_hw,tableConfig["Key/ToggleHalfOrWide"]);
    if (tableConfig.find("Key/ToggleHiraOrKata") != tableConfig.end())
        scim_string_to_key_list(key_toggle_hk,tableConfig["Key/ToggleHiraOrKata"]);
    if (tableConfig.find("Hook") != tableConfig.end())
        hookp = tableConfig["Hook"];
    else hookp = "";

}




/*!
    \fn Romkan::setPos(int p)
 */
void Romkan::setPos(int p)
{
    if (p < 0) p = 0;
    else if (p > getTextLength()) p = getTextLength();
    pos = p;
    buf.clear();
}


/*!
    \fn Romkan::clear()
 */
void Romkan::clear()
{
    text.clear();
    buf.clear();
}


/*!
    \fn Romkan::insert(char k)
 */
WideString Romkan::insert(char k)
{
    String s;
    s = k;
    if (k == 0) return(text);
    switch(mode) {
        case ASCII: {
            buf.clear();
            text = text.substr(0,pos) + utf8_mbstowcs(s) + text.substr(pos);
            pos ++;
            return(text);
            break;
        }
        case WASCII: {
            buf.clear();
            WideString w = utf8_mbstowcs(s);
            convHanZen(w,pos);
            text = text.substr(0,pos) + w + text.substr(pos);
            pos ++;
            return(text);
            break;
        }
        case ROMA: {
            buf += s;
            text = text.substr(0,pos) + utf8_mbstowcs(s) + text.substr(pos);
            pos ++;
            return(eval());
            break;
        }
        case KROMA: {
            buf += s;
            text = text.substr(0,pos) + utf8_mbstowcs(s) + text.substr(pos);
            pos ++;
            return(eval());
            break;
        }
        case HROMA: {
            buf += s;
            text = text.substr(0,pos) + utf8_mbstowcs(s) + text.substr(pos);
            pos ++;
            return(eval());
            break;
        }
    }
    return(text);
}


/*!
    \fn Romkan::eval()
 */
WideString Romkan::eval()
{
    // エヴァる。

    if (buf.length() == 2) {

        // n＋母音以外は「ん」＋子音である
        // ただし、nyは除外である。
        if (buf[0] == 'n') {
            String b = "aiueoy";
            if (nnMode) b += "n";
            bool boin = false;
            for(unsigned int i = 0;i < b.length();i ++) {
                if (buf[1] == b[i]) boin = true;
            }
            if (!boin) {
                WideString r;
                r += convChars[KANA_N];
                if (mode == HROMA) convZenHan(r,0);
                else if (mode == KROMA) convHiraKata(r);
                text = text.substr(0,pos - 2) + r + text.substr(pos - 1);
                buf = buf.substr(buf.length() - 1,1);
            }
        }

        // 同じ文字が２文字続くとそれは「っ」＋子音である。母音の連打がbufに残ってはいないはず。
        else if (buf[0] == buf[1]) {
            WideString r;
            r += convChars[KANA_XTU];
            if (mode == HROMA) convZenHan(r,0);
            else if (mode == KROMA) convHiraKata(r);
            text = text.substr(0,pos - 2) + r + text.substr(pos - 1);
            buf = buf.substr(buf.length() - 1,1);
            return(text);
        }
    }

    // 保留テーブルを検索する。
    if (keepTable.find(buf) != keepTable.end()) return(text);
    // テーブルを検索して入力する。
    map<String,WideString>::iterator it = RomkanTable.find(buf);
    if (it != RomkanTable.end()) {
        WideString r;
        r += it->second;
        if (mode == HROMA) convZenHan(r,0);
        else if (mode == KROMA) convHiraKata(r);
        text = text.substr(0,pos - buf.length()) + r + text.substr(pos);
        pos = pos - buf.length() + r.length();
        buf.clear();
        return(text);
    }
    if (buf.length()) {
        if (removeRemainder) {
            text = text.substr(0,pos - buf.length()) + text.substr(pos - buf.length() + 1);
            pos --;
        }
        buf = buf.substr(1);
        return(eval());
    }

    return(text);
}




/*!
    \fn Romkan::reset()
 */
void Romkan::reset()
{
    clear();
    pos = 0;
    mode = ROMA;
}

/*!
    \fn PreEditor::setText(const WideString &t)
 */
void Romkan::setText(const WideString &t)
{
    text = t;
    buf.clear();
}


/*!
    \fn Romkan::getText(bool hosei)
 */
WideString Romkan::getText(bool hosei)
{
    if (hosei) {
        if (buf.length()) {
            if (removeRemainder) {
                text = text.substr(0,pos - buf.length()) + text.substr(pos);
                pos -= buf.length();
                if (buf.substr(buf.length() - 1,1) == "n") {
                    WideString r;
                    r += convChars[KANA_N];
                    if (mode == HROMA) convZenHan(r,0);
                    else if (mode ==KROMA) convHiraKata(r);
                    text = text.substr(0,pos) + r + text.substr(pos);
                    pos ++;
                }
            } else 
            if (buf.substr(buf.length() - 1,1) == "n") {
                WideString r;
                r += convChars[KANA_N];
                if (mode == HROMA) convZenHan(r,0);
                else if (mode ==KROMA) convHiraKata(r);
                text = text.substr(0,pos - 1) + r + text.substr(pos);
            }
        }
    }
    return(text);
}


/*!
    \fn Romkan::backspace()
 */
void Romkan::backspace()
{
    if (getPos() == 0) return;
    text = text.substr(0,pos - 1) + text.substr(pos);
    setPos(pos - 1);
    // BSは小バッファもBSするゾ。
    if (buf.length()) buf = buf.substr(0,buf.length() - 1);
}



/*!
    \fn Romkan::del()
 */
void Romkan::del()
{
    if (getPos() == getTextLength()) return;
    text = text.substr(0,pos) + text.substr(pos + 1);
}



/*!
    \fn Romkan::hiraKata()
 */
void Romkan::hiraKata()
{
    convHiraKata(text);
}


/*!
    \fn Romkan::kataHira()
 */
void Romkan::kataHira()
{
    convKataHira(text);
}

/*!
    \fn Romkan::toHalf()
 */
void Romkan::toHalf()
{
    setPos(convZenHan(text,getPos()));
    buf.clear();
}

/*!
    \fn Romkan::toWide()
 */
void Romkan::toWide()
{
    setPos(convHanZen(text,getPos()));
    buf.clear();
}



/*!
    \fn Romkan::keyEventHook(const KeyEvent &key)
 */
bool Romkan::keyEventHook(const KeyEvent &key)
{
    if (key_toggle_hw.comp(key)) {
        switch (mode) {
            case ASCII: {
                mode = WASCII;
                break;
            }
            case WASCII: {
                mode = ASCII;
                break;
            }
            case ROMA: {
                mode = HROMA;
                break;
            }
            case HROMA: {
                mode = ROMA;
                break;
            }
        }
        return(true);
    }
    if (key_toggle_hk.comp(key)) {
        if (mode == ROMA) mode = KROMA;
        else if (mode == KROMA) mode = ROMA;
        return(true);
    }

    if ((isprint(key.code)) && ((mode == ASCII) || (mode == WASCII))) {
        if ((!key.is_alt_down()) && (!key.is_control_down())) {
            insert(key.get_ascii_code());
            return(true);
        }
    }
    
    for(vector<RomkanKeyEventList>::iterator it = hookKey.begin();it != hookKey.end();it ++) {
        if (it->comp(key)) {
            text = text.substr(0,pos) + it->p + text.substr(pos);
            pos += it->p.length();
            return(true);
        }
    }

    if ((buf.length() < hookp.length()) || (!hookp.length())) return(false);
    else if (buf.substr(0,hookp.length()) == hookp) {
        return(inputEvent(key));
    }
    return(false);
}


/*!
    \fn Romkan::getModeName()
 */
String Romkan::getModeName()
{
    switch(mode) {
        case ROMA: {
            return(String(_("Roma-Kana")));
            break;
        }
        case KROMA: {
            return(String(_("Roma-Katakana")));
            break;
        }
        case HROMA: {
            return(String(_("Half Roma-Kana")));
            break;
        }
        case ASCII: {
            return(String(_("Ascii")));
            break;
        }
        case WASCII: {
            return(String(_("Wide Ascii")));
            break;
        }
    }
    return(String(""));
}


/*!
    \fn Romkan::cancelEvent()
 */
bool Romkan::cancelEvent()
{
    if (((mode == ASCII) || (mode == WASCII)) && (asciiCancel)) {
        mode = ROMA;
        return(true);
    }
    return(false);
}


/*!
    \fn Romkan::inputEvent(const KeyEvent &key)
 */
bool Romkan::inputEvent(const KeyEvent &key)
{
    // 喰っておくべきもの。
    if ((key.code == SCIM_KEY_Shift_L) ||
        (key.code == SCIM_KEY_Shift_R) ||
        (key.code == SCIM_KEY_Control_L) ||
        (key.code == SCIM_KEY_Control_R) ||
        (key.code == SCIM_KEY_Alt_L) ||
        (key.code == SCIM_KEY_Alt_R) ||
        (key.code == SCIM_KEY_Super_L) ||
        (key.code == SCIM_KEY_Super_R) ||
        (key.code == SCIM_KEY_Hyper_L) ||
        (key.code == SCIM_KEY_Hyper_R)) return(true);
    if (key_ascii_mode.comp(key)) {
        mode = ASCII;
        return(true);
    }
    if (key_wascii_mode.comp(key)) {
        mode = WASCII;
        return(true);
    }

    if (key.get_ascii_code() && (!key.is_alt_down()) && (!key.is_control_down())) {
        if ((key.get_ascii_code() == ' ') && (!getTextLength())) return(false);
        // 素直に返すべきもの
        if ((key.code == SCIM_KEY_Return) ||
            (key.code == SCIM_KEY_Linefeed) ||
            (key.code == SCIM_KEY_BackSpace) ||
            (key.code == SCIM_KEY_Tab)) return(false);
        insert(key.get_ascii_code());
        return(true);
    }
    return(false);
}


/*!
    \fn Romkan::getName()
 */
String Romkan::getName()
{
    return(String(_("Roma")));
}




/*!
    \fn Romkan::getAttributeList()
 */
AttributeList Romkan::getAttributeList()
{
    AttributeList l;
    if (buf.length()) {
        Attribute a(pos - buf.length(),buf.length(),SCIM_ATTR_FOREGROUND,SCIM_RGB_COLOR(255,0,0));
        l.push_back(a);
    }
    return(l);
}
