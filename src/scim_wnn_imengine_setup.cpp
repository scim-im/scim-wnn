/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 *  Copyright (C) 2004 TAM (Teppei Tamra)
 *  Copyright (C) 2004 Hiroyuki Ikezoe
 *  Copyright (C) 2004 Takuro Ashie
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
 * Based on scim-hangul.
 * Copyright (c) 2004 James Su <suzhe@turbolinux.com.cn>
 */

#define Uses_SCIM_CONFIG_BASE

#include <gtk/gtk.h>

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <scim.h>
#include <gtk/scimkeyselection.h>
#include "scim_wnn_def.h"

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

using namespace scim;

#define scim_module_init wnn_imengine_setup_LTX_scim_module_init
#define scim_module_exit wnn_imengine_setup_LTX_scim_module_exit

#define scim_setup_module_create_ui       wnn_imengine_setup_LTX_scim_setup_module_create_ui
#define scim_setup_module_get_category    wnn_imengine_setup_LTX_scim_setup_module_get_category
#define scim_setup_module_get_name        wnn_imengine_setup_LTX_scim_setup_module_get_name
#define scim_setup_module_get_description wnn_imengine_setup_LTX_scim_setup_module_get_description
#define scim_setup_module_load_config     wnn_imengine_setup_LTX_scim_setup_module_load_config
#define scim_setup_module_save_config     wnn_imengine_setup_LTX_scim_setup_module_save_config
#define scim_setup_module_query_changed   wnn_imengine_setup_LTX_scim_setup_module_query_changed



static GtkWidget * create_setup_window ();
static void        load_config (const ConfigPointer &config);
static void        save_config (const ConfigPointer &config);
static bool        query_changed ();

// Module Interface.

extern "C" {
    void scim_module_init (void)
    {
        bindtextdomain (GETTEXT_PACKAGE, SCIM_WNN_LOCALEDIR);
        bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    }

    void scim_module_exit (void)
    {
    }

    GtkWidget * scim_setup_module_create_ui (void)
    {
        return create_setup_window ();
    }

    String scim_setup_module_get_category (void)
    {
        return String ("IMEngine");
    }

    String scim_setup_module_get_name (void)
    {
        return String (_("Wnn"));
    }

    String scim_setup_module_get_description (void)
    {
        return String (_("A Wnn IMEngine Module."));
    }

    void scim_setup_module_load_config (const ConfigPointer &config)
    {
        load_config (config);
    }

    void scim_setup_module_save_config (const ConfigPointer &config)
    {
        save_config (config);
    }

    bool scim_setup_module_query_changed ()
    {
        return query_changed ();
    }
} // extern "C"


// Internal data structure
struct KeyboardConfigData
{
    const char *key;
    String      data;
    const char *label;
    const char *title;
    const char *tooltip;
    GtkWidget  *entry;
    GtkWidget  *button;
};

struct KeyboardConfigPage
{
    const char         *label;
    KeyboardConfigData *data;
};

struct ListData
{
    GtkWidget *widget;
    String name;
    gchar *label;
};

// Internal data declaration.
//static bool __config_use_kana = false;
static String __config_server = SCIM_DEFAULT_WNN_SERVER;
static String __config_wnn_rc = SCIM_DEFAULT_WNN_RC;
static String __config_servertype = SCIM_DEFAULT_WNN_SERVERTYPE;
static String __config_preeditor = SCIM_DEFAULT_WNN_DEFAULT_PREEDITOR;
static bool __config_yosoku = SCIM_DEFAULT_WNN_YOSOKU;
static bool __config_numkeyselect = SCIM_DEFAULT_WNN_NUMKEY_SELECT;
static bool __config_autoconversion = SCIM_DEFAULT_WNN_AUTO_CONVERSION;
static int __config_alp = SCIM_DEFAULT_WNN_ALP;
static String __config_romkan_table = SCIM_DEFAULT_WNN_ROMKAN_TABLE_FILE;

static bool __have_changed    = true;

//static GtkWidget    * __widget_use_kana = 0;
static GtkWidget    * __widget_server = 0;
static GtkWidget    * __widget_servertype = 0;
static GtkWidget    * __widget_preeditor = 0;
static GtkWidget    * __widget_wnn_rc = 0;
static GtkWidget    * __widget_yosoku = 0;
static GtkWidget    * __widget_alp = 0;
static GtkWidget    * __widget_numkeyselect = 0;
static GtkWidget    * __widget_autoconversion = 0;
static GtkWidget    * __widget_romkan_table = 0;
static GtkTooltips  * __widget_tooltips = 0;

