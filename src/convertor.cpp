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
#include "convertor.h"

/* 全部ダミーメソッドさぁー♪ */

Convertor::Convertor(ConfigPointer cfg)
{
    config = cfg;
}


Convertor::~Convertor()
{
}

bool Convertor::isConnected() {
    return(false);
}

void Convertor::reset(){
    return;
}

void Convertor::setYomiText(WideString yomi) {
    return;
}

int Convertor::ren_conversion() {
    return(0);
}

WideString Convertor::getText() {
    return(WideString());
}

int Convertor::setPos(int p){
    return(0);
}

int Convertor::getPos() {
    return(0);
}

ResultList Convertor::getResultList(int p,int kt){
    return(ResultList());
}

ResultList Convertor::getYosokuList(const WideString &str) {
    return(ResultList());
}

bool Convertor::select(int p) {
    return(false);
}

AttributeList Convertor::getAttributeList() {
    return(AttributeList());
}

bool Convertor::resizeRegion(int w) {
    return(false);
}

void Convertor::updateFrequency() {
    return;
}

bool Convertor::connect() {
    return(false);
}
void Convertor::disconnect() {
    return;
}

int Convertor::getCaretPos() {
    return(0);
}


String Convertor::getName()
{
    return(String("Convertor"));
}
