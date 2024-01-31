#ifndef __PLAYVNP_PROTOCOL__H__
#define __PLAYVNP_PROTOCOL__H__

//--------------------------------------------------------------------------------
/// Protocol commands for PlayVnP
#define CMD_PLAY_PIC          'I'
#define CMD_STOP_PIC          'i'

#define CMD_PLAY_VIDEO        'V'
#define CMD_STOP_VIDEO        'v'
#define CMD_PAUSE_VIDEO       'P'

#define CMD_RUN_SLIDESHOW     'S'
#define CMD_STOP_SLIDESHOW    's'
#define CMD_PREVIEW           'p'
#define CMD_NEXT_SLIDE        'n'
#define CMD_PREV_SLIDE        'r'
#define CMD_GOTO_SLIDE        'g'
#define CMD_BLACK_SLIDE       'b'
#define CMD_WHITE_SLIDE       'w'
#define CMD_NEXT_LINK         'L'
#define CMD_PREV_LINK         'l'
#define CMD_ACTIVATE_LINK     'a'
#define CMD_FILEINFO          'F'

#define CMD_PING              '*'


#define RESP_PIC_OK           '+'
#define RESP_PIC_ERROR        '-'

#define RESP_VIDEO_OK         '+'
#define RESP_VIDEO_ERROR      '-'

#define RESP_PPT_OK           '+'
#define RESP_PPT_ERROR        '-'

#define RESP_FILEINFO         'F'
#define RESP_SLIDE_COUNT      'C'
#define RESP_SLIDE_INFO       'I'
#define RESP_SLIDE_PREVIEW    'P'

#define RESP_ERROR            '#'

#define RESP_UNKNOWN_CMD      '?'



//--------------------------------------------------------------------------------
// Commands for communication protocol with ModelLoader-Application
#define CMD_GET_MODELS        "!objlist\n"


#endif  // __PLAYVNP_PROTOCOL__H__