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

// ��ݲ��Τ��ޤ��ʤ���
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
    {SCIM_KEY_kana_fullstop,        "��"},
    {SCIM_KEY_kana_openingbracket,  "��"},
    {SCIM_KEY_kana_closingbracket,  "��"},
    {SCIM_KEY_kana_comma,           "��"},
    {SCIM_KEY_kana_conjunctive,     "��"},
    {SCIM_KEY_kana_middledot,       "��"},
    {SCIM_KEY_kana_WO,              "��"},
    {SCIM_KEY_kana_a,               "��"},
    {SCIM_KEY_kana_i,               "��"},
    {SCIM_KEY_kana_u,               "��"},
    {SCIM_KEY_kana_e,               "��"},
    {SCIM_KEY_kana_o,               "��"},
    {SCIM_KEY_kana_ya,              "��"},
    {SCIM_KEY_kana_yu,              "��"},
    {SCIM_KEY_kana_yo,              "��"},
    {SCIM_KEY_kana_tsu,             "��"},
    {SCIM_KEY_kana_tu,              "��"},
    {SCIM_KEY_prolongedsound,       "��"},
    {SCIM_KEY_kana_A,               "��"},
    {SCIM_KEY_kana_I,               "��"},
    {SCIM_KEY_kana_U,               "��"},
    {SCIM_KEY_kana_E,               "��"},
    {SCIM_KEY_kana_O,               "��"},
    {SCIM_KEY_kana_KA,              "��"},
    {SCIM_KEY_kana_KI,              "��"},
    {SCIM_KEY_kana_KU,              "��"},
    {SCIM_KEY_kana_KE,              "��"},
    {SCIM_KEY_kana_KO,              "��"},
    {SCIM_KEY_kana_SA,              "��"},
    {SCIM_KEY_kana_SHI,             "��"},
    {SCIM_KEY_kana_SU,              "��"},
    {SCIM_KEY_kana_SE,              "��"},
    {SCIM_KEY_kana_SO,              "��"},
    {SCIM_KEY_kana_TA,              "��"},
    {SCIM_KEY_kana_CHI,             "��"},
    {SCIM_KEY_kana_TI,              "��"},
    {SCIM_KEY_kana_TSU,             "��"},
    {SCIM_KEY_kana_TU,              "��"},
    {SCIM_KEY_kana_TE,              "��"},
    {SCIM_KEY_kana_TO,              "��"},
    {SCIM_KEY_kana_NA,              "��"},
    {SCIM_KEY_kana_NI,              "��"},
    {SCIM_KEY_kana_NU,              "��"},
    {SCIM_KEY_kana_NE,              "��"},
    {SCIM_KEY_kana_NO,              "��"},
    {SCIM_KEY_kana_HA,              "��"},
    {SCIM_KEY_kana_HI,              "��"},
    {SCIM_KEY_kana_FU,              "��"},
    {SCIM_KEY_kana_HU,              "��"},
    {SCIM_KEY_kana_HE,              "��"},
    {SCIM_KEY_kana_HO,              "��"},
    {SCIM_KEY_kana_MA,              "��"},
    {SCIM_KEY_kana_MI,              "��"},
    {SCIM_KEY_kana_MU,              "��"},
    {SCIM_KEY_kana_ME,              "��"},
    {SCIM_KEY_kana_MO,              "��"},
    {SCIM_KEY_kana_YA,              "��"},
    {SCIM_KEY_kana_YU,              "��"},
    {SCIM_KEY_kana_YO,              "��"},
    {SCIM_KEY_kana_RA,              "��"},
    {SCIM_KEY_kana_RI,              "��"},
    {SCIM_KEY_kana_RU,              "��"},
    {SCIM_KEY_kana_RE,              "��"},
    {SCIM_KEY_kana_RO,              "��"},
    {SCIM_KEY_kana_WA,              "��"},
    {SCIM_KEY_kana_N,               "��"},

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
    // ���äƤ����٤���Ρ�
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
        iconvert.convert(vs1,String("�������������������������ĤƤȤϤҤդؤۤѤԤפڤ�"));
        iconvert.convert(vs2,String("�����������������������¤ŤǤɤФӤ֤٤ܤФӤ֤٤�"));
        for(unsigned int i = 0;i < vs1.length();i ++) {
            if (vs1[i] == text[pos - 1]) text = text.substr(0,pos - 1) + vs2[i] + text.substr(pos);
        }
        return(true);
    }
    if ((((key.get_ascii_code() == '[') && (fakeKanaInput)) || (key.code == SCIM_KEY_semivoicedsound )) && (pos > 0)) {
        WideString vs1,vs2;
        iconvert.convert(vs1,String("�ϤҤդؤۤФӤ֤٤�"));
        iconvert.convert(vs2,String("�ѤԤפڤݤѤԤפڤ�"));
        for(unsigned int i = 0;i < vs1.length();i ++) {
            if (vs1[i] == text[pos - 1]) text = text.substr(0,pos - 1) + vs2[i] + text.substr(pos);
        }
        return(true);
    }
    if (fakeKanaInput) {
        char f1[] = "1234567890-^qwertyuiopasdfghjkl;:]zxcvbnm,./\\!\"#$%&'()~=~|QWERTYUIOP`{ASDFGHJKL+*}ZXCVBNM<>?_";
        WideString f2;
        iconvert.convert(f2,String("�̤դ������������ۤؤ��Ƥ�������ʤˤ餻���Ȥ��Ϥ����ޤΤ�줱��Ĥ����Ҥ��ߤ�ͤ����ɤ�������������������Ƥ�������ʤˤ餻���֤��Ȥ��Ϥ����ޤΤ�ܡ��פä����Ҥ��ߤ⡢������"));
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

