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
#include "wnnconversion.h"
#include <scim_wnn_def.h>

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

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

WnnConversion::WnnConversion(ConfigPointer cfg) : Convertor(cfg)
{
    // とりあえず決めうちでいきます。
    m_iconv.set_encoding ("EUC-JP");
    pos = 0;
    bunsetu = 0;
    wnn = 0;
    sType = Wnn4;
}


WnnConversion::~WnnConversion()
{
}


    /* w_charからEUCに変換するアレ */
void WnnConversion::wstostr(unsigned char *e,w_char *u) {
    w_char x;
    for(;*u;) {
        x = *u ++;
        if (x & 0x8000) {
            *e ++ = x >> 8;
            *e ++ = x;
        } else
        if (x & 0x80) {
            *e ++ = 0x8e;
            *e ++ = x & 0xff;
        } else
            *e++ = x;
    }
    *e ++ = 0;
}

    /* EUCからw_charに変換するナニ */
void WnnConversion::strtows(w_char *u,unsigned char *e) {
    w_char  x;
    for(;*e;) {
        x = *e ++;
        // これいらない↓
        //if (x == 0x8e) x = 0x80 | *e ++;
        if (x & 0x80)
        x = ((x << 8) & 0xff00)  | *e ++;
        *u ++ = x;
    }
    *u = 0;
}

/*!
    \fn WnnConversion::connect()
 */
bool WnnConversion::connect()
{
    String host = config->read(String(SCIM_CONFIG_WNN_SERVER),String(SCIM_DEFAULT_WNN_SERVER));
    String rc = config->read(String(SCIM_CONFIG_WNN_RC),String(SCIM_DEFAULT_WNN_RC));
    String t = config->read(String(SCIM_CONFIG_WNN_SERVERTYPE),String(SCIM_DEFAULT_WNN_SERVERTYPE));

    if (t == "Wnn6") {
        sType = Wnn6;
    } else if (t == "Wnn7") {
        sType = Wnn7;
    } else if (t == "Wnn8") {
        sType = Wnn8;
    } else {
        sType = Wnn4;
    }

    wnnConnect(String("scim-wnn"),host,rc,sType,0);
    return(true);
}

/*!
    \fn WnnConversion::disconnect()
 */
void WnnConversion::disconnect()
{
    wnnDisconnect();
}


/*!
    \fn WnnConversion::wnnConnect(String name,String host,String rc,JServerType type,int timeout)
 */
bool WnnConversion::wnnConnect(String name,String host,String rc,JServerType type,int timeout)
{
    wnn = jl_open((char *)name.c_str(),(char *)host.c_str(),(char *)rc.c_str(),wnn_error,wnn_message,timeout);
    //wnn = jl_open_lang("test","localhost","jp_JP","/usr/lib/wnn7/ja_JP/wnnenvrc",wnn_error,wnn_message,10);
    if (wnn == NULL) return(false);
    #ifdef HAVE_LIBWNN7
    // 予測入力を初期化しておく。
    jl_yosoku_init(wnn);
    #endif
    sType = type;
    return(true);
}

/*!
    \fn WnnConversion::wnnDisconnect()
 */
void WnnConversion::wnnDisconnect()
{
    if (wnn != NULL) {
        #ifdef HAVE_LIBWNN7
        jl_yosoku_free(wnn);
        #endif
        jl_dic_save_all();
        jl_close(wnn);
        wnn = NULL;
    }
}




/*!
    \fn WnnConversion::isConnected()
 */
bool WnnConversion::isConnected()
{
    if (wnn) 
	return(true);
    else 
	return(false);
}




/*!
    \fn WnnConversion::wnn_message (char *s)
 */
int WnnConversion::wnn_message (char *s)
{
    SCIM_DEBUG_IMENGINE(1) << s << "\n";
    return 0;
}

/*!
    \fn WnnConversion::wnn_error (char *s)
 */
int WnnConversion::wnn_error (char *s)
{
    SCIM_DEBUG_IMENGINE(1) << s << "\n";
    return 0;
}




/*!
    \fn WnnConversion::reset()
 */
void WnnConversion::reset()
{
    yomiText.clear();
    bunList.clear();
    yomiList.clear();
    text.clear();
    attr.clear();
    pos = 0;
    jl_kill(wnn,0,-1);
}


