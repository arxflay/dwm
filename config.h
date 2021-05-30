/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 33;
static const char *fonts[]          = {"Iosevka:size=13", "noto-color-emoji:size=13"};
static const char dmenufont[]       = "Iosevka:size=13";
static const char col_barbg[]       = "#2C2F3A";
static const long col_selIndicator = 0xCC241D;
static const char col_normborder[]       = "#444444";
static const char col_norm[]       = "#dddddd";
static const char col_sel[]       = "#e5d39b"; //color of selected elements like tags or text
static const char col_selborder[]        = "#2a2d38";
static const char col_seldmenu[] = "#1E1E1E";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_norm, col_barbg, col_normborder},
	[SchemeSel]  = { col_sel, col_barbg,  col_selborder},
};
/*commands*/
static const char *upvol[] = { "amixer", "-D", "pulse", "sset", "Master", "2%+", NULL };
static const char *downvol[] = { "amixer", "-D", "pulse", "sset", "Master", "2%-", NULL };
static const char *mute[] = { "amixer", "-q", "-D", "pulse", "sset", "Master", "toggle", NULL};
static const char *upblight[] = {"brightnessctl", "-d", "intel_backlight", "set", "+20%", NULL};
static const char *downblight[] = {"brightnessctl", "-d", "intel_backlight" , "set", "20-%", NULL};

/* tagging */
static const char *tags[] = { "❶", "❷", "❸", "❹"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            0,           -1 },
	{ "Steam",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       0,	    0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "| 🆃 |",      tile },    /* first entry is default */
	{ "| 🅵 |",      NULL },    /* no layout function means floating behavior */
	{ "| 🅼 |",      monocle },
};
/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_barbg, "-nf", col_norm, "-sb", col_seldmenu , "-sf", col_sel, NULL};
static const char *termcmd[]  = { "tilix", NULL };
static const char *kblayoutswitcher[] = { "/usr/share/dwm_modules/switch_klayout.sh", NULL };
static const char *screenshotapp[] = { "spectacle", NULL};
static const char *audiocontrol[] = { "pavucontrol", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
        { MODKEY,                       XK_Shift_L, spawn,         {.v = kblayoutswitcher}},
        { 0,                            XK_Print,   spawn,         {.v = screenshotapp}},
        { 0,                            XF86XK_AudioRaiseVolume,                spawn,          {.v=upvol}},
        { 0,                            XF86XK_AudioLowerVolume,                spawn,          {.v=downvol}},
        { 0,                            XF86XK_AudioMute,                       spawn,          {.v=mute}},
        { 0,                            XF86XK_MonBrightnessUp,                 spawn,          {.v=upblight}},
        { 0,                            XF86XK_MonBrightnessDown,                       spawn,          {.v=downblight}},
	{ MODKEY,                       XK_5, spawn,         {.v = audiocontrol}},
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

