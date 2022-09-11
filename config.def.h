/* See LICENSE file for copyright and license details. */

/* support for media keys */
#include <X11/XF86keysym.h>
#define XK_LVol XF86XK_AudioLowerVolume
#define XK_MVol XF86XK_AudioMute
#define XK_RVol XF86XK_AudioRaiseVolume
#define XK_APlay XF86XK_AudioPlay
#define XK_AStop XF86XK_AudioStop
#define XK_APrev XF86XK_AudioPrev
#define XK_ANext XF86XK_AudioNext
#define XK_LBri XF86XK_MonBrightnessDown
#define XK_RBri XF86XK_MonBrightnessUp

/* rules and classes */
#define TERMINAL "alacritty"
#define TERMINAL_CLASS "Alacritty"
#define BROWSER "firefox"
#define BROWSER_CLASS "firefox"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const int user_bh            = 24;       /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const char *fonts[]          = { "UbuntuMono Nerd Font:size=11" };
static const char dmenufont[]       = "monospace:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class           instance  title   tags mask  isfloating  isterminal  noswallow  monitor */
	{ BROWSER_CLASS,   NULL,     NULL,   1 << 1,    0,          0,           0,        -1 },
	{ "zoom",          NULL,     NULL,   1 << 2,    0,          0,           0,        -1 },
	{ TERMINAL_CLASS,  NULL,     NULL,   0,         0,          1,           0,        -1 },
	{ "discord",       NULL,     NULL,   1 << 8,    0,          0,           0,        -1 },
};

/* layout(s) */
static const float mfact     = 0.55;    /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;       /* number of clients in master area */
static const int resizehints = 0;       /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1;    /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
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

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_w,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_n,      zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY,                       XK_F5,     xrdb,           {.v = NULL } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_r,      quit,           {0} },

	/* custom keybinds */
	{ MODKEY,                       XK_e,      spawn,          SHCMD("$FILE") },
	{ MODKEY,                       XK_b,      spawn,          SHCMD("$BROWSER") },
	{ MODKEY,                       XK_c,      spawn,          SHCMD("$TERMINAL -e calcurse") },
	{ MODKEY,                       XK_Escape, spawn,          SHCMD("power") },
	{ MODKEY,                       XK_p,      spawn,          SHCMD("displays") },
	{ MODKEY,                       XK_x,      spawn,          SHCMD("slock") },
	{ MODKEY,                       XK_m,      spawn,          SHCMD("music") },
	{ 0,                            XK_APlay,  spawn,          SHCMD("mpc toggle") },
	{ 0,                            XK_AStop,  spawn,          SHCMD("mpc stop") },
	{ 0,                            XK_APrev,  spawn,          SHCMD("mpc prev") },
	{ 0,                            XK_ANext,  spawn,          SHCMD("mpc next") },
	{ ShiftMask,                    XK_LVol,   spawn,          SHCMD("mpc volume -5") },
	{ ShiftMask,                    XK_RVol,   spawn,          SHCMD("mpc volume +5") },
	{ 0,                            XK_Print,  spawn,          SHCMD("screen-save") },
	{ ShiftMask,                    XK_Print,  spawn,          SHCMD("screen-clip") },
	{ 0,                            XK_LVol,   spawn,          SHCMD("volctl -5") },
	{ 0,                            XK_MVol,   spawn,          SHCMD("volctl mute") },
	{ 0,                            XK_RVol,   spawn,          SHCMD("volctl +5") },
	{ 0,                            XK_LBri,   spawn,          SHCMD("brictl -10") },
	{ 0,                            XK_RBri,   spawn,          SHCMD("brictl +10") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigstatusbar,   {.i = 5} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

