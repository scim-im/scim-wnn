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

// って言うか先にWnnをC++向けに優しく包み込む母のようなライブラリ激しく希望。
// ああそうですね、「お前がやれ」ですよね。

#define Uses_SCIM_UTILITY
#define Uses_SCIM_IMENGINE
#define Uses_SCIM_LOOKUP_TABLE
#define Uses_SCIM_CONFIG_BASE
#define Uses_SCIM_ICONV

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

// scimのおまじない。
#include "scim_wnn_imengine.h"
#include "scim_wnn_def.h"

#define scim_module_init wnn_LTX_scim_module_init
#define scim_module_exit wnn_LTX_scim_module_exit
#define scim_imengine_module_init wnn_LTX_scim_imengine_module_init
#define scim_imengine_module_create_factory wnn_LTX_scim_imengine_module_create_factory
#ifndef SCIM_WNN_ICON_FILE
  #define SCIM_WNN_ICON_FILE           (SCIM_ICONDIR "/scim-wnn.png")
#endif


static Pointer <WnnFactory> _scim_wnn_factory;
static ConfigPointer _scim_config;

extern "C" {
    void scim_module_init (void)
    {
        bindtextdomain (GETTEXT_PACKAGE, SCIM_WNN_LOCALEDIR);
        bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    }

    void scim_module_exit (void)
    {
        _scim_wnn_factory.reset ();
        _scim_config.reset ();
    }

    unsigned int scim_imengine_module_init (const ConfigPointer &config)
    {
        _scim_config = config;
        return 1;
    }

    IMEngineFactoryPointer scim_imengine_module_create_factory (unsigned int factory)
    {
        if (factory != 0) return NULL;
        if (_scim_wnn_factory.null ()) {
            _scim_wnn_factory =
                new WnnFactory (utf8_mbstowcs (String (_("Wnn"))),String("ja_JP"));
        }
        return _scim_wnn_factory;
    }
}

WnnFactory::WnnFactory() {
    m_name = utf8_mbstowcs(_("Wnn"));
    set_languages(String("ja_JP"));
}

WnnFactory::~ WnnFactory() {
}

WnnFactory::WnnFactory(const WideString & name, const String & languages) {
    if (name.length () <= 8)
        m_name = name;
    else
        m_name.assign (name.begin (), name.begin () + 8);
    if (languages == String ("default"))
        set_languages (String (_("ja_JP")));
    else
        set_languages (languages);
}

WideString WnnFactory::get_name () const
{
    return m_name;
}

WideString WnnFactory::get_authors () const
{
    return utf8_mbstowcs (String (_("(C)2004-2006 TAM(Teppei Tamra) <tam-t@par.odn.ne.jp>")));
}

WideString WnnFactory::get_credits () const
{
    return WideString ();
}

WideString WnnFactory::get_help () const
{
    return utf8_mbstowcs (String (_("SCIM-WNN HELP")));
}

String WnnFactory::get_uuid () const
{
    return String ("c069395a-d62f-4a77-8229-446e44a99976");
}

String WnnFactory::get_icon_file () const
{
    return String (SCIM_WNN_ICON_FILE);
}

IMEngineInstancePointer WnnFactory::create_instance (const String& encoding, int id)
{
    return new WnnInstance (this, encoding, id);
}



// キーイベントチェック

void WnnKeyEventList::append(const KeyEvent &k) {
    push_back(k);
}

void WnnKeyEventList::append(int code,int mask) {
    KeyEvent k;
    k.code = code;
    k.mask = mask;
    append(k);
}

