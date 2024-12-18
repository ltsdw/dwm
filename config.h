/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 2;       /* snap pixel */
static const unsigned int gappih    = 2;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 2;       /* vert inner gap between windows */
static const unsigned int gappoh    = 2;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 2;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int vertpad            = 2;       /* vertical padding of bar */
static const int sidepad            = 2;       /* horizontal padding of bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "Web", "Dev", "Games", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class         instance    title       tags mask     isfloating   monitor */
	{ "Gimp",       NULL,       NULL,       1 << 5,       1,           -1 },
	{ "LibreWolf",  NULL,       NULL,       1 << 4,       0,           -1 },
	{ "kitty",      NULL,       NULL,       1 << 0,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "D[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };

static Key keys[] = {
	/* modifier                     key                         function            argument */
	{ MODKEY,                       XK_d,                       spawn,              {.v = dmenucmd } },
	{ MODKEY,                       XK_t,                       spawn,              SHCMD("${TERMINAL}") },
	{ MODKEY,                       XK_b,                       togglebar,          {0} },
	{ MODKEY,                       XK_j,                       focusstack,         {.i = -1 } },
	{ MODKEY,                       XK_k,                       focusstack,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_h,                       incnmaster,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_l,                       incnmaster,         {.i = +1 } },
	{ MODKEY|ControlMask,           XK_h,                       setmfact,           {.f = -0.05} },
	{ MODKEY|ControlMask,           XK_l,                       setmfact,           {.f = +0.05} },
	{ MODKEY,                       XK_Return,                  zoom,               {0} },
	{ MODKEY,                       XK_Tab,                     view,               {0} },
	{ MODKEY,                       XK_c,                       killclient,         {0} },
	{ MODKEY,                       XK_t,                       setlayout,          {.v = &layouts[0]} },
	{ MODKEY,                       XK_e,                       setlayout,          {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,                       setlayout,          {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,                   setlayout,          {0} },
	{ MODKEY|ShiftMask,             XK_space,                   togglefloating,     {0} },
	{ MODKEY,                       XK_f,                       togglefullscreen,   {0} },
	{ MODKEY,                       XK_0,                       view,               {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                       tag,                {.ui = ~0 } },
	{ MODKEY,                       XK_j,                       focusmon,           {.i = -1 } },
	{ MODKEY,                       XK_k,                       focusmon,           {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                   tagmon,             {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,                  tagmon,             {.i = +1 } },
	{ 0,                            XF86XK_MonBrightnessUp,     spawn,              SHCMD("light -A 5") },
	{ 0,                            XF86XK_MonBrightnessDown,   spawn,              SHCMD("light -U 5") },
	{ MODKEY|ShiftMask,             XK_Escape,                  quit,               {0} },

	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_q,                      4)
	TAGKEYS(                        XK_r,                      5)
	TAGKEYS(                        XK_s,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

	{ 0,             XK_Print, spawn,                   SHCMD("maim $XDG_PICTURES_DIR/screenshot-$(date +%F-%T).png") },
	{ ControlMask,   XK_Print, spawn,                   SHCMD("maim -s $XDG_PICTURES_DIR/screenshot-$(date +%F-%T).png") },
	{ 0,             XF86XK_AudioLowerVolume, spawn,    SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5%") },
	{ 0,             XF86XK_AudioLowerVolume, spawn,    SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5%") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
    {}
};
/* See LICENSE file for copyright and license details. */
