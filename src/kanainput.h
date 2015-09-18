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
#ifndef KANAINPUT_H
#define KANAINPUT_H

#define Uses_SCIM_ICONV
#define Uses_SCIM_CONFIG_BASE
#include <vector>
#include <scim.h>
#include <scim_iconv.h>
#include <scim_event.h>
#include "preeditor.h"

using namespace std;
using namespace scim;

/**
@author TAM(Teppei Tamra)
*/
namespace ScimWnn {

struct KanaInputKey {
    int code;
    const char kana[6];
};


class KanaInput : public PreEditor {
public:
    KanaInput(ConfigPointer cfg);

    ~KanaInput();
    virtual bool inputEvent(const KeyEvent &key);
    virtual String getName();
    virtual String getModeName();
    virtual String getPropertyName() {return(String("KanaInput"));};

protected:
    bool fakeKanaInput;
};
}

#endif
