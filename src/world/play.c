/* player lists */

#include <stdio.h>
#include <stdlib.h>

#include "world.h"
#include "externs.h"

extern int bpipe;

static struct rplist *rpmlist=NULL;

/* Send server information to other servers */
void send_sinfo(struct client *ccl, struct client *priv){
	struct wpacket spk;
	spk.type=WP_SINFO;
	spk.serverid=0;
	strncpy(spk.d.sinfo.name, slist[ccl->authed-1].name, 30);
	spk.d.sinfo.sid=ccl->authed;
	if(priv)
		reply(&spk, priv);
	else
		relay(&spk, ccl);
}

/*
 * This function is called when a new server starts
 * and needs to be given the current state of the
 * world.
 */
void send_rplay(struct client *ccl){
	struct rplist *c_pl;
	struct client *c_cl;
	struct wpacket spk;

	/* Send online server info */
	c_cl=clist;
	while(c_cl){
		if(c_cl==ccl) continue;
		if(c_cl->authed<=0) continue;
		send_sinfo(c_cl, ccl);
		c_cl=c_cl->next;
	}

	/* Send online player login packets */
	c_pl=rpmlist;
	spk.type=WP_NPLAYER;
	spk.serverid=0;
	spk.d.play.silent=1;
	while(c_pl){
		spk.d.play.id=c_pl->id;
		spk.d.play.server=c_pl->server;
		strncpy(spk.d.play.name, c_pl->name, 30);
		reply(&spk, ccl);
		/* Temporary stderr output */
		if(bpipe){
			fprintf(stderr, "SIGPIPE from send_rplay (fd: %d)\n", ccl->fd);
			bpipe=0;
		}
		c_pl=c_pl->next;
	}
}

void rem_players(short id){
	struct rplist *c_pl, *p_pl, *d_pl;
	c_pl=rpmlist;
	p_pl=c_pl;
	while(c_pl){
		d_pl=NULL;
		if(c_pl->server==id){
			if(c_pl==rpmlist)
				rpmlist=c_pl->next;
			else
				p_pl->next=c_pl->next;
			d_pl=c_pl;
		}
		p_pl=c_pl;
		c_pl=c_pl->next;
		if(d_pl) free(d_pl);
	}
}

void add_rplayer(struct wpacket *wpk){
	struct rplist *n_pl, *c_pl;
	unsigned short found=0;

	if(wpk->type==WP_NPLAYER && !wpk->d.play.server) return;
	c_pl=rpmlist;
	while(c_pl){
		if(/* c_pl->id==wpk->d.play.id && */ !(strcmp(c_pl->name, wpk->d.play.name))){
			found=1;
			break;
		}
		n_pl=c_pl;
		c_pl=c_pl->next;
	}
	if(wpk->type==WP_NPLAYER && !found){
		n_pl=malloc(sizeof(struct rplist));
		n_pl->next=rpmlist;
		n_pl->id=wpk->d.play.id;
		n_pl->server=wpk->d.play.server;
		strncpy(n_pl->name, wpk->d.play.name, 30);
		rpmlist=n_pl;
	}
	else if(wpk->type==WP_QPLAYER && found){
		if(c_pl==rpmlist)
			rpmlist=c_pl->next;
		else
			n_pl->next=c_pl->next;
		free(c_pl);
	}
}