bool WnnKeyEventList::comp(const KeyEvent &k) {
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


// 現物。


WnnInstance::WnnInstance (WnnFactory *factory, const String& encoding, int id)
    : IMEngineInstanceBase (factory, encoding, id)
{
    SCIM_DEBUG_IMENGINE(1) << "!!! WNNINSTANCE !!!\n";
    m_iconv.set_encoding ("EUC-JP");
    preeditors.push_back(new Romkan(_scim_config));
    preeditors.push_back(new KanaInput(_scim_config));
    m_preeditor = preeditors.at(0);
    wnn = 0;
    init();
}

WnnInstance::~WnnInstance()
{
    for(unsigned int i = 0;i < preeditors.size();i ++) {
        delete(preeditors.at(i));
    }
    if (wnn->isConnected()) wnn->disconnect();
}


/*!
    \fn WnnInstance::init()
 */
void WnnInstance::init()
{
    m_conversion = false;
    m_yosoku = false;
    m_lookup = false;

    // @todo if connected to jserver, should disconnect this.

    alp = _scim_config->read(String(SCIM_CONFIG_WNN_ALP),SCIM_DEFAULT_WNN_ALP);
    mini_status = _scim_config->read(String(SCIM_CONFIG_WNN_MINISTATUS),SCIM_DEFAULT_WNN_MINISTATUS);
    numkeyselect = _scim_config->read(String(SCIM_CONFIG_WNN_NUMKEY_SELECT),SCIM_DEFAULT_WNN_NUMKEY_SELECT);
    yosoku = _scim_config->read(String(SCIM_CONFIG_WNN_YOSOKU),SCIM_DEFAULT_WNN_YOSOKU);
    sType = _scim_config->read(String(SCIM_CONFIG_WNN_SERVERTYPE),String(SCIM_DEFAULT_WNN_SERVERTYPE));
    
    if (wnn) delete(wnn);
    wnn = new WnnConversion(_scim_config);

    
    defaultPreEditor = _scim_config->read(String(SCIM_CONFIG_WNN_DEFAULT_PREEDITOR),String(SCIM_DEFAULT_WNN_DEFAULT_PREEDITOR));
    auto_conversion = _scim_config->read(String(SCIM_CONFIG_WNN_AUTO_CONVERSION),SCIM_DEFAULT_WNN_AUTO_CONVERSION);;

    // デフォルトキー設定。
    scim_string_to_key_list(k_conversion_start,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_CONVERSION_START),
            String(SCIM_DEFAULT_WNN_KEY_CONVERSION_START)));
    scim_string_to_key_list(k_cancel,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_CANCEL),
            String(SCIM_DEFAULT_WNN_KEY_CANCEL)));
    scim_string_to_key_list(k_delete,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_DELETE),
            String(SCIM_DEFAULT_WNN_KEY_DELETE)));
    scim_string_to_key_list(k_backspace,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_BACKSPACE),
            String(SCIM_DEFAULT_WNN_KEY_BACKSPACE)));
    scim_string_to_key_list(k_commit,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_COMMIT),
            String(SCIM_DEFAULT_WNN_KEY_COMMIT)));
    scim_string_to_key_list(k_conversion_next,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_CONVERSION_NEXT),
            String(SCIM_DEFAULT_WNN_KEY_CONVERSION_NEXT)));
    scim_string_to_key_list(k_conversion_prev,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_CONVERSION_PREV),
            String(SCIM_DEFAULT_WNN_KEY_CONVERSION_PREV)));
    scim_string_to_key_list(k_conversion_expand,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_CONVERSION_EXPAND),
            String(SCIM_DEFAULT_WNN_KEY_CONVERSION_EXPAND)));
    scim_string_to_key_list(k_conversion_shrink,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_CONVERSION_SHRINK),
            String(SCIM_DEFAULT_WNN_KEY_CONVERSION_SHRINK)));
    scim_string_to_key_list(k_conversion_forward,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_CONVERSION_FORWARD),
            String(SCIM_DEFAULT_WNN_KEY_CONVERSION_FORWARD)));
    scim_string_to_key_list(k_conversion_backward,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_CONVERSION_BACKWARD),
            String(SCIM_DEFAULT_WNN_KEY_CONVERSION_BACKWARD)));
    scim_string_to_key_list(k_forward,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_FORWARD),
            String(SCIM_DEFAULT_WNN_KEY_FORWARD)));
    scim_string_to_key_list(k_backward,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_BACKWARD),
            String(SCIM_DEFAULT_WNN_KEY_BACKWARD)));
    scim_string_to_key_list(k_home,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_HOME),
            String(SCIM_DEFAULT_WNN_KEY_HOME)));
    scim_string_to_key_list(k_end,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_END),
            String(SCIM_DEFAULT_WNN_KEY_END)));
    scim_string_to_key_list(k_lookup_popup,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_LOOKUPPOPUP),
            String(SCIM_DEFAULT_WNN_KEY_LOOKUPPOPUP)));
    scim_string_to_key_list(k_lookup_pageup,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_LOOKUPPAGEUP),
            String(SCIM_DEFAULT_WNN_KEY_LOOKUPPAGEUP)));
    scim_string_to_key_list(k_lookup_pagedown,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_LOOKUPPAGEDOWN),
            String(SCIM_DEFAULT_WNN_KEY_LOOKUPPAGEDOWN)));
    scim_string_to_key_list(k_convert_hiragana,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_CONVERT_HIRAGANA),
            String(SCIM_DEFAULT_WNN_KEY_CONVERT_HIRAGANA)));
    scim_string_to_key_list(k_convert_katakana,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_CONVERT_KATAKANA),
            String(SCIM_DEFAULT_WNN_KEY_CONVERT_KATAKANA)));
    scim_string_to_key_list(k_convert_half,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_CONVERT_HALF),
            String(SCIM_DEFAULT_WNN_KEY_CONVERT_HALF)));
    scim_string_to_key_list(k_convert_wide,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_CONVERT_WIDE),
            String(SCIM_DEFAULT_WNN_KEY_CONVERT_WIDE)));

    scim_string_to_key_list(k_conversion_rensou,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_CONVERSION_RENSOU),
            String(SCIM_DEFAULT_WNN_KEY_CONVERSION_RENSOU)));
    scim_string_to_key_list(k_conversion_ikeiji,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_CONVERSION_IKEIJI),
            String(SCIM_DEFAULT_WNN_KEY_CONVERSION_IKEIJI)));
    scim_string_to_key_list(k_select_yosoku,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_SELECT_YOSOKU),
            String(SCIM_DEFAULT_WNN_KEY_SELECT_YOSOKU)));
    scim_string_to_key_list(k_auto_conversion,
        _scim_config->read(String(SCIM_CONFIG_WNN_KEY_AUTO_CONVERSION),
            String(SCIM_DEFAULT_WNN_KEY_AUTO_CONVERSION)));

    changePreEditor(defaultPreEditor);

    // ここで一度接続しておこう。
    wnn->connect();

}


