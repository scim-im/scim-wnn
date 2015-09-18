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

/* う　ぜ　ぇ */
#ifndef WNNPROTO_H
#define WNNPROTO_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

extern "C" {

    /* 基礎的なん */
    #define w_char unsigned short
    struct wnn_buf *jl_open_lang(char *,char *,char *,char *,int (*)(char *),int(*)(char *),int);
    #define jl_open(a,b,c,d,e,f) jl_open_lang(a,b,NULL,c,d,e,f)
    void jl_close(struct wnn_buf*);




    /* conversion */
    int jl_fi_ren_conv(struct wnn_buf*,w_char*,int,int,int);
    int jl_optimize_fi(struct wnn_buf*,int,int);
    int jl_fi_nobi_conv(struct wnn_buf*,int,int,int,int,int);
    int jl_zenkouho_suu(struct wnn_buf*);
    int jl_kill(struct wnn_buf*,int,int);
    int jl_zenkouho(struct wnn_buf*,int,int,int);
    int jl_zenkouho_dai(struct wnn_buf*,int,int,int,int);

    int jl_set_jikouho(struct wnn_buf*,int);
    int jl_set_jikouho_dai(struct wnn_buf*,int);

    #ifdef HAVE_LIBWNN7
        /* wnn6sdk is not having it.*/
        int jl_zenikeiji_dai(struct wnn_buf*,int,int,int,int);
        int jl_zenassoc_dai(struct wnn_buf*,int,int,int,int);
        int wnn_get_area(struct wnn_buf*,int,int,w_char*,int,int);
        void jl_get_zenkouho_kanji(struct wnn_buf*,int ,w_char *,int);
        #define jl_get_zenkouho_kanji(a,b,c) jl_get_zenkouho_kanji(a,b,c,512)
        #define jl_get_kanji(buf,bun1,bun2,area) wnn_get_area(buf,bun1,bun2,area,1,512)
        #define jl_get_yomi(buf,bun1,bun2,area) wnn_get_area(buf,bun1,bun2,area,0,512)

        // input completion for Wnn7
        extern int ykYosokuKouhoNum;
        extern char **ykYosokuKouho;
        extern int jl_yosoku_init(struct wnn_buf*);
        extern int jl_yosoku_set_param(struct wnn_buf*,int,int,int);
        extern int jl_yosoku_free(struct wnn_buf*);
        extern int jl_yosoku_yosoku(struct wnn_buf*,char *);
        extern int jl_yosoku_toroku(struct wnn_buf*,w_char*,short);
        extern int jl_yosoku_selected_cand(struct wnn_buf*,int);
        extern int jl_yosoku_delete_cand(struct wnn_buf*,int);
        extern int jl_yosoku_save_datalist(struct wnn_buf*);
        extern int jl_yosoku_cancel_toroku(struct wnn_buf *);
        extern int jl_yosoku_ikkatsu_toroku(struct wnn_buf *,char *,int);
        extern int jl_yosoku_ikkatsu_toroku_init(struct wnn_buf *);
        extern int jl_yosoku_ikkatsu_toroku_end(struct wnn_buf *);
        extern int jl_yosoku_toroku(struct wnn_buf *,w_char *,short);
        extern int jl_dic_save_all();

    #else
        int wnn_get_area(struct wnn_buf*,int,int,w_char*,int);
        void jl_get_zenkouho_kanji(struct wnn_buf*,int ,w_char *);
        #define jl_get_kanji(buf,bun1,bun2,area) wnn_get_area(buf,bun1,bun2,area,1)
        #define jl_get_yomi(buf,bun1,bun2,area) wnn_get_area(buf,bun1,bun2,area,0)
    #endif


    #define WNN_NO_USE    0
    #define WNN_USE_MAE   1
    #define WNN_USE_ATO   2
    #define WNN_USE_ZENGO (WNN_USE_MAE | WNN_USE_ATO)

    #define WNN_UNIQ_KNJ 2
    #define WNN_UNIQ 1
    #define WNN_NO_UNIQ  0

    #define WNN_NO_CREATE 0
    #define WNN_CREATE (-1)

    #define WNN_SHO 0
    #define WNN_DAI 1


}

#endif