static ListData __list_data [] =
{
    {
        NULL,
        String("Wnn4"),
        N_("Wnn4 or FreeWnn"),
    },
    {
        NULL,
        String("Wnn6"),
        N_("Wnn6"),
    },
    {
        NULL,
        String("Wnn7"),
        N_("Wnn7"),
    },
    {
        NULL,
        String("Wnn8"),
        N_("Wnn8"),
    },
    {
        NULL,
        String("Romkan"),
        N_("Roma-Kana input"),
    },
    {
        NULL,
        String("KanaInput"),
        N_("Kana input"),
    },
    {
        NULL,
        String(),
        NULL,
    },
};

static KeyboardConfigData __config_keyboards_common [] =
{
    {
        SCIM_CONFIG_WNN_KEY_COMMIT,
        SCIM_DEFAULT_WNN_KEY_COMMIT,
        N_("Commit keys:"),
        N_("Select commit keys"),
        N_("The key events to commit the preedit string. "),
        NULL,
        NULL,
    },
    {
        SCIM_CONFIG_WNN_KEY_CONVERSION_START,
        SCIM_DEFAULT_WNN_KEY_CONVERSION_START,
        N_("Convert keys:"),
        N_("Select convert keys"),
        N_("The key events to convert the preedit string to kanji. "),
        NULL,
        NULL,
    },
    {
        SCIM_CONFIG_WNN_KEY_CANCEL,
        SCIM_DEFAULT_WNN_KEY_CANCEL,
        N_("Cancel keys:"),
        N_("Select cancel keys"),
        N_("The key events to cancel preediting or converting. "),
        NULL,
        NULL,
    },
    {
        SCIM_CONFIG_WNN_KEY_BACKSPACE,
        SCIM_DEFAULT_WNN_KEY_BACKSPACE,
        N_("Backspace keys:"),
        N_("Select backspace keys"),
        N_("The key events to delete a character before caret. "),
        NULL,
        NULL,
    },
    {
        SCIM_CONFIG_WNN_KEY_DELETE,
        SCIM_DEFAULT_WNN_KEY_DELETE,
        N_("Delete keys:"),
        N_("Select delete keys"),
        N_("The key events to delete a character after caret. "),
        NULL,
        NULL,
    },
    {
        NULL,
        "",
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    },
};

static KeyboardConfigData __config_keyboards_caret [] =
{
    {
        SCIM_CONFIG_WNN_KEY_FORWARD,
        SCIM_DEFAULT_WNN_KEY_FORWARD,
        N_("Move forward keys:"),
        N_("Select move caret forward keys"),
        N_("The key events to move the caret to forward. "),
        NULL,
        NULL,
    },
    {
        SCIM_CONFIG_WNN_KEY_BACKWARD,
        SCIM_DEFAULT_WNN_KEY_BACKWARD,
        N_("Move backward keys:"),
        N_("Select move caret backward keys"),
        N_("The key events to move the caret to backward. "),
        NULL,
        NULL,
    },
    {
        NULL,
        "",
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    },
};

static KeyboardConfigData __config_keyboards_segment [] =
{
    {
        SCIM_CONFIG_WNN_KEY_CONVERSION_FORWARD,
        SCIM_DEFAULT_WNN_KEY_CONVERSION_FORWARD,
        N_("Next segment keys:"),
        N_("Select next segment keys"),
        N_("The key events to select next segment. "),
        NULL,
        NULL,
    },
    {
        SCIM_CONFIG_WNN_KEY_CONVERSION_BACKWARD,
        SCIM_DEFAULT_WNN_KEY_CONVERSION_BACKWARD,
        N_("Previous segment keys:"),
        N_("Select previous segment keys"),
        N_("The key events to select previous segment. "),
        NULL,
        NULL,
    },
    {
        SCIM_CONFIG_WNN_KEY_CONVERSION_SHRINK,
        SCIM_DEFAULT_WNN_KEY_CONVERSION_SHRINK,
        N_("Shrink segment keys:"),
        N_("Select shrink segment keys"),
        N_("The key events to shrink the selected segment. "),
        NULL,
        NULL,
    },
    {
        SCIM_CONFIG_WNN_KEY_CONVERSION_EXPAND,
        SCIM_DEFAULT_WNN_KEY_CONVERSION_EXPAND,
        N_("Expand segment keys:"),
        N_("Select expand segment keys"),
        N_("The key events to expand the selected segment. "),
        NULL,
        NULL,
    },
    {
        NULL,
        "",
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    },
};

