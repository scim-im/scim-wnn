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
#include "kanainput.h"
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

namespace ScimWnn {
const KanaInputKey KanaInputTable[] = {
    {SCIM_KEY_kana_fullstop,        "。"},
    {SCIM_KEY_kana_openingbracket,  "「"},
    {SCIM_KEY_kana_closingbracket,  "」"},
    {SCIM_KEY_kana_comma,           "、"},
    {SCIM_KEY_kana_conjunctive,     "・"},
    {SCIM_KEY_kana_middledot,       "・"},
    {SCIM_KEY_kana_WO,              "を"},
    {SCIM_KEY_kana_a,               "ぁ"},
    {SCIM_KEY_kana_i,               "ぃ"},
    {SCIM_KEY_kana_u,               "ぅ"},
    {SCIM_KEY_kana_e,               "ぇ"},
    {SCIM_KEY_kana_o,               "ぉ"},
    {SCIM_KEY_kana_ya,              "ゃ"},
    {SCIM_KEY_kana_yu,              "ゅ"},
    {SCIM_KEY_kana_yo,              "ょ"},
    {SCIM_KEY_kana_tsu,             "っ"},
    {SCIM_KEY_kana_tu,              "っ"},
    {SCIM_KEY_prolongedsound,       "ー"},
    {SCIM_KEY_kana_A,               "あ"},
    {SCIM_KEY_kana_I,               "い"},
    {SCIM_KEY_kana_U,               "う"},
    {SCIM_KEY_kana_E,               "え"},
    {SCIM_KEY_kana_O,               "お"},
    {SCIM_KEY_kana_KA,              "か"},
    {SCIM_KEY_kana_KI,              "き"},
    {SCIM_KEY_kana_KU,              "く"},
    {SCIM_KEY_kana_KE,              "け"},
    {SCIM_KEY_kana_KO,              "こ"},
    {SCIM_KEY_kana_SA,              "さ"},
    {SCIM_KEY_kana_SHI,             "し"},
    {SCIM_KEY_kana_SU,              "す"},
    {SCIM_KEY_kana_SE,              "せ"},
    {SCIM_KEY_kana_SO,              "そ"},
    {SCIM_KEY_kana_TA,              "た"},
    {SCIM_KEY_kana_CHI,             "ち"},
    {SCIM_KEY_kana_TI,              "ち"},
    {SCIM_KEY_kana_TSU,             "つ"},
    {SCIM_KEY_kana_TU,              "つ"},
    {SCIM_KEY_kana_TE,              "て"},
    {SCIM_KEY_kana_TO,              "と"},
    {SCIM_KEY_kana_NA,              "な"},
    {SCIM_KEY_kana_NI,              "に"},
    {SCIM_KEY_kana_NU,              "ぬ"},
    {SCIM_KEY_kana_NE,              "ね"},
    {SCIM_KEY_kana_NO,              "の"},
    {SCIM_KEY_kana_HA,              "は"},
    {SCIM_KEY_kana_HI,              "ひ"},
    {SCIM_KEY_kana_FU,              "ふ"},
    {SCIM_KEY_kana_HU,              "ふ"},
    {SCIM_KEY_kana_HE,              "へ"},
    {SCIM_KEY_kana_HO,              "ほ"},
    {SCIM_KEY_kana_MA,              "ま"},
    {SCIM_KEY_kana_MI,              "み"},
    {SCIM_KEY_kana_MU,              "む"},
    {SCIM_KEY_kana_ME,              "め"},
    {SCIM_KEY_kana_MO,              "も"},
    {SCIM_KEY_kana_YA,              "や"},
    {SCIM_KEY_kana_YU,              "ゆ"},
    {SCIM_KEY_kana_YO,              "よ"},
    {SCIM_KEY_kana_RA,              "ら"},
    {SCIM_KEY_kana_RI,              "り"},
    {SCIM_KEY_kana_RU,              "る"},
    {SCIM_KEY_kana_RE,              "れ"},
    {SCIM_KEY_kana_RO,              "ろ"},
    {SCIM_KEY_kana_WA,              "わ"},
    {SCIM_KEY_kana_N,               "ん"},

    {0,""}
};
}

KanaInput::KanaInput(ConfigPointer cfg) : PreEditor(cfg)
{
    fakeKanaInput = cfg->read(SCIM_CONFIG_WNN_KANAINPUT_FAKEKANA,SCIM_DEFAULT_WNN_KANAINPUT_FAKEKANA);
}


KanaInput::~KanaInput()
{
}

/*!
    \fn KanaInput::getModeName()
 */
String KanaInput::getModeName()
{
    return(_("KANA"));
}

/*!
    \fn KanaInput::getName()
 */
String KanaInput::getName()
{
    return(String(_("Kana")));
}


/*!
    \fn KanaInput::inputEvent(const KeyEvent &key)
 */
bool KanaInput::inputEvent(const KeyEvent &key)
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

    for(unsigned int i = 0;KanaInputTable[i].code != 0;++ i) {
        if (key.code == KanaInputTable[i].code) {
            WideString w;
            iconvert.convert(w,String(KanaInputTable[i].kana));
            text = text.substr(0,pos) + w + text.substr(pos);
            pos ++;
            return(true);
        }
    }
    if ((((key.get_ascii_code() == '@') && (fakeKanaInput)) || (key.code == SCIM_KEY_voicedsound )) && (pos > 0)) {
        WideString vs1,vs2;
        iconvert.convert(vs1,String("かきくけこさしすせそたちつてとはひふへほぱぴぷぺぽ"));
        iconvert.convert(vs2,String("がぎぐげござじずぜぞだぢづでどばびぶべぼばびぶべぼ"));
        for(unsigned int i = 0;i < vs1.length();i ++) {
            if (vs1[i] == text[pos - 1]) text = text.substr(0,pos - 1) + vs2[i] + text.substr(pos);
        }
        return(true);
    }
    if ((((key.get_ascii_code() == '[') && (fakeKanaInput)) || (key.code == SCIM_KEY_semivoicedsound )) && (pos > 0)) {
        WideString vs1,vs2;
        iconvert.convert(vs1,String("はひふへほばびぶべぼ"));
        iconvert.convert(vs2,String("ぱぴぷぺぽぱぴぷぺぽ"));
        for(unsigned int i = 0;i < vs1.length();i ++) {
            if (vs1[i] == text[pos - 1]) text = text.substr(0,pos - 1) + vs2[i] + text.substr(pos);
        }
        return(true);
    }
    if (fakeKanaInput) {
        char f1[] = "1234567890-^qwertyuiopasdfghjkl;:]zxcvbnm,./\\!\"#$%&'()~=~|QWERTYUIOP`{ASDFGHJKL+*}ZXCVBNM<>?_";
        WideString f2;
        iconvert.convert(f2,String("ぬふあうえおやゆよわほへたていすかんなにらせちとしはきくまのりれけむつさそひこみもねるめろ！”ぁぅぇぉゃゅょを＝〜ーたていすかんなにらせ｀「ちとしはきくまのり＋＊」っさそひこみも、。・＿"));
        for(unsigned i = 0;f1[i] != 0;i ++) {
            if (key.get_ascii_code() == f1[i]) {
                text = text.substr(0,pos) + f2[i] + text.substr(pos);
                pos ++;
                return(true);
            }
        }
    }
    if (isprint(key.code)) {
        String s;
        s += key.get_ascii_code();
        text = text.substr(0,pos) + utf8_mbstowcs(s) + text.substr(pos);
        pos ++;
        return(true);
    }
    return(false);
}

