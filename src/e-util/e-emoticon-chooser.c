/*
 * e-emoticon-chooser.c
 *
 * Copyright (C) 2008 Novell, Inc.
 * Copyright (C) 2012 Dan VrΓ‘til <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU Lesser General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "evolution-config.h"

#include "e-emoticon-chooser.h"

#include <glib/gi18n-lib.h>

/* Constant version of EEMoticon. */
typedef struct {
	const gchar *label;
	const gchar *icon_name;
	const gchar *unicode_character;
	const gchar *text_face;
} ConstantEmoticon;

static ConstantEmoticon available_emoticons[] = {
	/* Translators: :-) */
	{ N_("_Smile"),		"face-smile",		"π",	":-)"	},
	/* Translators: :-( */
	{ N_("S_ad"),		"face-sad",		"π",	":-("	},
	/* Translators: ;-) */
	{ N_("_Wink"),		"face-wink",		"π",	";-)"	},
	/* Translators: :-P */
	{ N_("Ton_gue"),	"face-raspberry",	"π",	":-P"	},
	/* Translators: :-)) */
	{ N_("Laug_h"),		"face-laugh",		"π",	":-D"	},
	/* Translators: :-| */
	{ N_("_Plain"),		"face-plain",		"π",	":-|"	},
	/* Translators: :-! */
	{ N_("Smi_rk"),		"face-smirk",		"π",	":-!"	},
	/* Translators: :"-) */
	{ N_("_Embarrassed"),	"face-embarrassed",	"π―",	":\"-)"	},
	/* Translators: :-D */
	{ N_("_Big Smile"),	"face-smile-big",	"π",	":-D"	},
	/* Translators: :-/ */
	{ N_("Uncer_tain"),	"face-uncertain",	"π",	":-/"	},
	/* Translators: :-O */
	{ N_("S_urprise"),	"face-surprise",	"π²",	":-O"	},
	/* Translators: :-S */
	{ N_("W_orried"),	"face-worried",		"π",	":-S"	},
	/* Translators: :-* */
	{ N_("_Kiss"),		"face-kiss",		"π",	":-*"	},
	/* Translators: X-( */
	{ N_("A_ngry"),		"face-angry",		"π ",	"X-("	},
	/* Translators: B-) */
	{ N_("_Cool"),		"face-cool",		"π",	"B-)"	},
	/* Translators: O:-) */
	{ N_("Ange_l"),		"face-angel",		"π",	"O:-)"	},
	/* Translators: :'( */
	{ N_("Cr_ying"),	"face-crying",		"π’",	":'("	},
	/* Translators: :-Q */
	{ N_("S_ick"),		"face-sick",		"π¨",	":-Q"	},
	/* Translators: |-) */
	{ N_("Tire_d"),		"face-tired",		"π«",	"|-)"	},
	/* Translators: >:-) */
	{ N_("De_vilish"),	"face-devilish",	"π",	">:-)"	},
	/* Translators: :-(|) */
	{ N_("_Monkey"),	"face-monkey",		"π΅",	":-(|)"	}
};

enum {
	ITEM_ACTIVATED,
	LAST_SIGNAL
};

static guint signals[LAST_SIGNAL];

G_DEFINE_INTERFACE (
	EEmoticonChooser,
	e_emoticon_chooser,
	G_TYPE_OBJECT)

static void
e_emoticon_chooser_default_init (EEmoticonChooserInterface *interface)
{
	g_object_interface_install_property (
		interface,
		g_param_spec_boxed (
			"current-emoticon",
			"Current Emoticon",
			"Currently selected emoticon",
			E_TYPE_EMOTICON,
			G_PARAM_READWRITE));

	signals[ITEM_ACTIVATED] = g_signal_new (
		"item-activated",
		G_TYPE_FROM_INTERFACE (interface),
		G_SIGNAL_RUN_LAST,
		G_STRUCT_OFFSET (EEmoticonChooserInterface, item_activated),
		NULL, NULL,
		g_cclosure_marshal_VOID__VOID,
		G_TYPE_NONE, 0);
}

EEmoticon *
e_emoticon_chooser_get_current_emoticon (EEmoticonChooser *chooser)
{
	EEmoticonChooserInterface *interface;

	g_return_val_if_fail (E_IS_EMOTICON_CHOOSER (chooser), NULL);

	interface = E_EMOTICON_CHOOSER_GET_INTERFACE (chooser);
	g_return_val_if_fail (interface->get_current_emoticon != NULL, NULL);

	return interface->get_current_emoticon (chooser);
}

void
e_emoticon_chooser_set_current_emoticon (EEmoticonChooser *chooser,
                                         EEmoticon *emoticon)
{
	EEmoticonChooserInterface *interface;

	g_return_if_fail (E_IS_EMOTICON_CHOOSER (chooser));

	interface = E_EMOTICON_CHOOSER_GET_INTERFACE (chooser);
	g_return_if_fail (interface->set_current_emoticon != NULL);

	interface->set_current_emoticon (chooser, emoticon);
}

void
e_emoticon_chooser_item_activated (EEmoticonChooser *chooser)
{
	g_return_if_fail (E_IS_EMOTICON_CHOOSER (chooser));

	g_signal_emit (chooser, signals[ITEM_ACTIVATED], 0);
}

GList *
e_emoticon_chooser_get_items (void)
{
	GList *list = NULL;
	gint ii;

	for (ii = 0; ii < G_N_ELEMENTS (available_emoticons); ii++)
		list = g_list_prepend (list, &available_emoticons[ii]);

	return g_list_reverse (list);
}

const EEmoticon *
e_emoticon_chooser_lookup_emoticon (const gchar *icon_name)
{
	gint ii;

	g_return_val_if_fail (icon_name && *icon_name, NULL);

	for (ii = 0; ii < G_N_ELEMENTS (available_emoticons); ii++) {
		if (strcmp (available_emoticons[ii].icon_name, icon_name) == 0) {
			return (const EEmoticon *) &available_emoticons[ii];
		}
	}

	return NULL;
}