static KeyboardConfigData __config_keyboards_candidates [] =
{
    {
        SCIM_CONFIG_WNN_KEY_CONVERSION_NEXT,
        SCIM_DEFAULT_WNN_KEY_CONVERSION_NEXT,
        N_("Next keys:"),
        N_("Select next candidate keys"),
        N_("The key events to select next candidate. "),
        NULL,
        NULL,
    },
    {
        SCIM_CONFIG_WNN_KEY_CONVERSION_PREV,
        SCIM_DEFAULT_WNN_KEY_CONVERSION_PREV,
        N_("Previous keys:"),
        N_("Select previous candidate keys"),
        N_("The key events to select previous candidate. "),
        NULL,
        NULL,
    },
    {
        SCIM_CONFIG_WNN_KEY_LOOKUPPOPUP,
        SCIM_DEFAULT_WNN_KEY_LOOKUPPOPUP,
        N_("candidates list keys:"),
        N_("Select candidates list keys"),
        N_("The key events to popup candidates list. "),
        NULL,
        NULL,
    },
    {
        SCIM_CONFIG_WNN_KEY_LOOKUPPAGEUP,
        SCIM_DEFAULT_WNN_KEY_LOOKUPPAGEUP,
        N_("Page up keys:"),
        N_("Select page up candidates keys"),
        N_("The key events to select page up candidates. "),
        NULL,
        NULL,
    },
    {
        SCIM_CONFIG_WNN_KEY_LOOKUPPAGEDOWN,
        SCIM_DEFAULT_WNN_KEY_LOOKUPPAGEDOWN,
        N_("Page down keys:"),
        N_("Select page down candidates keys"),
        N_("The key events to select page down candidates. "),
        NULL,
        NULL,
    },
    {
        NULL,
        "",
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    },
};

static KeyboardConfigData __config_keyboards_other [] =
{
    {
        SCIM_CONFIG_WNN_KEY_AUTO_CONVERSION,
        SCIM_DEFAULT_WNN_KEY_AUTO_CONVERSION,
        N_("auto conversion mode keys:"),
        N_("Select auto conversion mode keys"),
        N_("The key events to change auto conversion mode. "),
        NULL,
        NULL,
    },
    {
        SCIM_CONFIG_WNN_KEY_SELECT_YOSOKU,
        SCIM_DEFAULT_WNN_KEY_SELECT_YOSOKU,
        N_("Yosoku keys:"),
        N_("Select yosoku keys"),
        N_("The key events to select from completion list. "),
        NULL,
        NULL,
    },
    {
        SCIM_CONFIG_WNN_KEY_CONVERSION_IKEIJI,
        SCIM_DEFAULT_WNN_KEY_CONVERSION_IKEIJI,
        N_("Convert to ikeiji keys:"),
        N_("Select ikeiji keys"),
        N_("The key events to convert the selected candidate to ikei-kanji. "),
        NULL,
        NULL,
    },
    {
        SCIM_CONFIG_WNN_KEY_CONVERSION_RENSOU,
        SCIM_DEFAULT_WNN_KEY_CONVERSION_RENSOU,
        N_("Convert to rensou keys:"),
        N_("Select rensou keys"),
        N_("The key events to convert the selected candidate to the associated acceptation string.  "),
        NULL,
        NULL,
    },
    {
        SCIM_CONFIG_WNN_KEY_CONVERT_HIRAGANA,
        SCIM_DEFAULT_WNN_KEY_CONVERT_HIRAGANA,
        N_("Convert to HIRAGANA keys:"),
        N_("Select convert to HIRAGANA keys"),
        N_("The key events to convert the selected candidate or the preedit string to HIRAGANA. "),
        NULL,
        NULL,
    },
    {
        SCIM_CONFIG_WNN_KEY_CONVERT_KATAKANA,
        SCIM_DEFAULT_WNN_KEY_CONVERT_KATAKANA,
        N_("Convert to KATAKANA keys:"),
        N_("Select convert to KATAGANA keys"),
        N_("The key events to convert the selected candidate or the preedit string to KATAKANA. "),
        NULL,
        NULL,
    },
    {
        NULL,
        "",
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    },
};

static struct KeyboardConfigPage __key_conf_pages[] =
{
    {N_("Common keys"),     __config_keyboards_common},
    {N_("Preedit keys"),    __config_keyboards_caret},
    {N_("Segment keys"), __config_keyboards_segment},
    {N_("Candidates keys"), __config_keyboards_candidates},
    {N_("Other function keys"), __config_keyboards_other},
};
static unsigned int __key_conf_pages_num = sizeof (__key_conf_pages) / sizeof (KeyboardConfigPage);

static void on_default_editable_changed      (GtkEditable *editable,
                                              gpointer     user_data);
static void on_default_number_changed        (GtkEditable *editable,
                                              gpointer     user_data);
static void on_default_toggle_button_toggled (GtkToggleButton *togglebutton,
                                              gpointer         user_data);
static void on_default_key_selection_clicked (GtkButton *button,
                                              gpointer   user_data);
static void on_default_file_selection_button (GtkButton *button,
                                              gpointer user_data);
static void on_default_list_changed          (GtkList *list,
                                              GtkWidget *widget,
                                              gpointer     user_data);
static void setup_widget_value (void);


