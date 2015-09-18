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
#ifndef SCIM_WNN_IMENGINE_H
#define SCIM_WNN_IMENGINE_H

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <romkan.h>
#include <kanainput.h>
#include <wnnconversion.h>
#define Uses_SCIM_ICONV
#include <scim.h>
#include <scim_iconv.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


using namespace scim;
using namespace std;

class WnnFactory : public IMEngineFactoryBase
{
protected:
    friend class WnnInstance;
    WideString m_name;
public:
    WnnFactory ();
    WnnFactory (const WideString & name, const String & languages);
    virtual ~WnnFactory ();
    virtual WideString get_name () const;
    virtual WideString get_authors () const;
    virtual WideString get_credits () const;
    virtual WideString get_help () const;
    virtual String get_uuid () const;
    virtual String get_icon_file () const;
    virtual IMEngineInstancePointer create_instance (const String& encoding, int id = -1);
protected:

};

class WnnKeyEventList : public KeyEventList
{
    // �ꥹ�Ȥ˥������٥�Ȥ��ɲä��ơ�comp(KeyEvent)��Ƚ�̡�
public:
    void append(const KeyEvent &k);
    void append(int code,int mask);
    bool comp(const KeyEvent &k);
};


class WnnInstance : public IMEngineInstanceBase
{
public:
    WnnInstance (WnnFactory *factory, const String  &encoding, int id = -1);
    virtual ~WnnInstance ();
    virtual bool process_key_event (const KeyEvent& key);
    virtual void move_preedit_caret (unsigned int pos);
    virtual void select_candidate (unsigned int item);
    virtual void update_lookup_table_page_size (unsigned int page_size);
    virtual void lookup_table_page_up ();
    virtual void lookup_table_page_down ();
    virtual void reset ();
    virtual void focus_in ();
    virtual void focus_out ();
    virtual void trigger_property (const String &property);
    void startLookup();

protected:
    CommonLookupTable m_lookup_table;
    PreEditor *m_preeditor;
    Convertor *wnn;
    bool m_conversion;
    bool m_yosoku;
    bool m_lookup;
    ResultList m_convList;
    PropertyList m_proplist;
    int alp;
    int alp_count;
    bool numkeyselect;
    bool mini_status;
    bool yosoku;
    bool auto_conversion;
    String defaultPreEditor;
    IConvert m_iconv;
    vector<PreEditor *> preeditors;
    String sType;
protected:
    bool process_preedit_key_event(const KeyEvent &key);
    bool process_conversion_key_event(const KeyEvent &key);
    bool process_yosoku_key_event(const KeyEvent &key);
    void startConversion(WideString s);
    void createLookupTable(ResultList cList);
    void init();
    void updatePreEditor();
    void updateProperty();
    void updateConversion();
    void changePreEditor(const String &name);
    void autoConversion();

private:
    WnnKeyEventList
        k_conversion_start,     // �Ѵ�����
        k_cancel,               // ����󥻥륭��
        k_delete,               // ���
        k_backspace,            // �Хå����ڡ���
        k_commit,               // ����
        k_conversion_next,      // ������
        k_conversion_prev,      // ������
        k_conversion_expand,    // ����
        k_conversion_shrink,    // �̾�
        k_conversion_forward,   // ��ʸ��
        k_conversion_backward,  // ��ʸ��
        k_forward,              // ���������ư/��
        k_backward,             // ���������ư/���
        k_home,                 // ���������ư/��Ƭ
        k_end,                  // ���������ư/����
        k_lookup_popup,         // �������ɽ��
        k_lookup_pageup,        // �����������
        k_lookup_pagedown,      // �����������
        k_convert_hiragana,     // �Ҥ餬���Ѵ�
        k_convert_katakana,     // ���������Ѵ�
        k_convert_half,         // Ⱦ���Ѵ�
        k_convert_wide,         // �����Ѵ�
        k_auto_conversion,      // �Ѵ��⡼�ɥȥ���

        // for Wnn7
        k_conversion_ikeiji,    // �۷����Ѵ�
        k_conversion_rensou,    // Ϣ���Ѵ�
        k_select_yosoku,        // ͽ¬����

        k_dummy;
};


#endif