/*!
    \fn WnnInstance::changePreEditor(const String &name)
 */
void WnnInstance::changePreEditor(const String &name)
{
    for(unsigned int i = 0;i < preeditors.size();i ++) {
        if (preeditors[i]->getPropertyName() == name) {
            m_preeditor = preeditors[i];
            return;
        }
    }
    return;
}


/*!
    \fn WnnInstance::updateProperty()
 */
void WnnInstance::updateProperty()
{
    if (m_proplist.empty()) {
        Property p;
        p = Property(SCIM_PROP_WNN_INPUTMODE,"",String(""),_("input mode"));
        m_proplist.push_back(p);
        for(unsigned int i = 0;i < preeditors.size();i ++) {
            p = Property(String(SCIM_PROP_WNN_INPUTMODE) + String("/") + preeditors[i]->getPropertyName(),
                preeditors[i]->getName(),String(""),_("mode status"));
            m_proplist.push_back(p);
        }
        p = Property(SCIM_PROP_WNN_MODESTATUS,"",String(""),_("mode status"));
        m_proplist.push_back(p);
        p = Property(SCIM_PROP_WNN_CONVERSIONMODE,"",String(""),_("conversion mode"));
        m_proplist.push_back(p);
    }
    PropertyList::iterator it;
    it = find(m_proplist.begin(),m_proplist.end(),SCIM_PROP_WNN_INPUTMODE);
    if (it != m_proplist.end()) {
        it->set_label(m_preeditor->getName());
    }
    update_property(*it);
    it = find(m_proplist.begin(),m_proplist.end(),SCIM_PROP_WNN_MODESTATUS);
    if (it != m_proplist.end()) {
        if (m_conversion) it->set_label(_("Kanji"));
        else if (m_yosoku) it->set_label(_("Yosoku"));
        else it->set_label(m_preeditor->getModeName());
    }
    update_property(*it);
    it = find(m_proplist.begin(),m_proplist.end(),SCIM_PROP_WNN_CONVERSIONMODE);
    if (it != m_proplist.end()) {
        if (auto_conversion) it->set_label(_("AUTO"));
        else  it->set_label(_("REN"));
    }
    update_property(*it);
    register_properties(m_proplist);
}