static GtkWidget *
create_options_page ()
{
    GtkWidget *vbox, *table, *label, *button, *button2;
    unsigned int i;
    GList *list = NULL;

    vbox = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox);

    for(i = 0;__list_data[i].label;i ++) {
        __list_data[i].widget = gtk_list_item_new_with_label(_(__list_data[i].label));
        gtk_widget_show (__list_data[i].widget);
    }

    //__widget_use_kana = gtk_check_button_new_with_mnemonic (_("Use _Kana Table"));
    //gtk_widget_show (__widget_use_kana);
    //gtk_box_pack_start (GTK_BOX (vbox), __widget_use_kana, FALSE, FALSE, 4);
    //gtk_container_set_border_width (GTK_CONTAINER (__widget_use_kana), 4);

    // Connect all signals.
    //g_signal_connect ((gpointer) __widget_use_kana, "toggled",
    //                  G_CALLBACK (on_default_toggle_button_toggled),
    //                  &__config_use_kana);

    table = gtk_table_new (3, 6, FALSE);
    gtk_box_pack_start (GTK_BOX (vbox), table, FALSE, FALSE, 0);
    gtk_widget_show (table);

    /* server */
    label = gtk_label_new_with_mnemonic (_("_Server: "));
    gtk_widget_show (label);
    gtk_misc_set_alignment (GTK_MISC (label), 1.0, 0.5);
    gtk_misc_set_padding (GTK_MISC (label), 4, 0);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 4, 4);

    __widget_server = gtk_entry_new ();
    gtk_widget_show (__widget_server);
    gtk_table_attach (GTK_TABLE (table), __widget_server, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_FILL|GTK_EXPAND),
                      (GtkAttachOptions) (GTK_FILL), 4, 4);
    gtk_label_set_mnemonic_widget (GTK_LABEL (label), __widget_server);
    if (!__widget_tooltips)
        __widget_tooltips = gtk_tooltips_new();
    gtk_tooltips_set_tip (__widget_tooltips, __widget_server,
                              _("input the server name to connect."), NULL);


    /* wnnenvrc */
    label = gtk_label_new_with_mnemonic (_("_wnnenvrc: "));
    gtk_widget_show (label);
    gtk_misc_set_alignment (GTK_MISC (label), 1.0, 0.5);
    gtk_misc_set_padding (GTK_MISC (label), 4, 0);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 4, 4);

    __widget_wnn_rc = gtk_entry_new ();
    gtk_widget_show (__widget_wnn_rc);
    gtk_table_attach (GTK_TABLE (table), __widget_wnn_rc, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_FILL|GTK_EXPAND),
                      (GtkAttachOptions) (GTK_FILL), 4, 4);
    gtk_label_set_mnemonic_widget (GTK_LABEL (label), __widget_wnn_rc);

    button = gtk_button_new_with_label (_("..."));
    gtk_widget_show (button);
    gtk_table_attach (GTK_TABLE (table), button, 2, 3, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 4, 4);
    //gtk_label_set_mnemonic_widget (GTK_LABEL (label), button);
    gtk_tooltips_set_tip (__widget_tooltips, __widget_wnn_rc,
                              _("input the path of wnnenvrc."), NULL);

    /* servertype */
    label = gtk_label_new_with_mnemonic (_("Server_type: "));
    gtk_widget_show (label);
    gtk_misc_set_alignment (GTK_MISC (label), 1.0, 0.5);
    gtk_misc_set_padding (GTK_MISC (label), 4, 0);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 2, 3,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 4, 4);

    list = g_list_append(list,__list_data[0].widget);
    list = g_list_append(list,__list_data[1].widget);
    list = g_list_append(list,__list_data[2].widget);
    list = g_list_append(list,__list_data[3].widget);
    __widget_servertype = gtk_list_new ();
    gtk_list_append_items(GTK_LIST(__widget_servertype),list);
    gtk_widget_show (__widget_servertype);
    gtk_table_attach (GTK_TABLE (table), __widget_servertype, 1, 2, 2, 3,
                      (GtkAttachOptions) (GTK_FILL|GTK_EXPAND),
                      (GtkAttachOptions) (GTK_FILL), 4, 4);
    gtk_label_set_mnemonic_widget (GTK_LABEL (label), __list_data[0].widget);
    gtk_tooltips_set_tip (__widget_tooltips, __widget_servertype,
                              _("select the server type from Wnn4 / Wnn6 / Wnn7."), NULL);

    /* default preeditor */
    label = gtk_label_new_with_mnemonic (_("Default_PreEditor: "));
    gtk_widget_show (label);
    gtk_misc_set_alignment (GTK_MISC (label), 1.0, 0.5);
    gtk_misc_set_padding (GTK_MISC (label), 4, 0);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 3, 4,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 4, 4);

    list = NULL;
    list = g_list_append(list,__list_data[4].widget);
    list = g_list_append(list,__list_data[5].widget);
    __widget_preeditor = gtk_list_new ();
    gtk_list_append_items(GTK_LIST(__widget_preeditor),list);
    gtk_widget_show (__widget_preeditor);
    gtk_table_attach (GTK_TABLE (table), __widget_preeditor, 1, 2, 3, 4,
                      (GtkAttachOptions) (GTK_FILL|GTK_EXPAND),
                      (GtkAttachOptions) (GTK_FILL), 4, 4);
    gtk_label_set_mnemonic_widget (GTK_LABEL (label), __list_data[3].widget);
    gtk_tooltips_set_tip (__widget_tooltips, __widget_preeditor,
                              _("select the default preeditor."), NULL);


    /* ALP */
    label = gtk_label_new_with_mnemonic (_("_Auto candidates popup: "));
    gtk_widget_show (label);
    gtk_misc_set_alignment (GTK_MISC (label), 1.0, 0.5);
    gtk_misc_set_padding (GTK_MISC (label), 4, 0);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 4, 5,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 4, 4);

    GtkAdjustment* aj;
    aj = (GtkAdjustment *) gtk_adjustment_new(0,-1,10,1,1,1);
    __widget_alp = gtk_spin_button_new(aj,1,0);
    gtk_widget_show (__widget_alp);
    gtk_table_attach (GTK_TABLE (table), __widget_alp, 1, 2, 4, 5,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 4, 4);
    gtk_label_set_mnemonic_widget (GTK_LABEL (label), __widget_alp);
    gtk_tooltips_set_tip (__widget_tooltips, __widget_alp,
                              _("set the count of conversion until displaying the list of lookup result automatically."), NULL);

    /* numkeyselection */
    label = gtk_label_new_with_mnemonic (_("_NumKey selection: "));
    gtk_widget_show (label);
    gtk_misc_set_alignment (GTK_MISC (label), 1.0, 0.5);
    gtk_misc_set_padding (GTK_MISC (label), 4, 0);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 5, 6,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 4, 4);

    __widget_numkeyselect = gtk_check_button_new();
    gtk_widget_show (__widget_numkeyselect);
    gtk_table_attach (GTK_TABLE (table), __widget_numkeyselect, 1, 2, 5, 6,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 4, 4);
    gtk_label_set_mnemonic_widget (GTK_LABEL (label), __widget_numkeyselect);
    gtk_tooltips_set_tip (__widget_tooltips, __widget_numkeyselect,
                              _("If you use the candidate selection by numkey, check this."), NULL);

    /* auto conversion */
    label = gtk_label_new_with_mnemonic (_("Auto _conversion: "));
    gtk_widget_show (label);
    gtk_misc_set_alignment (GTK_MISC (label), 1.0, 0.5);
    gtk_misc_set_padding (GTK_MISC (label), 4, 0);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 6, 7,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 4, 4);

    __widget_autoconversion = gtk_check_button_new();
    gtk_widget_show (__widget_autoconversion);
    gtk_table_attach (GTK_TABLE (table), __widget_autoconversion, 1, 2, 6, 7,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 4, 4);
    gtk_label_set_mnemonic_widget (GTK_LABEL (label), __widget_autoconversion);
    gtk_tooltips_set_tip (__widget_tooltips, __widget_autoconversion,
                              _("If you set default to the auto conversion mode use, check this."), NULL);


    /* completion */
    label = gtk_label_new_with_mnemonic (_("_YOSOKU-input: "));
    gtk_widget_show (label);
    gtk_misc_set_alignment (GTK_MISC (label), 1.0, 0.5);
    gtk_misc_set_padding (GTK_MISC (label), 4, 0);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 7, 8,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 4, 4);

    __widget_yosoku = gtk_check_button_new();
    gtk_widget_show (__widget_yosoku);
    gtk_table_attach (GTK_TABLE (table), __widget_yosoku, 1, 2, 7, 8,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 4, 4);
    gtk_label_set_mnemonic_widget (GTK_LABEL (label), __widget_yosoku);
    gtk_tooltips_set_tip (__widget_tooltips, __widget_yosoku,
                              _("if you use YOSOKU-input , check this."), NULL);


    // romkan_table
    label = gtk_label_new_with_mnemonic (_("_Roma-Kana table: "));
    gtk_widget_show (label);
    gtk_misc_set_alignment (GTK_MISC (label), 1.0, 0.5);
    gtk_misc_set_padding (GTK_MISC (label), 4, 0);
    gtk_table_attach (GTK_TABLE (table), label, 0, 1, 8, 9,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 4, 4);

    __widget_romkan_table = gtk_entry_new ();
    gtk_widget_show (__widget_romkan_table);
    gtk_table_attach (GTK_TABLE (table), __widget_romkan_table, 1, 2, 8, 9,
                      (GtkAttachOptions) (GTK_FILL|GTK_EXPAND),
                      (GtkAttachOptions) (GTK_FILL), 4, 4);
    gtk_label_set_mnemonic_widget (GTK_LABEL (label), __widget_romkan_table);

    button2 = gtk_button_new_with_label (_("..."));
    gtk_widget_show (button2);
    gtk_table_attach (GTK_TABLE (table), button2, 2, 3, 8, 9,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 4, 4);
    gtk_tooltips_set_tip (__widget_tooltips, __widget_romkan_table,
                              _("input the path of Roma-Kana convert table file."), NULL);



    /* connect all signals */
    g_signal_connect ((gpointer) __widget_server, "changed",
                      G_CALLBACK (on_default_editable_changed),
                      &__config_server);
    g_signal_connect ((gpointer) __widget_wnn_rc, "changed",
                      G_CALLBACK (on_default_editable_changed),
                      &__config_wnn_rc);
    g_signal_connect ((gpointer) button, "clicked",
                      G_CALLBACK (on_default_file_selection_button),
                      __widget_wnn_rc);
    g_signal_connect ((gpointer) __widget_servertype, "select-child",
                      G_CALLBACK (on_default_list_changed),
                      &__config_servertype);
    g_signal_connect ((gpointer) __widget_preeditor, "select-child",
                      G_CALLBACK (on_default_list_changed),
                      &__config_preeditor);
    g_signal_connect ((gpointer) __widget_alp, "changed",
                      G_CALLBACK (on_default_number_changed),
                      &__config_alp);
    g_signal_connect ((gpointer) __widget_numkeyselect, "toggled",
                      G_CALLBACK (on_default_toggle_button_toggled),
                      &__config_numkeyselect);
    g_signal_connect ((gpointer) __widget_autoconversion, "toggled",
                      G_CALLBACK (on_default_toggle_button_toggled),
                      &__config_autoconversion);
    g_signal_connect ((gpointer) __widget_yosoku, "toggled",
                      G_CALLBACK (on_default_toggle_button_toggled),
                      &__config_yosoku);
    g_signal_connect ((gpointer) __widget_romkan_table, "changed",
                      G_CALLBACK (on_default_editable_changed),
                      &__config_romkan_table);
    g_signal_connect ((gpointer) button2, "clicked",
                      G_CALLBACK (on_default_file_selection_button),
                      __widget_romkan_table);

    return vbox;
}

