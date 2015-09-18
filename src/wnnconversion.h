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
#ifndef WNNCONVERSION_H
#define WNNCONVERSION_H


#include <cstdio>
#include <cstdlib>
#include <vector>
#include <wnnproto.h>
#define Uses_SCIM_ICONV
#include <scim.h>
#include <scim_iconv.h>
#include <scim_attribute.h>
#include <convertor.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif



using namespace scim;
using namespace std;


/**
@author TAM(Teppei Tamra)
*/
namespace ScimWnn {
enum JServerType { Wnn4, Wnn6, Wnn7, Wnn8 };

#define DEFAULT 0
#define IKEIJI  1
#define RENSOU  2
#define YOSOKU  3


class WnnConversion : public Convertor {
public:
    WnnConversion(ConfigPointer cfg);

    ~WnnConversion();
    virtual bool isConnected();
    virtual void reset();
    virtual void setYomiText(WideString yomi);
    virtual int ren_conversion();
    virtual WideString getText();
    virtual int setPos(int p);
    virtual int getPos();
    virtual ResultList getResultList(int p = -1 ,int kt = DEFAULT);
    virtual ResultList getYosokuList(const WideString &str);
    virtual bool select(int p);
    virtual AttributeList getAttributeList();
    virtual bool resizeRegion(int w);
    virtual void updateFrequency();
    virtual bool connect();
    virtual void disconnect();
    virtual int getCaretPos();
    virtual String getName();


protected:
    bool wnnConnect(String name,String host,String rc,JServerType type,int timeout);
    void wnnDisconnect();
    void wstostr(unsigned char *e,w_char *u);
    void strtows(w_char *u,unsigned char *e);
    static int wnn_message (char *s);
    static int wnn_error (char *s);
    void createText();

protected:
    struct wnn_buf *wnn;
    String wnn_server;
    String wnn_rc;
    JServerType sType;
    int wnn_timeout;
    IConvert m_iconv;
    WideString yomiText;
    int pos;
    int bunsetu;
    int caretPos;
    WideString text;
    vector<WideString> bunList;
    vector<WideString> yomiList;
    AttributeList attr;
    ResultList convList;

};
}
#endif