/*!
    \fn WnnInstance::updatePreEditor()
 */
void WnnInstance::updatePreEditor()
{
    if (m_preeditor->getTextLength()) {
        if (auto_conversion && (m_preeditor->getTextLength() == m_preeditor->getPos()))
            autoConversion();
        else {
            show_preedit_string();
            update_preedit_string(m_preeditor->getText(),m_preeditor->getAttributeList());
            update_preedit_caret(m_preeditor->getPos());
        }
        if ((!m_conversion) && yosoku && wnn->isConnected() && (sType == "Wnn7") && (!auto_conversion)) {
            m_convList = wnn->getYosokuList(m_preeditor->getText());
            if (m_convList.count) {
                m_lookup_table.clear();
                for(unsigned int i = 0;i < m_convList.count;i ++) {
                    m_lookup_table.append_candidate(m_convList.kouho.at(i));
                }
                startLookup();
            } else {
                //m_lookup_table.clear();
                hide_lookup_table();
            }
        } else hide_lookup_table();
    } else {
        hide_preedit_string();
        hide_lookup_table();
    }
    if (mini_status) {
        update_aux_string(utf8_mbstowcs(m_preeditor->getModeName()));
        show_aux_string();
    } else {
        hide_aux_string();
    }

    updateProperty();
}
/*!
    \fn WnnInstance::updateConversion()
 */
void WnnInstance::updateConversion()
{
    update_preedit_string(wnn->getText(),wnn->getAttributeList());
    update_preedit_caret(wnn->getCaretPos());
    if (m_lookup) {
        m_lookup_table.set_cursor_pos(m_convList.pos);
        update_lookup_table(m_lookup_table);
        show_lookup_table();
    } else {
        hide_lookup_table();
        hide_aux_string();
    }
    updateProperty();
}


bool WnnInstance::process_key_event (const KeyEvent& key)
{
    if (key.is_key_release()) return(false);
    KeyEvent ke = key;
    if (ke.mask & SCIM_KEY_CapsLockMask) ke.mask -= SCIM_KEY_CapsLockMask;
    if (ke.mask & SCIM_KEY_NumLockMask) ke.mask -= SCIM_KEY_NumLockMask;
    if (ke.mask & SCIM_KEY_ScrollLockMask) ke.mask -= SCIM_KEY_ScrollLockMask;
    if (m_conversion) return(process_conversion_key_event(ke));
    else if (m_yosoku) return(process_yosoku_key_event(ke));
    else return(process_preedit_key_event(ke));
}


/*!
    \fn WnnInstance::process_preedit_key_event(const KeyEvent &key)
 */