static GtkWidget *
create_keyboard_page (unsigned int page)
{
    GtkWidget *table;
    GtkWidget *label;

    if (page >= __key_conf_pages_num)
        return NULL;

    KeyboardConfigData *data = __key_conf_pages[page].data;

    table = gtk_table_new (3, 3, FALSE);
    gtk_widget_show (table);

    // Create keyboard setting.
    for (unsigned int i = 0; data[i].key; ++ i) {
        label = gtk_label_new (NULL);
        gtk_label_set_text_with_mnemonic (GTK_LABEL (label), _(data[i].label));
        gtk_widget_show (label);
        gtk_misc_set_alignment (GTK_MISC (label), 1.0, 0.5);
        gtk_misc_set_padding (GTK_MISC (label), 4, 0);
        gtk_table_attach (GTK_TABLE (table), label, 0, 1, i, i+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (GTK_FILL), 4, 4);

        data[i].entry = gtk_entry_new ();
        gtk_widget_show (data[i].entry);
        gtk_table_attach (GTK_TABLE (table), data[i].entry, 1, 2, i, i+1,
                          (GtkAttachOptions) (GTK_FILL|GTK_EXPAND),
                          (GtkAttachOptions) (GTK_FILL), 4, 4);
        gtk_entry_set_editable (GTK_ENTRY (data[i].entry), FALSE);

        data[i].button = gtk_button_new_with_label (_("..."));
        gtk_widget_show (data[i].button);
        gtk_table_attach (GTK_TABLE (table), data[i].button, 2, 3, i, i+1,
                          (GtkAttachOptions) (GTK_FILL),
                          (GtkAttachOptions) (GTK_FILL), 4, 4);
        gtk_label_set_mnemonic_widget (GTK_LABEL (label), data[i].button);
    }

    for (unsigned int i = 0; data[i].key; ++ i) {
        g_signal_connect ((gpointer) data[i].button, "clicked",
                          G_CALLBACK (on_default_key_selection_clicked),
                          &(data[i]));
        g_signal_connect ((gpointer) data[i].entry, "changed",
                          G_CALLBACK (on_default_editable_changed),
                          &(data[i].data));
    }

    if (!__widget_tooltips)
        __widget_tooltips = gtk_tooltips_new();
    for (unsigned int i = 0; data[i].key; ++ i) {
        gtk_tooltips_set_tip (__widget_tooltips, data[i].entry,
                              _(data[i].tooltip), NULL);
    }

    return table;
}

