
#ifndef ENGINESTRUCTS_H
#define ENGINESTRUCTS_H

#include <extdll.h>
#include <meta_api.h>
#include "osdep.h"
#include "entity_state.h"

typedef struct sizebuf_s 
{
	char*			debugname;
	int				overflow; 
	byte*			data;
	unsigned int	maxsize;
	unsigned int	cursize;
} sizebuf_t;

typedef struct usercmd_s
{
	short			lerp_msec;
	byte			msec;
	vec3_t			viewangles;
	float			forwardmove;
	float			sidemove;
	float			upmove;
	byte			lightlevel;
	unsigned short	buttons;
	byte			impulse;
	byte			weaponselect;
	int				impact_index;
	vec3_t			impact_position;
} usercmd_t;

typedef struct packet_entities_s
{
	int				num_entities;
	int				max_entities;
	entity_state_t	entities[64];
} packet_entities_t;

typedef struct client_frame_s
{
	double				senttime;
	float				ping_time;
	clientdata_t		*cdata;
	float				frame_time;
	packet_entities_t	entities;
} client_frame_t;

typedef struct event_args_s
{
	int		flags;
	int		entindex;
	float	origin[3];
	float	angles[3];
	float	velocity[3];
	int		ducking;
	float	fparam1;
	float	fparam2;
	int		iparam1;
	int		iparam2;
	int		bparam1;
	int		bparam2;
} event_args_t;

typedef struct event_info_s
{
	unsigned short	index;
	short			packet_index;
	short			entity_index;
	float			fire_time;
	event_args_t	args;
	int				flags;
} event_info_t;

enum netadrtype_t
{
	NA_UNUSED		= 0x0,
	NA_LOOPBACK		= 0x1,
	NA_BROADCAST	= 0x2,
	NA_IP			= 0x3,
	NA_IPX			= 0x4,
	NA_BROADCAST_IPX = 0x5,
};

typedef struct netadr_s 
{
	netadrtype_t	type;
	byte			ip[4];
	byte 			ipx[10]; 
	unsigned short	port;
} netadr_t;

enum netsrc_t
{
	NS_CLIENT = 0x0,
	NS_SERVER = 0x1,
};

typedef struct fragbuf_s
{
	struct fragbuf_s	*next;
	int					bufferid;
	sizebuf_t			frag_message;
	byte				frag_message_buf[1400];
	qboolean			isfile;
	qboolean			isbuffer;
	char				filename[64];
	int					foffset;
	int					size;
} fragbuf_t;

typedef struct fragbufwaiting_s
{
	struct fragbufwaiting_s *next;
	int						fragbufcount;
	fragbuf_t *				fragbufs;
} fragbufwaiting_t;

typedef struct flowstats_s
{
	int		size;
	double	time;
} flowstats_t;

typedef struct flow_s
{
	flowstats_t stats[32];
	int			current;
	double		nextcompute;
	float		kbytespersec;
	float		avgkbytespersec;
	int			totalbytes;
} flow_t;

typedef struct client_s client_t;

typedef struct netchan_s
{
	netsrc_t			sock;
	netadr_t			remote_address;
	int					client_index;
	float				last_received;
	float				connect_time;
	double				rate;
	double				cleartime;
	int					incoming_sequence;
	int					incoming_acknowledged;
	int					incoming_reliable_acknowledged;
	int					incoming_reliable_sequence;
	int					outgoing_sequence;
	int					reliable_sequence;
	int					last_reliable_sequence;
	client_t*			netchan_client_TO_CHECK;
	int					spawned;
	sizebuf_t			message;
	char				message_buf[3992];
	int					reliable_length;
	char				reliable_buf[3992];
	fragbufwaiting_t *	waitlist[2];
	int					reliable_fragment[2];
	unsigned int		reliable_fragid[2];
	fragbuf_t *			fragbufs[2];
	int					fragbufcount[2];
	short				frag_startpos[2];
	short				frag_length[2];
	fragbuf_t			*incomingbufs[2];
	qboolean			incomingready[2];
	char				incomingfilename[64];
	flow_t				flow[2];
	int					chanpayload[48];
	int					tail;
} netchan_t;

struct client_s
{
	int				active;
	int				spawned;
	int				lastmsg;
	int				connected;
	int				resources;
	int				ready;
	int				consistency;
	netchan_t		netchan;
	int				chokecount;
	int				delta_sequence;
	int				fakeclient;
	int				ishltv;
	usercmd_t		lastcmd;
	double			localtime;
	double			delay;
	double			nexttimeout;
	signed	int		ping;
	signed	int		packetloss;
	double			unk_0;
	double			nextping;
	double			lastcmdtime;
	sizebuf_t		datagram;
	char			datagram_buffer[4000];
	double			connection_started;
	double			next_messagetime;
	double			next_messageinterval;
	int				send_message;
	int				drop;
	client_frame_t*	frames;	//
	event_info_t	events[64];	// 
	edict_t*		edict;
	const edict_t *	target_edict;
	int				userid;
	int				steamid0;
	int				steamid1;
	int				steamid2;
	int				ipaddress;
	char			setinfo[256];
	int				sendinfo;
	int				unk_1;
	char			hashedcdkey[64];
	char			playername[32];
	int				topcolor;
	int				bottomcolor;
	int				unk_2;
	resource_t		player_customization;
	resource_t		resource_list;
	customization_t customization_list;
	int				unk_3;
	customization_t *customization_ptr;
	int				cl_lw;
	int				cl_lc;
	int				payload[65];
	int				m_bLoopback;
	unsigned long  m_VoiceStreams[2];
};

typedef struct server_s 
{
	#if defined __linux__
		char	padding[ 0x140 ];
	#else
		char	padding[ 0x148 ];
	#endif
	resource_t	consistencyData[ 1280 ]; 
	uint32		consistencyDataCount; 
	// [...]
} server_t;

#endif // ENGINESTRUCTS_H