bool WnnInstance::process_preedit_key_event(const KeyEvent &key)
{
    // PreEditorのキーイベントフック。
    if (m_preeditor->keyEventHook(key)) {
        updatePreEditor();
        return(true);
    } else

    // バッファが空では無い場合。
    if (m_preeditor->getTextLength()) {
        if (k_conversion_start.comp(key)) {
            startConversion(m_preeditor->getText(true));
            alp_count ++;
            return(true);
        } else
        if (k_commit.comp(key)) {
            if (auto_conversion) {
                commit_string(wnn->getText());
                wnn->reset();
            } else commit_string(m_preeditor->getText(true));
            m_preeditor->reset();
            updatePreEditor();
            return(true);
        } else
        if (k_forward.comp(key) || k_backward.comp(key)) {
            k_backward.comp(key) ? m_preeditor->setPos(m_preeditor->getPos() - 1) : m_preeditor->setPos(m_preeditor->getPos() + 1);
            updatePreEditor();
            return(true);
        } else
        if (k_home.comp(key) || k_end.comp(key)) {
            k_end.comp(key) ? m_preeditor->setPos(m_preeditor->getTextLength()) : m_preeditor->setPos(0);
            updatePreEditor();
            return(true);
        } else
        if ((k_backspace.comp(key)) || (k_delete.comp(key))) {
            k_backspace.comp(key) ? m_preeditor->backspace(): m_preeditor->del();
            updatePreEditor();
            return(true);
        } else
        if (k_convert_hiragana.comp(key)) {
            m_preeditor->kataHira();
            updatePreEditor();
            return(true);
        } else
        if (k_convert_katakana.comp(key)) {
            m_preeditor->hiraKata();
            updatePreEditor();
            return(true);
        } else
        if (k_convert_half.comp(key)) {
            m_preeditor->toHalf();
            updatePreEditor();
            return(true);
        } else
        if (k_convert_wide.comp(key)) {
            m_preeditor->toWide();
            updatePreEditor();
            return(true);
        } else
        if (k_select_yosoku.comp(key) && yosoku) {
            if ((m_convList.kType == YOSOKU) && (m_convList.count)) {
                return(process_yosoku_key_event(key));
            }
        }
    }

    // バッファの存在にかかわらず。
    if (k_cancel.comp(key)) {
        if (m_preeditor->cancelEvent()) {
            updatePreEditor();
            return(true);
        }
        if (m_preeditor->getTextLength()) {
            m_preeditor->reset();
        }
        updatePreEditor();
        return(true);
    }

    if (k_auto_conversion.comp(key)) {
        auto_conversion ? auto_conversion = false : auto_conversion = true;
        updatePreEditor();
        return(true);
    }

    if (m_preeditor->inputEvent(key)) {
        updatePreEditor();

        return(true);
    } else {
        // preeditorで処理できなかった場合はやはりcommitしてアプリケーションに返すべきだ。
        if (m_preeditor->getTextLength()) {
            if (auto_conversion) {
                commit_string(wnn->getText());
                wnn->reset();
            } else commit_string(m_preeditor->getText(true));
        }
        m_preeditor->reset();
        updatePreEditor();
    }

    return(false);
}

/*!
    \fn WnnInstance::process_conversion_key_event(const KeyEvent &key)
 */