static GtkWidget *
create_setup_window ()
{
    static GtkWidget *window = NULL;

    if (!window) {
        GtkWidget *notebook = gtk_notebook_new();
        gtk_widget_show (notebook);
        window = notebook;
        gtk_notebook_set_scrollable (GTK_NOTEBOOK (notebook), TRUE);

        // Create the first page.
        GtkWidget *page = create_options_page ();
        GtkWidget *label = gtk_label_new (_("Options"));
        gtk_widget_show (label);
        gtk_notebook_append_page (GTK_NOTEBOOK (notebook), page, label);

        setup_widget_value ();

        // Create the second page.
        for (unsigned int i = 0; i < __key_conf_pages_num; i++) {
            page = create_keyboard_page (i);
            label = gtk_label_new (_(__key_conf_pages[i].label));
            gtk_widget_show (label);
            gtk_notebook_append_page (GTK_NOTEBOOK (notebook), page, label);
        }

        setup_widget_value ();
    }

    return window;
}

static void
setup_widget_value (void)
{
//    if (__widget_use_kana) {
//        gtk_toggle_button_set_active (
//            GTK_TOGGLE_BUTTON (__widget_use_kana),
//            __config_use_kana);
//    }
    if (__widget_server) {
        gtk_entry_set_text (
            GTK_ENTRY (__widget_server),
            __config_server.c_str ());
    }

    if (__widget_wnn_rc) {
        gtk_entry_set_text (
            GTK_ENTRY (__widget_wnn_rc),
            __config_wnn_rc.c_str ());
    }

    if (__widget_servertype) {
        for(unsigned int i = 0;__list_data[i].widget;i ++) {
            if (__list_data[i].name == __config_servertype) {
                gtk_list_select_child(GTK_LIST(__widget_servertype),__list_data[i].widget);
                break;
            }
        }
    }

    if (__widget_preeditor) {
        for(unsigned int i = 0;__list_data[i].widget;i ++) {
            if (__list_data[i].name == __config_preeditor) {
                gtk_list_select_child(GTK_LIST(__widget_preeditor),__list_data[i].widget);
                break;
            }
        }
    }

    if (__widget_alp) {
        gtk_spin_button_set_value (
            GTK_SPIN_BUTTON (__widget_alp),
            __config_alp);
    }

    if (__widget_numkeyselect) {
        gtk_toggle_button_set_active (
            GTK_TOGGLE_BUTTON (__widget_numkeyselect),
            __config_numkeyselect);
    }

    if (__widget_autoconversion) {
        gtk_toggle_button_set_active (
            GTK_TOGGLE_BUTTON (__widget_autoconversion),
            __config_autoconversion);
    }

    if (__widget_yosoku) {
        gtk_toggle_button_set_active (
            GTK_TOGGLE_BUTTON (__widget_yosoku),
            __config_yosoku);
    }

    if (__widget_romkan_table) {
        gtk_entry_set_text (
            GTK_ENTRY (__widget_romkan_table),
            __config_romkan_table.c_str ());
    }

    for (unsigned int j = 0; j < __key_conf_pages_num; ++ j) {
        for (unsigned int i = 0; __key_conf_pages[j].data[i].key; ++ i) {
            if (__key_conf_pages[j].data[i].entry) {
                gtk_entry_set_text (
                    GTK_ENTRY (__key_conf_pages[j].data[i].entry),
                    __key_conf_pages[j].data[i].data.c_str ());
            }
        }
    }
}