/*!
    \fn WnnConversion::setYomiText(WideString yomi)
 */
void WnnConversion::setYomiText(WideString yomi)
{
    yomiText = yomi;
}



/*!
    \fn WnnConversion::ren_conversion()
 */
int WnnConversion::ren_conversion()
{
    convList.Yomi.clear();
    convList.kouho.clear();
    bunList.clear();
    yomiList.clear();
    pos = 0;
    w_char ws[1024];
    char c[2048];
    String y;
    m_iconv.convert(y,yomiText);
    strtows(ws,(unsigned char*)y.c_str());
    bunsetu = jl_fi_ren_conv(wnn,ws,0,-1,WNN_USE_ZENGO);
    if (bunsetu == -1) return(-1);
    for(unsigned int i = 0;i < bunsetu;i ++) {
        WideString w;
        jl_get_kanji(wnn,i,i + 1,ws);
        wstostr((unsigned char*)c,ws);
        m_iconv.convert(w,c,strlen(c));
        bunList.push_back(w);
        jl_get_yomi(wnn,i,i + 1,ws);
        wstostr((unsigned char*)c,ws);
        m_iconv.convert(w,c,strlen(c));
        yomiList.push_back(w);
    }
    createText();
    return(bunsetu);
}


/*!
    \fn WnnConversion::resizeRegion(int w)
 */
bool WnnConversion::resizeRegion(int w)
{
    if (w == 0) return(false);
    if ((yomiList[pos].length() + w) <= 0)  return(false);
    if (((pos + 1) >= yomiList.size()) && (w > 0)) return(false);

    w_char ws[1024];
    char c[2048];
    int s;
    int h = WNN_NO_USE;
    if (pos > 0) h = WNN_USE_MAE;
    else if (pos < bunsetu - 1) h = WNN_USE_ATO;
    bunsetu = jl_fi_nobi_conv(wnn,pos,yomiList[pos].length() + w,-1,h,WNN_SHO);
    convList.kouho.clear();
    bunList.clear();
    yomiList.clear();
    for(unsigned int i = 0;i < bunsetu;i ++) {
        WideString w;
        jl_get_kanji(wnn,i,i + 1,ws);
        wstostr((unsigned char*)c,ws);
        m_iconv.convert(w,c,strlen(c));
        bunList.push_back(w);
        jl_get_yomi(wnn,i,i + 1,ws);
        wstostr((unsigned char*)c,ws);
        m_iconv.convert(w,c,strlen(c));
        yomiList.push_back(w);
    }
    createText();
    return(true);
}


/*!
    \fn WnnConversion::createText()
 */
void WnnConversion::createText()
{
    WideString w;
    caretPos = 0;
    for(unsigned int i = 0;i < bunsetu;i ++) {
        if (pos == i) {
            caretPos = w.length();
            attr.clear();
            Attribute a(w.length(),bunList[i].length(),SCIM_ATTR_DECORATE,SCIM_ATTR_DECORATE_REVERSE);
            attr.push_back(a);
        }
        w = w + bunList[i];
    }
    text = w;
}

/*!
    \fn WnnConversion::getText()
 */
WideString WnnConversion::getText()
{
    return(text);
}


/*!
    \fn WnnConversion::getAttributeList()
 */
AttributeList WnnConversion::getAttributeList()
{
    return(attr);
}


/*!
    \fn WnnConversion::setPos(int p)
 */
int WnnConversion::setPos(int p)
{
    if (p >= bunsetu) p = (p % bunsetu);
    else if (p < 0) p = bunsetu - (abs(p) % bunsetu);

    pos = p;
    createText();
    return(pos);
}


/*!
    \fn WnnConversion::getPos()
 */
int WnnConversion::getPos()
{
    return(pos);
}


/*!
    \fn WnnConversion::getYosokuList(const WideString &str)
 */
ResultList WnnConversion::getYosokuList(const WideString &str)
{
    convList.Yomi = str;
    convList.kouho.clear();
    convList.count = 0;
    convList.pos = 0;
    convList.kType = YOSOKU;
    #ifdef HAVE_LIBWNN7
    String s;
    m_iconv.convert(s,convList.Yomi);
    // @todo ここもmbstowcsしたほうが良いんか？
    char c[1024];
    strcpy(c,s.c_str());
    if (jl_yosoku_yosoku(wnn,c) != 0) return(convList);
    convList.count = ykYosokuKouhoNum;
    for(unsigned int i = 0;i < convList.count;i ++) {
        WideString w;
        m_iconv.convert(w,ykYosokuKouho[i],strlen(ykYosokuKouho[i]));
        convList.kouho.push_back(w);
    }
    //jl_yosoku_free(wnn);
    #endif
    return(convList);
}