bool WnnInstance::process_conversion_key_event(const KeyEvent &key)
{
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

    if ((alp <= alp_count) && (alp != 0)) {
        if (!m_lookup) startLookup();
    }

    if (k_lookup_popup.comp(key)) {
        if (m_lookup) return(true);
        startLookup();
        return(true);
    } else
    if (k_lookup_pageup.comp(key)) {
        if (m_lookup) {
            lookup_table_page_up();
        }
    } else
    if (k_lookup_pagedown.comp(key)) {
        if (m_lookup) {
            lookup_table_page_down();
        }
    } else
    if (k_commit.comp(key)) {
        commit_string(wnn->getText());
        wnn->updateFrequency();
        m_preeditor->reset();
        wnn->reset();
        m_conversion = false;
        updatePreEditor();
        m_lookup = false;
        alp_count = 1;
        return(true);
    } else
    if (k_cancel.comp(key) || k_backspace.comp(key)) {
        wnn->reset();
        m_conversion = false;
        m_lookup = false;
        alp_count = 0;
        updatePreEditor();
        return(true);
    } else
    if (k_conversion_next.comp(key) || k_conversion_prev.comp(key)) {
        k_conversion_prev.comp(key) ? m_convList.pos --: m_convList.pos ++;
        if (m_convList.pos >= m_convList.count) m_convList.pos = 0;
        else if (m_convList.pos < 0) m_convList.pos = m_convList.count - 1;
        alp_count ++;

        wnn->select(m_convList.pos);
        updateConversion();
        return(true);
    } else
    if (k_conversion_expand.comp(key) || k_conversion_shrink.comp(key)) {
        bool r;
        k_conversion_shrink.comp(key) ? r = wnn->resizeRegion(-1) : r = wnn->resizeRegion(1);
        if (!r) return(true);
        m_convList = wnn->getResultList();
        if (alp == -1) {
            startLookup();
        } else m_lookup = false;
        updateConversion();
        alp_count = 1;
        return(true);
    } else
    if (k_conversion_rensou.comp(key) && ((sType == "Wnn7") || (sType == "Wnn8")) ) {
        m_convList = wnn->getResultList(wnn->getPos(),RENSOU);
        if (m_convList.count == 0) return(true);
        startLookup();
        updateConversion();
        alp_count = 1;
        return(true);
    } else
    if (k_conversion_ikeiji.comp(key) && ((sType == "Wnn7") || (sType == "Wnn8")) ) {
        m_convList = wnn->getResultList(wnn->getPos(),IKEIJI);
        if (m_convList.count == 0) return(true);
        startLookup();
        updateConversion();
        alp_count = 1;
        return(true);
    } else
    if (k_conversion_forward.comp(key) || k_conversion_backward.comp(key)) {
        k_conversion_backward.comp(key) ? wnn->setPos(wnn->getPos() - 1) : wnn->setPos(wnn->getPos() + 1);
        m_convList = wnn->getResultList();
        if (alp == -1) {
            startLookup();
        } else m_lookup = false;
        updateConversion();
        alp_count = 1;
        return(true);
    } else
    if (k_convert_hiragana.comp(key) || k_convert_katakana.comp(key)) {
        WideString res = m_convList.Yomi;
        k_convert_hiragana.comp(key) ? m_preeditor->convKataHira(res) : m_preeditor->convHiraKata(res);
        for(unsigned int i = 0;i < m_convList.count;i ++) {
            if (res == m_convList.kouho[i]) {
                m_convList.pos = i;
                wnn->select(m_convList.pos);
                updateConversion();
                break;
            }
        }
        return(true);
    } else
    if (key.get_ascii_code()) {
        if (key.mask & (SCIM_KEY_AltMask | SCIM_KEY_ControlMask)) return(false);
            if (m_lookup && numkeyselect) {
                char numk[] = "1234567890";
                int numc = 0;
                bool numkey = false;
                for(;numk[numc]; ++ numc) {
                    if (key.get_ascii_code() == numk[numc]) {
                        numkey = true;
                        break;
                    }
                }
                if (numkey) {
                    if (m_lookup_table.get_current_page_size() < numc) return(true);
                    m_convList.pos = numc + m_lookup_table.get_current_page_start();
                    wnn->select(m_convList.pos);
                    wnn->setPos(wnn->getPos() + 1);
                    m_convList = wnn->getResultList();
                    updateConversion();
                    startLookup();
                    return(true);
                }
            }

        commit_string(wnn->getText());
        wnn->updateFrequency();
        m_preeditor->reset();
        wnn->reset();
        m_conversion = false;
        m_lookup = false;
        alp_count = 0;
        updatePreEditor();
        return(process_preedit_key_event(key));
    }


    return(true);
}



/*!
    \fn WnnInstance::process_yosoku_key_event(const KeyEvent &key)
 */
bool WnnInstance::process_yosoku_key_event(const KeyEvent &key)
{
    if (!m_yosoku) {
        m_yosoku = true;
        update_preedit_string(m_convList.kouho.at(m_convList.pos));
        update_preedit_caret(0);
        show_preedit_string();
        updateProperty();
        return(true);
    }

    if (k_lookup_pageup.comp(key)) {
        lookup_table_page_up();
        return(true);
    } else
    if (k_lookup_pagedown.comp(key)) {
        lookup_table_page_down();
        return(true);
    } else
    if (k_conversion_next.comp(key) || k_conversion_prev.comp(key) || k_select_yosoku.comp(key)) {
        k_conversion_prev.comp(key) ? m_convList.pos --: m_convList.pos ++;
        if (m_convList.pos >= m_convList.count) m_convList.pos = 0;
        else if (m_convList.pos < 0) m_convList.pos = m_convList.count - 1;
        m_lookup_table.set_cursor_pos(m_convList.pos);
        update_lookup_table(m_lookup_table);
        update_preedit_string(m_convList.kouho.at(m_convList.pos));
        update_preedit_caret(0);
//        select_candidate((unsigned int)m_convList.pos);
        updateProperty();
        return(true);
    } else
    if (k_commit.comp(key)) {
        m_yosoku = false;
        commit_string(m_convList.kouho.at(m_convList.pos));
        m_preeditor->reset();
        updatePreEditor();
        return(true);
    } else
    if (k_cancel.comp(key) || k_backspace.comp(key)) {
        m_yosoku = false;
        updatePreEditor();
        return(true);
    }
    if (!key.get_ascii_code()) return(true);
    if ((!(key.is_control_down() || key.is_alt_down() || key.is_shift_down())) && numkeyselect) {
        char numk[] = "1234567890";
        for(int numc = 0;numk[numc]; ++ numc) {
            if (key.get_ascii_code() == numk[numc]) {
                if (m_lookup_table.get_current_page_size() > numc)
                    select_candidate((unsigned int)numc);
                return(true);
            }
        }
    }

    m_yosoku = false;
    commit_string(m_convList.kouho.at(m_convList.pos));
    m_preeditor->reset();
    updatePreEditor();
    return(process_preedit_key_event(key));
}