static void
load_config (const ConfigPointer &config)
{
    if (!config.null ()) {
    //    __config_use_kana =
    //        config->read (String (SCIM_CONFIG_IMENGINE_ANTHY_USE_KANA),
    //                      __config_use_kana);
        __config_server =
            config->read (String (SCIM_CONFIG_WNN_SERVER),
                          __config_server);
        __config_wnn_rc =
            config->read (String (SCIM_CONFIG_WNN_RC),
                          __config_wnn_rc);
        __config_servertype =
            config->read (String (SCIM_CONFIG_WNN_SERVERTYPE),
                          __config_servertype);
        __config_preeditor =
            config->read (String (SCIM_CONFIG_WNN_DEFAULT_PREEDITOR),
                          __config_preeditor);
        __config_alp =
            config->read (String (SCIM_CONFIG_WNN_ALP),
                          __config_alp);
        __config_numkeyselect =
            config->read (String (SCIM_CONFIG_WNN_NUMKEY_SELECT),
                          __config_numkeyselect);
        __config_autoconversion =
            config->read (String (SCIM_CONFIG_WNN_AUTO_CONVERSION),
                          __config_autoconversion);
        __config_yosoku =
            config->read (String (SCIM_CONFIG_WNN_YOSOKU),
                          __config_yosoku);
        __config_romkan_table =
            config->read (String (SCIM_CONFIG_WNN_ROMKAN_TABLE_FILE),
                          __config_romkan_table);

        for (unsigned int j = 0; j < __key_conf_pages_num; j++) {
            for (unsigned int i = 0; __key_conf_pages[j].data[i].key; ++ i) {
                __key_conf_pages[j].data[i].data =
                    config->read (String (__key_conf_pages[j].data[i].key),
                                  __key_conf_pages[j].data[i].data);
            }
        }

        setup_widget_value ();

        __have_changed = false;
    }
}

