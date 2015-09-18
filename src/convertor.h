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
#ifndef CONVERTOR_H
#define CONVERTOR_H

#define Uses_SCIM_CONFIG_BASE

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <scim.h>
#include <scim_iconv.h>
#include <scim_attribute.h>


using namespace std;
using namespace scim;

/**
@author TAM (Teppei Tamra)
*/
namespace ScimWnn {
class ResultList{
public:
    WideString Yomi;
    int pos;
    int count;
    int kType;
    vector<WideString> kouho;
};

class Convertor{
public:
    Convertor(ConfigPointer cfg);

    ~Convertor();
    virtual bool isConnected();
    virtual void reset();
    virtual void setYomiText(WideString yomi);
    virtual int ren_conversion();
    virtual WideString getText();
    virtual int setPos(int p);
    virtual int getPos();
    virtual ResultList getResultList(int p = -1 ,int kt = 0);
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
    ConfigPointer config;


};

}
using namespace ScimWnn;
#endif