void WnnInstance::move_preedit_caret (unsigned int pos)
{
    //if (!m_conversion) m_preeditor->setPos(pos);
    //update_preedit_caret(pos);
}

void WnnInstance::select_candidate (unsigned int item)
{
    if (!m_lookup_table.number_of_candidates()) return;

    int p = m_lookup_table.get_current_page_start() + item;
    m_convList.pos = p;
    wnn->select(m_convList.pos);
    if (m_convList.kType != YOSOKU) {
        update_preedit_string(wnn->getText(),wnn->getAttributeList());
        update_preedit_caret(wnn->getCaretPos());
    } else {
        update_preedit_string(m_convList.kouho.at(m_convList.pos));
        update_preedit_caret(0);
        if (!m_yosoku) {
            // マウスで選択した場合は予測選択モードに突入。
            m_yosoku = true;
            show_preedit_string();
            updateProperty();
        }
    }
    m_lookup_table.set_cursor_pos(m_convList.pos);
    update_lookup_table(m_lookup_table);
}

void WnnInstance::update_lookup_table_page_size (unsigned int page_size)
{
    m_lookup_table.set_page_size (page_size);
}

void WnnInstance::lookup_table_page_up ()
{
    if (!m_lookup_table.number_of_candidates () || !m_lookup_table.get_current_page_start ()) return;

    int p = m_convList.pos - m_lookup_table.get_current_page_size();
    if (p < 0) p = 0;
    m_convList.pos = p;
    wnn->select(m_convList.pos);
    update_preedit_string(wnn->getText(),wnn->getAttributeList());
    update_preedit_caret(wnn->getCaretPos());
    m_lookup_table.set_cursor_pos(m_convList.pos);
    update_lookup_table(m_lookup_table);
}

void WnnInstance::lookup_table_page_down ()
{
    if (!m_lookup_table.number_of_candidates () ||
        m_lookup_table.get_current_page_start () + m_lookup_table.get_current_page_size () >=
          m_lookup_table.number_of_candidates ())
        return;

    int p = m_convList.pos + m_lookup_table.get_current_page_size();
    if (p >= m_convList.count) p = m_convList.count - 1;
    m_convList.pos = p;
    wnn->select(m_convList.pos);
    update_preedit_string(wnn->getText(),wnn->getAttributeList());
    update_preedit_caret(wnn->getCaretPos());
    m_lookup_table.set_cursor_pos(m_convList.pos);
    update_lookup_table(m_lookup_table);
}

void WnnInstance::reset ()
{
    // Qt-immodule+scim-qtimmではこのメソッドが大量に呼ばれるようだが、他ではどうなのか？。
    alp_count = 0;
    m_conversion = false;
    m_lookup = false;
    wnn->reset();
    m_preeditor->reset();
    //if (wnn->isConnected()) wnn->wnnDisconnect();
    //wnn->wnnConnect(String("scim-wnn"),host,rc,sType,0);
    //updatePreEditor();
}

void WnnInstance::focus_in ()
{
    if (!m_conversion) updatePreEditor();
    else updateProperty();
}