static void
save_config (const ConfigPointer &config)
{
    if (!config.null ()) {
    //    config->write (String (SCIM_CONFIG_IMENGINE_ANTHY_USE_KANA),
    //                    __config_use_kana);
        config->write (String (SCIM_CONFIG_WNN_SERVER),
                       __config_server);
        config->write (String (SCIM_CONFIG_WNN_RC),
                       __config_wnn_rc);
        config->write (String (SCIM_CONFIG_WNN_SERVERTYPE),
                       __config_servertype);
        config->write (String (SCIM_CONFIG_WNN_DEFAULT_PREEDITOR),
                       __config_preeditor);
        config->write (String (SCIM_CONFIG_WNN_ALP),
                       __config_alp);
        config->write (String (SCIM_CONFIG_WNN_NUMKEY_SELECT),
                       __config_numkeyselect);
        config->write (String (SCIM_CONFIG_WNN_AUTO_CONVERSION),
                       __config_autoconversion);
        config->write (String (SCIM_CONFIG_WNN_YOSOKU),
                       __config_yosoku);
        config->write (String (SCIM_CONFIG_WNN_ROMKAN_TABLE_FILE),
                       __config_romkan_table);

        for (unsigned j = 0; j < __key_conf_pages_num; j++) {
            for (unsigned int i = 0; __key_conf_pages[j].data[i].key; ++ i) {
                config->write (String (__key_conf_pages[j].data[i].key),
                               __key_conf_pages[j].data[i].data);
            }
        }

        __have_changed = false;
    }
}

static bool
query_changed ()
{
    return __have_changed;
}


static void
on_default_toggle_button_toggled (GtkToggleButton *togglebutton,
                                  gpointer         user_data)
{
    bool *toggle = static_cast<bool*> (user_data);

    if (toggle) {
        *toggle = gtk_toggle_button_get_active (togglebutton);
        __have_changed = true;
    }
}

static void
on_default_editable_changed (GtkEditable *editable,
                             gpointer     user_data)
{
    String *str = static_cast <String *> (user_data);

    if (str) {
        *str = String (gtk_entry_get_text (GTK_ENTRY (editable)));
        __have_changed = true;
    }
}

static void
on_default_number_changed (GtkEditable *editable,
                             gpointer     user_data)
{
    int *num = static_cast <int *> (user_data);

    if (num) {
        *num = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (editable));
        __have_changed = true;
    }
}

static void
on_default_list_changed (GtkList *list,
                         GtkWidget *widget,
                         gpointer     user_data)
{
    String *str = static_cast <String *> (user_data);

    if (str) {
        for(unsigned int i = 0;__list_data[i].widget;i ++) {
            if (__list_data[i].widget == widget)
                *str = __list_data[i].name;
        }
        __have_changed = true;
    }
}

static void
on_default_key_selection_clicked (GtkButton *button,
                                  gpointer   user_data)
{
    KeyboardConfigData *data = static_cast <KeyboardConfigData *> (user_data);

    if (data) {
        GtkWidget *dialog = scim_key_selection_dialog_new (_(data->title));
        gint result;

        scim_key_selection_dialog_set_keys (
            SCIM_KEY_SELECTION_DIALOG (dialog),
            gtk_entry_get_text (GTK_ENTRY (data->entry)));

        result = gtk_dialog_run (GTK_DIALOG (dialog));

        if (result == GTK_RESPONSE_OK) {
            const gchar *keys = scim_key_selection_dialog_get_keys (
                            SCIM_KEY_SELECTION_DIALOG (dialog));

            if (!keys) keys = "";

            if (strcmp (keys, gtk_entry_get_text (GTK_ENTRY (data->entry))) != 0)
                gtk_entry_set_text (GTK_ENTRY (data->entry), keys);
        }

        gtk_widget_destroy (dialog);
    }
}

static void
on_ok_clicked (GtkButton *button, gpointer user_data)
{
    bool *ok = static_cast<bool*>(user_data);
    if (ok)
        *ok = true;
}

static void
on_default_file_selection_button (GtkButton *button, gpointer user_data)
{
    GtkEntry *entry = GTK_ENTRY (user_data);
    const char *path = gtk_entry_get_text (entry);
    bool ok = false;

    GtkFileSelection *sel = GTK_FILE_SELECTION (gtk_file_selection_new ("Select a file"));
    gtk_file_selection_set_filename (sel, path);
    g_signal_connect (G_OBJECT (sel->ok_button), "clicked",
                      G_CALLBACK (on_ok_clicked),
                      (gpointer) &ok);
    gtk_window_set_transient_for (
        GTK_WINDOW (sel),
        GTK_WINDOW (gtk_widget_get_toplevel(GTK_WIDGET (entry))));
    gtk_dialog_run (GTK_DIALOG (sel));

    if (ok)
        gtk_entry_set_text (entry, gtk_file_selection_get_filename (sel));

    gtk_widget_destroy (GTK_WIDGET (sel));
}