/*!
    \fn WnnConversion::getResultList(int p,int kt)
 */
ResultList WnnConversion::getResultList(int p,int kt)
{
        w_char k[1024];
        char buf[2048];
        WideString u;
    convList.kouho.clear();
    convList.Yomi.clear();
    convList.pos = 0;
    convList.count = 0;
    if ((sType != Wnn8) && (sType != Wnn7) && (kt != DEFAULT)) return(convList);

    if (p == -1) p = pos;
    if ((p >= bunsetu) || (p < 0)) p = p % bunsetu;
    pos = p;
    jl_get_yomi(wnn,pos,pos + 1,k);
        wstostr((unsigned char*)buf,k);
        m_iconv.convert(u,buf,strlen(buf));
        convList.Yomi = u;

    // bunsetu connection control
    int conn = WNN_USE_ZENGO;
    if (bunsetu == 1) conn = WNN_NO_USE;
    else if (pos == 0) conn = WNN_USE_ATO;
    else if (pos == (bunsetu - 1)) conn = WNN_USE_MAE;
    switch (kt) {
        #ifdef HAVE_LIBWNN7
        case IKEIJI: {
            convList.pos = jl_zenikeiji_dai(wnn,pos,pos + 1,conn,WNN_UNIQ);
            convList.kType = IKEIJI;
            break;
        }
        case RENSOU: {
            convList.pos = jl_zenassoc_dai(wnn,pos,pos + 1,conn,WNN_UNIQ);
            convList.kType = RENSOU;
            break;
        }
        #endif
        default: {
            convList.pos = jl_zenkouho_dai(wnn,pos,pos + 1,conn,WNN_UNIQ);
            convList.kType = DEFAULT;
            break;
        }
    }

    // @todo be implement to ikeiji conversion using "jl_zenikeiji_dai()".
    // @todo be implement to rensou conversion using "jl_zenassoc_dai()".
    // @todo jl_zenkouho should change to jl_zenkouho_dai.
    //convList.pos = jl_zenassoc_dai(wnn,pos,pos + 1,WNN_USE_ZENGO,WNN_UNIQ);
    //convList.pos = jl_zenkouho(wnn,pos,conn,WNN_UNIQ);
    if (convList.pos == -1) return(convList);
    convList.count = jl_zenkouho_suu(wnn);

    for (unsigned int i = 0;i < convList.count; i ++) {
        jl_get_zenkouho_kanji(wnn,i,k);
        wstostr((unsigned char*)buf,k);
        m_iconv.convert(u,buf,strlen(buf));
        convList.kouho.push_back(u);
    }
    select(convList.pos);
    createText();
    return(convList);

}


/*!
    \fn WnnConversion::select(int p)
 */
bool WnnConversion::select(int p)
{
    if (p > convList.count) p = 0;
    convList.pos = p;
    switch(convList.kType) {
        case DEFAULT: {
            jl_set_jikouho_dai(wnn,p);
            break;
        }
        case YOSOKU: {
            #ifdef HAVE_LIBWNN7
            jl_yosoku_selected_cand(wnn,p);
            #endif
            return(true);
        }
        default: {
            break;
        }
    }

    bunList.at(pos) = convList.kouho.at(p);
    createText();

    return(true);

}






/*!
    \fn WnnConversion::updateFrequency()
 */
void WnnConversion::updateFrequency()
{
    if (bunsetu) {
        #ifdef HAVE_LIBWNN7
        w_char ws[1024];
        int c = jl_get_kanji(wnn,0,bunsetu,ws);
        jl_yosoku_toroku(wnn,ws,c);
        #endif
        jl_optimize_fi(wnn,0,-1);
    }
}



/*!
    \fn WnnConversion::getCaretPos()
 */
int WnnConversion::getCaretPos()
{
    return(caretPos);
}


String WnnConversion::getName()
{
    return(String("Wnn"));
}