void WnnInstance::focus_out ()
{
    if (m_conversion) {
        commit_string(wnn->getText());
        wnn->updateFrequency();
        wnn->reset();
        m_conversion = false;
        m_lookup = false;
        alp_count = 0;
    } else if (m_preeditor->getTextLength()) {
        commit_string(m_preeditor->getText(true));
    }
    m_preeditor->reset();
    updatePreEditor();
}

void WnnInstance::trigger_property (const String &property)
{
    String s = SCIM_PROP_WNN_INPUTMODE;
    if ((property.length() > s.length()) && (property.substr(0,s.length()) == s)) {
        changePreEditor(property.substr(s.length() + 1));
        updateProperty();
    }
    s = SCIM_PROP_WNN_CONVERSIONMODE;
    if (property == s) {
        auto_conversion ? auto_conversion = false : auto_conversion = true;
        updatePreEditor();
    }
}






/*!
    \fn WnnInstance::startConversion(WideString s)
 */
void WnnInstance::startConversion(WideString s)
{
    // 一度lookupは消しておこう。
    m_lookup = false;
    m_lookup_table.clear();
    hide_lookup_table();

    hide_aux_string();
    if (!wnn->isConnected()) {
        if (!wnn->connect()) {
            update_aux_string(utf8_mbstowcs(String(_("could not connect to jserver."))));
            show_aux_string();
            return;
        }
    }

    wnn->setYomiText(s);
    if (wnn->ren_conversion() == -1) {
        update_aux_string(utf8_mbstowcs(String(_("could not connect to jserver."))));
        show_aux_string();
        return;
    }
    m_convList.kouho.clear();
    m_convList = wnn->getResultList();
    m_conversion = true;

    alp_count = 1;

    show_preedit_string();
    update_preedit_string(wnn->getText(),wnn->getAttributeList());
    update_preedit_caret(wnn->getCaretPos());
    if (alp == -1) {
        startLookup();
    }
    updateProperty();
}


/*!
    \fn WnnInstance::autoConversion()
 */
void WnnInstance::autoConversion()
{
    if (!wnn->isConnected()) {
        if (!wnn->connect()) {
            update_aux_string(utf8_mbstowcs(String(_("could not connect to jserver."))));
            show_aux_string();
            show_preedit_string();
            update_preedit_string(m_preeditor->getText(),m_preeditor->getAttributeList());
            update_preedit_caret(m_preeditor->getPos());
            return;
        }
    }

    wnn->reset();
    wnn->setYomiText(m_preeditor->getText(true));
    if (wnn->ren_conversion() == -1) {
        update_aux_string(utf8_mbstowcs(String(_("could not connect to jserver."))));
        show_aux_string();
        show_preedit_string();
        update_preedit_string(m_preeditor->getText(),m_preeditor->getAttributeList());
        update_preedit_caret(m_preeditor->getPos());
        return;
    }

    show_preedit_string();
    update_preedit_string(wnn->getText());
    update_preedit_caret(wnn->getText().size());
}


/*!
    \fn WnnInstance::createLookupTable(ResultList cList)
 */
void WnnInstance::createLookupTable(ResultList cList)
{
    hide_lookup_table();
    m_lookup_table.clear();
    if (!cList.count) return;
    for (unsigned int i = 0;i < cList.count;i ++) {
        m_lookup_table.append_candidate(cList.kouho.at(i));
    }
    m_lookup_table.set_cursor_pos(cList.pos);
    update_lookup_table(m_lookup_table);
}


/*!
    \fn WnnInstance::startLookup()
 */
void WnnInstance::startLookup()
{
    createLookupTable(m_convList);
    if (m_convList.count == 0) return;
    m_lookup = true;
    switch(m_convList.kType) {
        case IKEIJI: {
            update_aux_string(utf8_mbstowcs(String(_("Ikeiji lookup result"))));
            break;
        }
        case RENSOU: {
            update_aux_string(utf8_mbstowcs(String(_("association lookup result"))));
            break;
        }
        case YOSOKU: {
            update_aux_string(utf8_mbstowcs(String(_("yosoku lookup result"))));
            break;
        }
        default: {
            update_aux_string(utf8_mbstowcs(String(_("lookup result"))));
            break;
        }
    }
    show_aux_string();
    show_lookup_table();
}






