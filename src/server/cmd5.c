/* File: cmd5.c */

/* Purpose: Spell/Prayer commands */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#define SERVER

#include "angband.h"



/*
 * XXX XXX XXX XXX
 */
extern void display_spell_list(void);

/*
 * Hack -- Display all known spells in a window
 *
 * XXX XXX XXX Need to analyze size of the window.
 *
 * XXX XXX XXX Need more color coding.
 *
 * I dunno about this -- I've never found it very useful, so it'll be
 * gone from MAngband (for a while, at least).  I'll eventually just transmit
 * all of this information to client and let it handle it if the user wants
 * to have this in a window --KLJ--
 */
void display_spell_list(void)
{
}



/*
 * Returns spell chance of failure for spell		-RAK-
 */
static s16b spell_chance(int Ind, magic_type *s_ptr)
{
	player_type *p_ptr = Players[Ind];

	int		chance, minfail;

	/* Extract the base spell failure rate */
	chance = s_ptr->sfail;

	/* Reduce failure rate by "effective" level adjustment */
	chance -= 3 * (p_ptr->lev - s_ptr->slevel);

	/* Reduce failure rate by INT/WIS adjustment */
	chance -= 3 * (adj_mag_stat[p_ptr->stat_ind[p_ptr->mp_ptr->spell_stat]] - 1);

	/* Not enough mana to cast */
	if (s_ptr->smana > p_ptr->csp)
	{
		/* Hack -- Since at the moment casting spells without enough mana*/
		/* is impossible, I'm taking this out, as it confuses people. */
		/* chance += 5 * (s_ptr->smana - p_ptr->csp); */
	}

	/* Extract the minimum failure rate */
	minfail = adj_mag_fail[p_ptr->stat_ind[p_ptr->mp_ptr->spell_stat]];

	/* Non mage/sorceror/priest characters never get too good */
	if ((p_ptr->pclass != CLASS_MAGE) && (p_ptr->pclass != CLASS_PRIEST) && (p_ptr->pclass != CLASS_SORCERER))
	{
		if (minfail < 5) minfail = 5;
	}

	/* Hack -- Priest prayer penalty for "edged" weapons  -DGK */
	if ((p_ptr->pclass == 2) && (p_ptr->icky_wield)) chance += 25;

	/* Minimum failure rate */
	if (chance < minfail) chance = minfail;

	/* Stunning makes spells harder */
	if (p_ptr->stun > 50) chance += 25;
	else if (p_ptr->stun) chance += 15;

	/* Always a 5 percent chance of working */
	if (chance > 95) chance = 95;

	/* Return the chance */
	return (chance);
}



/*
 * Determine if a spell is "okay" for the player to cast or study
 * The spell must be legible, not forgotten, and also, to cast,
 * it must be known, and to study, it must not be known.
 */
static bool spell_okay(int Ind, int j, bool known)
{
	player_type *p_ptr = Players[Ind];

	magic_type *s_ptr;

	/* Access the spell */
	s_ptr = &p_ptr->mp_ptr->info[j];

	/* Spell is illegal */
	if (s_ptr->slevel > p_ptr->lev) return (FALSE);

	/* Spell is forgotten */
	if ((j < 32) ?
	    (p_ptr->spell_forgotten1 & (1L << j)) :
	    (p_ptr->spell_forgotten2 & (1L << (j - 32))))
	{
		/* Never okay */
		return (FALSE);
	}

	/* Spell is learned */
	if ((j < 32) ?
	    (p_ptr->spell_learned1 & (1L << j)) :
	    (p_ptr->spell_learned2 & (1L << (j - 32))))
	{
		/* Okay to cast, not to study */
		return (known);
	}

	/* Okay to study, not to cast */
	return (!known);
}



/*
 * Extra information on a spell		-DRS-
 *
 * We can use up to 14 characters of the buffer 'p'
 *
 * The strings in this function were extracted from the code in the
 * functions "do_cmd_cast()" and "do_cmd_pray()" and may be dated.
 */
static void spell_info(int Ind, char *p, int j)
{
	player_type *p_ptr = Players[Ind];

	/* Default */
	strcpy(p, "");

#ifdef DRS_SHOW_SPELL_INFO

	/* Mage spells */
	if (p_ptr->mp_ptr->spell_book == TV_MAGIC_BOOK)
	{
		int plev = p_ptr->lev;

		/* Analyze the spell */
		switch (j)
		{
			case 0: sprintf(p, " dam %dd%d", 3+((plev-1)/5), 4 + (plev / 10)); break;
			case 2: strcpy(p, " range 10"); break;
			case 5: strcpy(p, " heal 2d8"); break;
			case 8: sprintf(p, " dam %d", 10 + (plev / 2)); break;
			case 10: sprintf(p, " dam %dd8", (3+((plev-5)/4))); break;
			case 14: sprintf(p, " range %d", plev * 10); break;
			case 15: strcpy(p, " dam 6d8"); break;
			case 16: sprintf(p, " dam %dd8", (5+((plev-5)/4))); break;
			case 20: sprintf(p, " dam %d", (60 + (plev * 2))); break;
			case 24: sprintf(p, " dam %dd8", (8+((plev-5)/4))); break;
			case 26: sprintf(p, " dam %d", 40 + plev); break;
			case 29: sprintf(p, " dur %d+d20", plev); break;
			case 30: sprintf(p, " dam %d", 75 + (plev * 3 / 2)); break;
			case 38: sprintf(p, " dam %d", 50 + plev/2); break;
			case 39: sprintf(p, " dam %d", 50 + plev); break;
			case 40: sprintf(p, " dam %d", 80 + plev); break;
			case 41: sprintf(p, " dam %d", 75 + plev); break;
			case 42: sprintf(p, " dam %d", 400 + plev*2); break;
			case 43: sprintf(p, " dam %d", 400 + (plev * 3)); break;
			case 49: strcpy(p, " dur 20+d20"); break;
			case 50: strcpy(p, " dur 20+d20"); break;
			case 51: strcpy(p, " dur 20+d20"); break;
			case 52: strcpy(p, " dur 20+d20"); break;
			case 53: strcpy(p, " dur 20+d20"); break;
			case 54: strcpy(p, " dur 25+d25"); break;
			case 55: strcpy(p, " dur 30+d20"); break;
			case 56: strcpy(p, " dur 25+d25"); break;
			case 57: sprintf(p, " dur %d+d25", 30+plev); break;
			case 58: strcpy(p, " dur 8+d8"); break;
		}
	}

	/* Priest spells */
	if (p_ptr->mp_ptr->spell_book == TV_PRAYER_BOOK)
	{
		int plev = p_ptr->lev;

		/* See below */
		int orb = (plev / ((p_ptr->pclass == 2) ? 2 : 4));

		/* Analyze the spell */
		switch (j)
		{
			case 1: strcpy(p, " heal 2d10"); break;
			case 2: strcpy(p, " dur 12+d12"); break;
			case 9: sprintf(p, " range %d", 3*plev); break;
			case 10: strcpy(p, " heal 4d10"); break;
			case 11: strcpy(p, " dur 24+d24"); break;
			case 15: strcpy(p, " dur 10+d10"); break;
			case 17: sprintf(p, " %d+3d6", plev + orb); break;
			case 18: strcpy(p, " heal 6d10"); break;
			case 19: strcpy(p, " dur 24+d24"); break;
			case 20: sprintf(p, " dur %d+d25", 3*plev); break;
			case 23: strcpy(p, " heal 8d10"); break;
			case 25: strcpy(p, " dur 48+d48"); break;
			case 26: sprintf(p, " dam d%d", 3*plev); break;
			case 27: strcpy(p, " heal 300"); break;
			case 28: sprintf(p, " dam d%d", 3*plev); break;
			case 30: strcpy(p, " heal 1000"); break;
			case 36: strcpy(p, " heal 4d10"); break;
			case 37: strcpy(p, " heal 8d10"); break;
			case 38: strcpy(p, " heal 2000"); break;
			case 41: sprintf(p, " dam d%d", 4*plev); break;
			case 42: sprintf(p, " dam d%d", 4*plev); break;
			case 45: sprintf(p, " dam %d", (plev * 5)); break;
			case 52: strcpy(p, " range 10"); break;
			case 53: sprintf(p, " range %d", 8*plev); break;
		}
	}

	/* Sorcery spells */
	if (p_ptr->mp_ptr->spell_book == TV_SORCERY_BOOK)
	{
		int plev = p_ptr->lev;

		/* Analyze the spell */
		switch (j)
		{
			case  0: sprintf(p, " dam %dd%d", 3+((plev-1)/5), 4 + (plev / 10)); break;
                        case  1: sprintf(p, " range %d", 10); break;
                        case  6: sprintf(p, " power %d", (plev * 3) / 2); break;
                        case  8: sprintf(p, " dam %d", 10 + (plev / 2)); break;
                        case  9: sprintf(p, " range %d", plev * 5); break;
                        case 12: sprintf(p, " dam %dd%d", 5 + ((plev - 5) / 4), 8); break;
                        case 15: sprintf(p, " dur %d+d20", 10 + plev); break;
                        case 14: sprintf(p, " dam %dd%d", 6+((plev-5)/4),8); break;
                        case 16: sprintf(p, " dur %d+d20", 20); break;
                        case 17: sprintf(p, " dam %dd%d", 9+((plev-5)/4), 8); break;
                        case 21: sprintf(p, " dur %d+d%d", plev, 20 + plev); break;
                        case 22: sprintf(p, " dam 4*%dd%d", 5+((plev-5)/4), 8); break;
                        case 25: sprintf(p, " dur %d+d20", 20); break;
                        case 28: sprintf(p, " dam %d", 100 + (3 * plev)); break;
                        case 29: sprintf(p, " dur %d+d20", 30); break;
                        case 30: sprintf(p, " dam %d", 130 + (plev * 5 / 2)); break;
                        case 36: sprintf(p, " dur %d+d20", 20); break;
                        case 38: sprintf(p, " dam 8*%dd%d", 15 + (plev / 10), 8); break;
                        case 39: sprintf(p, " dur 8+d10"); break;
                        case 44: sprintf(p, " dam %d", 100 + (3 * plev)); break;
                        case 46: sprintf(p, " dam %d", 600 + (2 * plev)); break;
                        case 56: sprintf(p, " dam %d", 250 + (2 * plev)); break;
                        case 57: sprintf(p, " dam %d", 400 + (2 * plev)); break;
                        case 58: sprintf(p, " dam %d", 500 + (2 * plev)); break;
                        case 60: sprintf(p, " dam 8*%dd%d", 20 + (plev / 10), 10); break;
                        case 61: sprintf(p, " dam %d", 450 + (2 * plev)); break;
                        case 62: sprintf(p, " dam 5*%d", 250 + plev); break;
                        case 63: sprintf(p, " dam %d", 800); break;
		}
	}
	
	/* Rogue spells */
	if (p_ptr->mp_ptr->spell_book == TV_SHADOW_BOOK)
	{
		int plev = p_ptr->lev;

		/* Analyze the spell */
		switch (j)
		{
			case 0: strcpy(p, " range 10"); break;
                        case 1: sprintf(p, " dam %d", 8 + (plev / 2)); break;
                        case 9: sprintf(p, " power %d", 30 + (plev * 3)); break;
                        case 13: sprintf(p, " dur %d+d20", 20 + plev); break;
                        case 18: sprintf(p, " dur %d+d20", 20 + plev); break;
                        case 33: sprintf(p, " range %d", 10 + (plev / 5)); break;
                        case 40: sprintf(p, " dur %d+d20", 20 + plev); break;
                        case 43: sprintf(p, " dur %d+d20", 30 + plev); break;
		}
	}

	
	/* Archer spells */
	if (p_ptr->mp_ptr->spell_book == TV_HUNT_BOOK)
	{
		int plev = p_ptr->lev;

		/* Analyze the spell */
		switch (j)
		{
			case 0: sprintf(p, " dam 3/shot"); break;
			case 2: sprintf(p, " dam 5/shot"); break;
			case 4: sprintf(p, " dam 7/shot"); break;

			case 8: sprintf(p, " dam 8/shot"); break;
			case 11: sprintf(p, " dam 11/shot"); break;

			case 16: sprintf(p, " dam 7/shot"); break;
			case 17: sprintf(p, " dam 11/shot"); break;
			case 20: sprintf(p, " dam 25/shot"); break;
		}
	}

	
	/* Archer spells */
	if (p_ptr->mp_ptr->spell_book == TV_PSI_BOOK)
	{
	  int plev = p_ptr->lev;
	  
	  /* Analyze the spell */
	  switch (j)
	    {
	    case 1: sprintf(p, " dam %dd%d", 3 + ((plev - 1) / 5), 4 + (plev / 5)); break;
	    case 10: sprintf(p, " dur %d+d20", p_ptr->lev); break;
	    case 13: sprintf(p, " dur %d+d20", p_ptr->lev); break;
	    case 14: sprintf(p, " dam 10d%d", p_ptr->lev); break;
	    case 18: sprintf(p, " dur 30+d20"); break;
	    case 19: sprintf(p, " dur 30+d20"); break;
	    case 20: sprintf(p, " dam 100+d%d", plev * 2); break;
	    case 21: sprintf(p, " dam %d", 150 + (plev * 3)); break;
	    case 23: sprintf(p, " dam %d", 200 + (plev * 2)); break;
	    case 33: sprintf(p, " dur %d+d10", p_ptr->lev); break;
	    case 40: sprintf(p, " heal %dd6", plev / 2); break;
	    case 43: sprintf(p, " dur %d+d10", p_ptr->lev / 2); break;
	    case 44: sprintf(p, " dur d%d", p_ptr->lev + 10); break;
	    case 45: sprintf(p, " heal 5000"); break;
	    }
	}
	
#endif

}


/*
 * Print a list of spells (for browsing or casting)
 */
static void print_spells(int Ind, int book, byte *spell, int num)
{
	player_type *p_ptr = Players[Ind];

	int			i, j;

	magic_type		*s_ptr;

	cptr		comment;

	char		info[80];

	char		out_val[160];


	/* Dump the spells */
	for (i = 0; i < num; i++)
	{
		/* Access the spell */
		j = spell[i];

		/* Access the spell */
		s_ptr = &p_ptr->mp_ptr->info[j];

		/* Skip illegible spells */
		if (s_ptr->slevel >= 99)
		{
			sprintf(out_val, "  %c) %-30s", I2A(i), "(illegible)");
			Send_spell_info(Ind, book, i, out_val);
			continue;
		}

		/* XXX XXX Could label spells above the players level */

		/* Get extra info */
		spell_info(Ind, info, j);

		/* Use that info */
		comment = info;

		/* Analyze the spell */
		if ((j < 32) ?
		    ((p_ptr->spell_forgotten1 & (1L << j))) :
		    ((p_ptr->spell_forgotten2 & (1L << (j - 32)))))
		{
			comment = " forgotten";
		}
		else if (!((j < 32) ?
		           (p_ptr->spell_learned1 & (1L << j)) :
		           (p_ptr->spell_learned2 & (1L << (j - 32)))))
		{
			comment = " unknown";
		}
		else if (!((j < 32) ?
		           (p_ptr->spell_worked1 & (1L << j)) :
		           (p_ptr->spell_worked2 & (1L << (j - 32)))))
		{
			comment = " untried";
		}

		/* Dump the spell --(-- */
		sprintf(out_val, "  %c) %-30s%2d %4d %3d%%%s",
		        I2A(i), spell_names[p_ptr->mp_ptr->spell_type][j],
		        s_ptr->slevel, s_ptr->smana, spell_chance(Ind, s_ptr), comment);
		Send_spell_info(Ind, book, i, out_val);
	}
}



/*
 * Allow user to choose a spell/prayer from the given book.
 *
 * If a valid spell is chosen, saves it in '*sn' and returns TRUE
 * If the user hits escape, returns FALSE, and set '*sn' to -1
 * If there are no legal choices, returns FALSE, and sets '*sn' to -2
 *
 * The "prompt" should be "cast", "recite", or "study"
 * The "known" should be TRUE for cast/pray, FALSE for study
 */
#if 0
static int get_spell(int Ind, int *sn, cptr prompt, int sval, bool known)
{
}
#endif




/*
 * Peruse the spells/prayers in a Book
 *
 * Note that *all* spells in the book are listed
 */
void do_cmd_browse(int Ind, int book)
{
	player_type *p_ptr = Players[Ind];

	int			i, item, sval;

	byte		spell[64], num = 0;

	object_type		*o_ptr;


	/* Warriors are illiterate */
	if (!p_ptr->mp_ptr->spell_book)
	{
	  //		msg_print(Ind, "You cannot read books!");
		return;
	}

	/* No lite */
	if (p_ptr->blind || no_lite(Ind))
	{
		msg_print(Ind, "You cannot see!");
		return;
	}

	/* Confused */
	if (p_ptr->confused)
	{
		msg_print(Ind, "You are too confused!");
		return;
	}


	/* Restrict choices to "useful" books */
	item_tester_tval = p_ptr->mp_ptr->spell_book;

	item = book;

	/* Get the item (in the pack) */
	if (item >= 0)
	{
		o_ptr = &p_ptr->inventory[item];
	}

	/* Get the item (on the floor) */
	else
	{
		o_ptr = &o_list[0 - item];
	}

	if (o_ptr->tval != p_ptr->mp_ptr->spell_book)
	{
		msg_print(Ind, "SERVER ERROR: Tried browsing a bad book!");
		return;
	}


	/* Access the item's sval */
	sval = o_ptr->sval;


	/* Extract spells */
	for (i = 0; i < 64; i++)
	{
		/* Check for this spell */
		if ((i < 32) ?
		    (spell_flags[p_ptr->mp_ptr->spell_type][sval][0] & (1L << i)) :
		    (spell_flags[p_ptr->mp_ptr->spell_type][sval][1] & (1L << (i - 32))))
		{
			/* Collect this spell */
			spell[num++] = i;
		}
	}


	/* Display the spells */
	print_spells(Ind, book, spell, num);
}




/*
 * Study a book to gain a new spell/prayer
 */
void do_cmd_study(int Ind, int book, int spell)
{
	player_type *p_ptr = Players[Ind];

	int			i, sval;

	int			j = -1;

	cptr p = ((p_ptr->mp_ptr->spell_book == TV_PRAYER_BOOK) ? "prayer" : "spell");

	object_type		*o_ptr;

	byte spells[64], num = 0;
	
	if (p_ptr->pclass == CLASS_WARRIOR)
	{
		p = "technic";
	}


	if (!p_ptr->mp_ptr->spell_book)
	{
		msg_print(Ind, "You cannot read books!");
		return;
	}

	if (p_ptr->blind || no_lite(Ind))
	{
		msg_print(Ind, "You cannot see!");
		return;
	}

	if (p_ptr->confused)
	{
		msg_print(Ind, "You are too confused!");
		return;
	}

	if (!(p_ptr->new_spells))
	{
		msg_format(Ind, "You cannot learn any new %ss!", p);
		return;
	}


	/* Restrict choices to "useful" books */
	item_tester_tval = p_ptr->mp_ptr->spell_book;

	/* Get the item (in the pack) */
	if (book >= 0)
	{
		o_ptr = &p_ptr->inventory[book];
	}

	/* Get the item (on the floor) */
	else
	{
		o_ptr = &o_list[0 - book];
	}

	if (o_ptr->tval != p_ptr->mp_ptr->spell_book)
	{
		msg_print(Ind, "SERVER ERROR: Trying to gain a spell from a bad book!");
		return;
	}

	/* Access the item's sval */
	sval = o_ptr->sval;


	/* Mage -- Learn a selected spell */
	if ((p_ptr->mp_ptr->spell_book == TV_MAGIC_BOOK) || (p_ptr->mp_ptr->spell_book == TV_HUNT_BOOK) || (p_ptr->mp_ptr->spell_book == TV_PSI_BOOK))
	{
		for (i = 0; i < 64; i++)
		{
			/* Check for this spell */
			if ((i < 32) ?
				(spell_flags[p_ptr->mp_ptr->spell_type][sval][0] & (1L << i)) :
				(spell_flags[p_ptr->mp_ptr->spell_type][sval][1] & (1L << (i - 32))))
			{
				/* Collect this spell */
				spells[num++] = i;
			}
		}

		/* Set the spell number */
		j = spells[spell];

		if (!spell_okay(Ind, j, FALSE))
		{
			msg_print(Ind, "You cannot gain that spell!");
			return;
		}
	}

	/* Sorcery -- Learn a selected spell */
	if (p_ptr->mp_ptr->spell_book == TV_SORCERY_BOOK)
	{
		for (i = 0; i < 64; i++)
		{
			/* Check for this spell */
			if ((i < 32) ?
				(spell_flags[p_ptr->mp_ptr->spell_type][sval][0] & (1L << i)) :
				(spell_flags[p_ptr->mp_ptr->spell_type][sval][1] & (1L << (i - 32))))
			{
				/* Collect this spell */
				spells[num++] = i;
			}
		}

		/* Set the spell number */
		j = spells[spell];

		if (!spell_okay(Ind, j, FALSE))
		{
			msg_print(Ind, "You cannot gain that spell!");
			return;
		}
	}

	/* Shadow -- Learn a selected spell */
	if (p_ptr->mp_ptr->spell_book == TV_SHADOW_BOOK)
	{
		for (i = 0; i < 64; i++)
		{
			/* Check for this spell */
			if ((i < 32) ?
				(spell_flags[p_ptr->mp_ptr->spell_type][sval][0] & (1L << i)) :
				(spell_flags[p_ptr->mp_ptr->spell_type][sval][1] & (1L << (i - 32))))
			{
				/* Collect this spell */
				spells[num++] = i;
			}
		}

		/* Set the spell number */
		j = spells[spell];

		if (!spell_okay(Ind, j, FALSE))
		{
			msg_print(Ind, "You cannot gain that spell!");
			return;
		}
	}

	/* Fighting -- Learn a selected technic */
	if (p_ptr->mp_ptr->spell_book == TV_FIGHT_BOOK)
	{
		for (i = 0; i < 64; i++)
		{
			/* Check for this spell */
			if ((i < 32) ?
				(spell_flags[p_ptr->mp_ptr->spell_type][sval][0] & (1L << i)) :
				(spell_flags[p_ptr->mp_ptr->spell_type][sval][1] & (1L << (i - 32))))
			{
				/* Collect this spell */
				spells[num++] = i;
			}
		}

		/* Set the spell number */
		j = spells[spell];

		if (!spell_okay(Ind, j, FALSE))
		{
			msg_print(Ind, "You cannot gain that technic!");
			return;
		}
	}

	/* Priest -- Learn a random prayer */
	if (p_ptr->mp_ptr->spell_book == TV_PRAYER_BOOK)
	{
		int k = 0;

		/* Extract spells */
		for (i = 0; i < 64; i++)
		{
			/* Check spells in the book */
			if ((i < 32) ?
			    (spell_flags[p_ptr->mp_ptr->spell_type][sval][0] & (1L << i)) :
			    (spell_flags[p_ptr->mp_ptr->spell_type][sval][1] & (1L << (i - 32))))
			{
				/* Skip non "okay" prayers */
				if (!spell_okay(Ind, i, FALSE)) continue;

				/* Hack -- Prepare the randomizer */
				k++;

				/* Hack -- Apply the randomizer */
				if (rand_int(k) == 0) j = i;
			}
		}
	}

	/* Nothing to study */
	if (j < 0)
	{
		/* Message */
		msg_format(Ind, "You cannot learn any %ss in that book.", p);

		/* Abort */
		return;
	}


	/* Take a turn */
	p_ptr->energy -= level_speed(p_ptr->dun_depth);

	/* Learn the spell */
	if (j < 32)
	{
		p_ptr->spell_learned1 |= (1L << j);
	}
	else
	{
		p_ptr->spell_learned2 |= (1L << (j - 32));
	}

	/* Find the next open entry in "spell_order[]" */
	for (i = 0; i < 64; i++)
	{
		/* Stop at the first empty space */
		if (p_ptr->spell_order[i] == 99) break;
	}

	/* Add the spell to the known list */
	p_ptr->spell_order[i++] = j;

	/* Mention the result */
	msg_format(Ind, "You have learned the %s of %s.",
	           p, spell_names[p_ptr->mp_ptr->spell_type][j]);

	/* One less spell available */
	p_ptr->new_spells--;

	/* Report on remaining prayers */
	if (p_ptr->new_spells)
	{
		msg_format(Ind, "You can learn %d more %ss.", p_ptr->new_spells, p);
	}

	/* Save the new_spells value */
	p_ptr->old_spells = p_ptr->new_spells;

	/* Redraw Study Status */
	p_ptr->redraw |= (PR_STUDY);

	/* Update the spell info */
	p_ptr->window |= (PW_SPELL);
}

bool check_antimagic(int Ind)
{
	player_type *p_ptr = Players[Ind];

	int i;

	for (i = 1; i <= NumPlayers; i++)
	  {
		player_type *q_ptr = Players[i];
		int dis, antichance = 0, antidis = 0;
		object_type *o_ptr;

		/* Skip disconnected players */
		if (q_ptr->conn == NOT_CONNECTED) continue;

		/* Skip players not on this depth */
		if (q_ptr->dun_depth != p_ptr->dun_depth) continue;

		if (!q_ptr->anti_magic) continue;

		/* Compute distance */
		dis = distance(p_ptr->py, p_ptr->px, q_ptr->py, q_ptr->px);

		/* Compute the probability of the unbeliever to disrupt any magic attemps */
		if ((q_ptr->pclass == CLASS_UNBELIEVER))
		  {
		    antichance += q_ptr->lev;
		    antidis += 1 + (q_ptr->lev / 11);
		  }
		
		o_ptr = &q_ptr->inventory[INVEN_WIELD];
	        if (o_ptr->k_idx)
		  {
		    u32b f1, f2, f3;

		    object_flags(o_ptr, &f1, &f2, &f3);
		    
		    if (f2 & TR2_ANTI_MAGIC)
		      {
			int minus = o_ptr->to_h + o_ptr->to_d + o_ptr->pval;
		
			/* Enchanting DarkSwords is not a wise thing */
			antichance += 50 - minus;
			antidis += 4 - (minus / 10);
		      }
		  }

		antichance -= p_ptr->lev;
		
		if (antichance > 95) antichance = 95;
	
		if (dis > antidis) antichance = 0;

		/* Got disrupted ? */
		if (magik(antichance))
		  {
		    msg_format(Ind, "%s anti-magic shield disrupts your attemps.", q_ptr->name);
		    return TRUE;
		  }
	  }

	/* Assume no antimagic */
	return FALSE;
}


/*
 * Cast a spell
 *
 * Many of the spells have a strange get_aim_dir(), and then they return.
 * What this does is it sends a PKT_GET_DIRECTION to the client, which then
 * (when the player hits a direction key), send a response to the server.  The
 * server then calls do_cmd_cast_aux() with the direction.  This is a crappy
 * way of doing things, but it should work.  Without this, the server is hung
 * until the player hits a direction key, and we try very hard not to have
 * any undue slowness in the server. --KLJ--
 */
void do_cmd_cast(int Ind, int book, int spell)
{
	player_type *p_ptr = Players[Ind];

	int			i, j, sval;
	int			chance, beam;
	int			plev = p_ptr->lev;

	object_type		*o_ptr;

	magic_type		*s_ptr;

	byte spells[64], num = 0;

	bool antifail = FALSE;

	/* No anti-magic shields around ? */
	if (check_antimagic(Ind)) antifail = TRUE;

	/* Require spell ability */
	if (p_ptr->mp_ptr->spell_book != TV_MAGIC_BOOK)
	{
		msg_print(Ind, "You cannot cast spells!");
		return;
	}

	/* Require lite */
	if (p_ptr->blind || no_lite(Ind))
	{
		msg_print(Ind, "You cannot see!");
		return;
	}

	/* Not when confused */
	if (p_ptr->confused)
	{
		msg_print(Ind, "You are too confused!");
		return;
	}


	/* Restrict choices to spell books */
	item_tester_tval = p_ptr->mp_ptr->spell_book;

	/* Get the item (in the pack) */
	if (book >= 0)
	{
		o_ptr = &p_ptr->inventory[book];
	}

	/* Get the item (on the floor) */
	else
	{
		o_ptr = &o_list[0 - book];
	}

	if (o_ptr->tval != p_ptr->mp_ptr->spell_book)
	{
		msg_print(Ind, "SERVER ERROR: Tried to cast spell from bad book!");
		return;
	}

        if( check_guard_inscription( o_ptr->note, 'm' )) {
                msg_print(Ind, "The item's inscription prevents it");
                return;
        };

	/* Access the item's sval */
	sval = o_ptr->sval;

	for (i = 0; i < 64; i++)
	{
		/* Check for this spell */
		if ((i < 32) ?
			(spell_flags[p_ptr->mp_ptr->spell_type][sval][0] & (1L << i)) :
			(spell_flags[p_ptr->mp_ptr->spell_type][sval][1] & (1L << (i - 32))))
		{
			/* Collect this spell */
			spells[num++] = i;
		}
	}

	/* Set the spell number */
	j = spells[spell];

	if (!spell_okay(Ind, j, 1))
	{
		msg_print(Ind, "You cannot cast that spell.");
		return;
	}

	/* Access the spell */
	s_ptr = &p_ptr->mp_ptr->info[j];

	/* Check mana */
	if (s_ptr->smana > p_ptr->csp)
	{
		msg_print(Ind, "You do not have enough mana.");
		return;
	}

	/* Spell failure chance */
	chance = spell_chance(Ind, s_ptr);

	/* Failed spell */
	if ((rand_int(100) < chance) || antifail)
	{
		if (flush_failure) flush();
		msg_print(Ind, "You failed to get the spell off!");
	}

	/* Process spell */
	else
	{
		/* Hack -- chance of "beam" instead of "bolt" */
		beam = ((p_ptr->pclass == 1) ? plev : (plev / 2));

		/* Spells.  */
		switch (j)
		{
			case 0:
			{
				p_ptr->current_spell = 0;
				get_aim_dir(Ind);
				return;
			}

			case 1:
			{
				(void)detect_creatures(Ind);
				break;
			}

			case 2:
			{
				msg_format_near(Ind, "%s blinks away!", p_ptr->name);
				teleport_player(Ind, 10);
				break;
			}

			case 3:
			{
				(void)lite_area(Ind, damroll(2, (plev / 2)), (plev / 10) + 1);
				break;
			}

			case 4:
			{
				(void)detect_treasure(Ind);
				break;
			}

			case 5:
			{
				(void)hp_player(Ind, damroll(2, 8));
				(void)set_cut(Ind, p_ptr->cut - 15);
				break;
			}

			case 6:
			{
				(void)detect_object(Ind);
				break;
			}

			case 7:
			{
				(void)detect_sdoor(Ind);
				(void)detect_trap(Ind);
				break;
			}

			case 8:
			{
				p_ptr->current_spell = 8;
				get_aim_dir(Ind);
				return;
			}

			case 9:
			{
				p_ptr->current_spell = 9;
				get_aim_dir(Ind);
				return;
			}

			case 10:
			{
				p_ptr->current_spell = 10;
				get_aim_dir(Ind);
				return;
			}

			case 11:
			{
				(void)destroy_doors_touch(Ind);
				break;
			}

			case 12:
			{
				p_ptr->current_spell = 12;
				get_aim_dir(Ind);
				return;
			}

			case 13:
			{
				(void)set_poisoned(Ind, 0);
				break;
			}

			case 14:
			{
				msg_format_near(Ind, "%s teleports away!", p_ptr->name);
				teleport_player(Ind, plev * 5);
				break;
			}

			case 15:
			{
				p_ptr->current_spell = 15;
				get_aim_dir(Ind);
				return;
			}

			case 16:
			{
				p_ptr->current_spell = 16;
				get_aim_dir(Ind);
				return;
			}

			case 17:
			{
				p_ptr->current_spell = 17;
				get_aim_dir(Ind);
				return;
			}

			case 18:
			{
				(void)set_food(Ind, PY_FOOD_MAX - 1);
				break;
			}

			case 19:
			{
				(void)recharge(Ind, 5);
				break;
			}

			case 20:
			{
				p_ptr->current_spell = 20;
				get_aim_dir(Ind);
				return;
			}

			case 21:
			{
				p_ptr->current_spell = 21;
				get_aim_dir(Ind);
				return;
			}

			case 22:
			{
				(void)ident_spell(Ind);
				break;
			}

			case 23:
			{
				(void)sleep_monsters(Ind);
				break;
			}

			case 24:
			{
				p_ptr->current_spell = 24;
				get_aim_dir(Ind);
				return;
			}

			case 25:
			{
				p_ptr->current_spell = 25;
				get_aim_dir(Ind);
				return;
			}

			case 26:
			{
				p_ptr->current_spell = 26;
				get_aim_dir(Ind);
				return;
			}

			case 27:
			{
				(void)recharge(Ind, 40);
				break;
			}

			case 28:
			{
				p_ptr->current_spell = 28;
				get_aim_dir(Ind);
				return;
			}

			case 29:
			{
				msg_format_near(Ind, "%s starts moving faster.", p_ptr->name);
				if (!p_ptr->fast)
				{
					(void)set_fast(Ind, randint(20) + plev);
				}
				else
				{
					(void)set_fast(Ind, p_ptr->fast + randint(5));
				}
				break;
			}

			case 30:
			{
				p_ptr->current_spell = 30;
				get_aim_dir(Ind);
				return;
			}

			case 31:
			{
				msg_format_near(Ind, "%s unleashes great power!", p_ptr->name);
				destroy_area(p_ptr->dun_depth, p_ptr->py, p_ptr->px, 15, TRUE);
				break;
			}

			case 32:
			{
				(void)genocide(Ind);
				break;
			}

			case 33:
			{
				(void)door_creation(Ind);
				break;
			}

			case 34:
			{
				(void)stair_creation(Ind);
				break;
			}

			case 35:
			{
				teleport_player_level(Ind);
				break;
			}

			case 36:
			{
				msg_format_near(Ind, "%s casts a spell, and the ground shakes!", p_ptr->name);
				earthquake(p_ptr->dun_depth, p_ptr->py, p_ptr->px, 10);
				break;
			}

			case 37:
			{
				if (!p_ptr->word_recall)
				{
					set_recall_depth(p_ptr, o_ptr);
					p_ptr->word_recall = rand_int(20) + 15;
					msg_print(Ind, "The air about you becomes charged...");
				}
				else
				{
					p_ptr->word_recall = 0;
					msg_print(Ind, "A tension leaves the air around you...");
				}
				break;
			}

			case 38:
			{
				p_ptr->current_spell = 38;
				get_aim_dir(Ind);
				return;
			}

			case 39:
			{
				p_ptr->current_spell = 39;
				get_aim_dir(Ind);
				return;
			}

			case 40:
			{
				p_ptr->current_spell = 40;
				get_aim_dir(Ind);
				return;
			}

			case 41:
			{
				p_ptr->current_spell = 41;
				get_aim_dir(Ind);
				return;
			}

			case 42:
			{
				p_ptr->current_spell = 42;
				get_aim_dir(Ind);
				return;
			}

			case 43:
			{
				p_ptr->current_spell = 43;
				get_aim_dir(Ind);
				return;
			}

			case 44:
			{
				(void)detect_evil(Ind);
				break;
			}

			case 45:
			{
				(void)detect_magic(Ind);
				break;
			}

			case 46:
			{
				recharge(Ind, 100);
				break;
			}

			case 47:
			{
				(void)genocide(Ind);
				break;
			}

			case 48:
			{
				(void)mass_genocide(Ind);
				break;
			}

			case 49:
			{
				(void)set_oppose_fire(Ind, p_ptr->oppose_fire + randint(20) + 20);
				break;
			}

			case 50:
			{
				(void)set_oppose_cold(Ind, p_ptr->oppose_cold + randint(20) + 20);
				break;
			}

			case 51:
			{
				(void)set_oppose_acid(Ind, p_ptr->oppose_acid + randint(20) + 20);
				break;
			}

			case 52:
			{
				(void)set_oppose_pois(Ind, p_ptr->oppose_pois + randint(20) + 20);
				break;
			}

			case 53:
			{
				(void)set_oppose_acid(Ind, p_ptr->oppose_acid + randint(20) + 20);
				(void)set_oppose_elec(Ind, p_ptr->oppose_elec + randint(20) + 20);
				(void)set_oppose_fire(Ind, p_ptr->oppose_fire + randint(20) + 20);
				(void)set_oppose_cold(Ind, p_ptr->oppose_cold + randint(20) + 20);
				(void)set_oppose_pois(Ind, p_ptr->oppose_pois + randint(20) + 20);
				break;
			}

			case 54:
			{
				(void)hp_player(Ind, 10);
				(void)set_hero(Ind, p_ptr->hero + randint(25) + 25);
				(void)set_afraid(Ind, 0);
				break;
			}

			case 55:
			{
				msg_format_near(Ind, "%s forms a mystic shield.", p_ptr->name);
				(void)set_shield(Ind, p_ptr->shield + randint(20) + 30);
				break;
			}

			case 56:
			{
				msg_format_near(Ind, "%s enters a battle rage!", p_ptr->name);
				(void)hp_player(Ind, 30);
				(void)set_shero(Ind, p_ptr->shero + randint(25) + 25);
				(void)set_afraid(Ind, 0);
				break;
			}

			case 57:
			{
				if (!p_ptr->fast)
				{
					(void)set_fast(Ind, randint(30) + 30 + plev);
				}
				else
				{
					(void)set_fast(Ind, p_ptr->fast + randint(10));
				}
				break;
			}

			case 58:
			{
				(void)set_invuln(Ind, p_ptr->invuln + randint(8) + 8);
				break;
			}
		}

		/* A spell was cast */
		if (!((j < 32) ?
		      (p_ptr->spell_worked1 & (1L << j)) :
		      (p_ptr->spell_worked2 & (1L << (j - 32)))))
		{
			int e = s_ptr->sexp;

			/* The spell worked */
			if (j < 32)
			{
				p_ptr->spell_worked1 |= (1L << j);
			}
			else
			{
				p_ptr->spell_worked2 |= (1L << (j - 32));
			}

			/* Gain experience */
			gain_exp(Ind, e * s_ptr->slevel);

			/* Fix the spell info */
			p_ptr->window |= PW_SPELL;
		}
	}

	/* Take a turn */
	p_ptr->energy -= level_speed(p_ptr->dun_depth) / p_ptr->num_spell;

	/* Sufficient mana */
	if (s_ptr->smana <= p_ptr->csp)
	{
		/* Use some mana */
		p_ptr->csp -= s_ptr->smana;
	}

	/* Over-exert the player */
	else
	{
		int oops = s_ptr->smana - p_ptr->csp;

		/* No mana left */
		p_ptr->csp = 0;
		p_ptr->csp_frac = 0;

		/* Message */
		msg_print(Ind, "You faint from the effort!");

		/* Hack -- Bypass free action */
		(void)set_paralyzed(Ind, p_ptr->paralyzed + randint(5 * oops + 1));

		/* Damage CON (possibly permanently) */
		if (rand_int(100) < 50)
		{
			bool perm = (rand_int(100) < 25);

			/* Message */
			msg_print(Ind, "You have damaged your health!");

			/* Reduce constitution */
			(void)dec_stat(Ind, A_CON, 15 + randint(10), perm);
		}
	}

	/* Redraw mana */
	p_ptr->redraw |= (PR_MANA);

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);
}


/*
 * Finish casting a spell that required a direction --KLJ--
 */
void do_cmd_cast_aux(int Ind, int dir)
{
	player_type *p_ptr = Players[Ind];

	int plev = p_ptr->lev;
	int beam = ((p_ptr->pclass == 1) ? plev : (plev / 2));

	magic_type *s_ptr = &p_ptr->mp_ptr->info[p_ptr->current_spell];

	/* Only fire in direction 5 if we have a target */
	if ((dir == 5) && !target_okay(Ind))
	{
		/* Reset current spell */
		p_ptr->current_spell = -1;

		/* Done */
		return;
	}

	/* We assume that the spell can be cast, and so forth */
	switch(p_ptr->current_spell)
	{
		case 0:
		{
			msg_format_near(Ind, "%s fires a magic missile.", p_ptr->name);
			fire_bolt_or_beam(Ind, beam-10, GF_MISSILE, dir,
						damroll(3 + ((plev - 1) / 5), 4 + (plev / 10)));
			break;
		}

		case 8:
		{
			msg_format_near(Ind, "%s casts a stinking cloud.", p_ptr->name);
			fire_ball(Ind, GF_POIS, dir,
				10 + (plev / 2), 2);
			break;
		}

		case 9:
		{
			confuse_monster(Ind, dir, plev);
			break;
		}

		case 10:
		{
			msg_format_near(Ind, "%s casts a lightning bolt.", p_ptr->name);
			fire_bolt_or_beam(Ind, beam-10, GF_ELEC, dir,
				damroll(3+((plev-5)/4), 8));
			break;
		}

		case 12:
		{
			sleep_monster(Ind, dir);
			break;
		}

		case 15:
		{
			msg_print(Ind, "A line of blue shimmering light appears.");
			lite_line(Ind, dir);
			break;
		}

		case 16:
		{
			msg_format_near(Ind, "%s casts a frost bolt.", p_ptr->name);
			fire_bolt_or_beam(Ind, beam-10, GF_COLD, dir,
				damroll(5+((plev-5)/4), 8));
			break;
		}

		case 17:
		{
			(void)wall_to_mud(Ind, dir);
			break;
		}

		/* Tidal Wave */
		case 20:
		{
			msg_format_near(Ind, "%s invoke a tidal wave.", p_ptr->name);
			fire_ball(Ind, GF_WATER, dir, 60 + (plev * 2), 2 + (plev / 30));
			break;
		}

		case 21:
		{
			(void)poly_monster(Ind, dir);
			break;
		}

		case 24:
		{
			msg_format_near(Ind, "%s casts a fire bolt.", p_ptr->name);
			fire_bolt_or_beam(Ind, beam, GF_FIRE, dir,
				damroll(8+((plev-5)/4), 8));
			break;
		}

		case 25:
		{
			(void)slow_monster(Ind, dir);
			break;
		}

		case 26:
		{
			msg_format_near(Ind, "%s casts a frost ball.", p_ptr->name);
			fire_ball(Ind, GF_COLD, dir, 40 + (plev), 4);
			break;
		}

		case 28:
		{
			(void)teleport_monster(Ind, dir);
			break;
		}

		case 30:
		{
			msg_format_near(Ind, "%s casts a fire ball.", p_ptr->name);
			fire_ball(Ind, GF_FIRE, dir, 75 + (plev * 3 / 2), 2);
			break;
		}

		case 38:
		{
			msg_format_near(Ind, "%s casts a cloud of death.", p_ptr->name);
			fire_ball(Ind, GF_POIS, dir, 30 + (plev / 2), 3);
			break;
		}

		case 39:
		{
			msg_format_near(Ind, "%s casts an acid ball.", p_ptr->name);
			fire_ball(Ind, GF_ACID, dir, 50 + (plev), 2);
			break;
		}

		case 40:
		{
			msg_format_near(Ind, "%s casts a frost ball.", p_ptr->name);
			fire_ball(Ind, GF_COLD, dir, 80 + (plev), 3);
			break;
		}

		case 41:
		{
			msg_format_near(Ind, "%s casts a meteor shower.", p_ptr->name);
			fire_ball(Ind, GF_METEOR, dir, 75 + (plev), 3);
			break;
		}

		case 42:
		{
			msg_format_near(Ind, "%s casts a mana ball.", p_ptr->name);
			fire_ball(Ind, GF_MANA, dir, 400 + (plev * 2), 3 + (plev / 45));
			break;
		}

		case 43:
		{
			msg_format_near(Ind, "%s casts a mana strike.", p_ptr->name);
			fire_bolt(Ind, GF_MANA, dir, 400 + (plev * 3));
			break;
		}

		default:  /* For some reason we got called for a spell that */
		{         /* doesn't require a direction */
			msg_print(Ind, "SERVER ERROR: do_cmd_cast_aux() called for non-directional spell!");
			p_ptr->current_spell = -1;
			return;
		}
	}	

	if (!((p_ptr->current_spell < 32) ?
		(p_ptr->spell_worked1 & (1L << p_ptr->current_spell)) :
		(p_ptr->spell_worked2 & (1L << (p_ptr->current_spell - 32)))))
	{
		int e = s_ptr->sexp;

		if (p_ptr->current_spell < 32)
		{
			p_ptr->spell_worked1 |= (1L << p_ptr->current_spell);
		}
		else
		{
			p_ptr->spell_worked2 |= (1L << (p_ptr->current_spell - 32));
		}

		gain_exp(Ind, e * s_ptr->slevel);

		/* Fix the spell info */
		p_ptr->window |= PW_SPELL;
	}

	p_ptr->energy -= level_speed(p_ptr->dun_depth) / p_ptr->num_spell;

	if (s_ptr->smana <= p_ptr->csp)
	{
		/* Use some mana */
		p_ptr->csp -= s_ptr->smana;
	}

	/* Over-exert the player */
	else
	{
		int oops = s_ptr->smana - p_ptr->csp;

		/* No mana left */
		p_ptr->csp = 0;
		p_ptr->csp_frac = 0;

		/* Message */
		msg_print(Ind, "You faint from the effort!");

		/* Hack -- bypass free action */
		(void)set_paralyzed(Ind, p_ptr->paralyzed + randint(5 * oops + 1));

		/* Damage CON (possibly permanently) */
		if (rand_int(100) < 50)
		{
			bool perm = (rand_int(100) < 25);

			/* Message */
			msg_print(Ind, "You have damaged your health!");

			/* Reduce constitution */
			(void)dec_stat(Ind, A_CON, 15 + randint(10), perm);
		}
	}

	/* Reset current spell */
	p_ptr->current_spell = -1;

	/* Resend mana */
	p_ptr->redraw |= (PR_MANA);

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);
}


/*
 * Cast a sorcery spell
 *
 * Many of the spells have a strange get_aim_dir(), and then they return.
 * What this does is it sends a PKT_GET_DIRECTION to the client, which then
 * (when the player hits a direction key), send a response to the server.  The
 * server then calls do_cmd_sorc_aux() with the direction.  This is a crappy
 * way of doing things, but it should work.  Without this, the server is hung
 * until the player hits a direction key, and we try very hard not to have
 * any undue slowness in the server. --KLJ--
 */
void do_cmd_sorc(int Ind, int book, int spell)
{
	player_type *p_ptr = Players[Ind];

	int			i, j, sval;
	int			chance, beam;
	int			plev = p_ptr->lev;

	object_type		*o_ptr;

	magic_type		*s_ptr;

	byte spells[64], num = 0;

	bool antifail = FALSE;

	/* No anti-magic shields around ? */
	if (check_antimagic(Ind)) antifail = TRUE;
	
	/* Require spell ability */
	if (p_ptr->mp_ptr->spell_book != TV_SORCERY_BOOK)
	{
		msg_print(Ind, "You cannot cast spells!");
		return;
	}

	/* Require lite */
	if (p_ptr->blind || no_lite(Ind))
	{
		msg_print(Ind, "You cannot see!");
		return;
	}

	/* Not when confused */
	if (p_ptr->confused)
	{
		msg_print(Ind, "You are too confused!");
		return;
	}


	/* Restrict choices to spell books */
	item_tester_tval = p_ptr->mp_ptr->spell_book;

	/* Get the item (in the pack) */
	if (book >= 0)
	{
		o_ptr = &p_ptr->inventory[book];
	}

	/* Get the item (on the floor) */
	else
	{
		o_ptr = &o_list[0 - book];
	}

	if (o_ptr->tval != p_ptr->mp_ptr->spell_book)
	{
		msg_print(Ind, "SERVER ERROR: Tried to cast spell from bad book!");
		return;
	}

        if(check_guard_inscription(o_ptr->note, 'm')) {
                msg_print(Ind, "The item's inscription prevents it");
                return;
        };

	/* Access the item's sval */
	sval = o_ptr->sval;

	for (i = 0; i < 64; i++)
	{
		/* Check for this spell */
		if ((i < 32) ?
			(spell_flags[p_ptr->mp_ptr->spell_type][sval][0] & (1L << i)) :
			(spell_flags[p_ptr->mp_ptr->spell_type][sval][1] & (1L << (i - 32))))
		{
			/* Collect this spell */
			spells[num++] = i;
		}
	}

	/* Set the spell number */
	if (spell < 64) j = spells[spell];
	/* Affecting other spells */
	else j = spells[spell-64];

	if (!spell_okay(Ind, j, 1))
	{
		msg_print(Ind, "You cannot cast that spell.");
		return;
	}

	/* Access the spell */
	s_ptr = &p_ptr->mp_ptr->info[j];

	/* Check mana */
	if (s_ptr->smana > p_ptr->csp)
	{
		msg_print(Ind, "You do not have enough mana.");
		return;
	}

	/* Spell failure chance */
	chance = spell_chance(Ind, s_ptr);

	/* Failed spell */
	if ((rand_int(100) < chance) || antifail)
	{
		if (flush_failure) flush();
		msg_print(Ind, "You failed to get the spell off!");
	}

	/* Process spell */
	else
	{
		/* Hack -- chance of "beam" instead of "bolt" */
		beam = ((p_ptr->pclass == 1) ? plev : (plev / 2));
		
		if (spell >= 64) j += 64;

		/* Spells.  */
		switch (j)
		{
			/* Magic Missile */
			case 0:
			{
				p_ptr->current_spell = j;
				get_aim_dir(Ind);
				return;
			}
	   case 1: /* Phase Door */
                        teleport_player(Ind, 10);
		       break;
           case 2: /* Detect Monsters */
			(void)detect_creatures(Ind);
		       break;
           case 3: /* Detect Traps */
			(void)detect_trap(Ind);
		       break;
           case 4: /* Light Area */
                        (void)lite_area(Ind, damroll(2, (plev / 2)), (plev / 10) + 1);
                       break;
           case 5: /* Detect Doors/Stairs */
                        (void)detect_sdoor(Ind);
		       break;
           case 6: /* Confuse Monster */
			p_ptr->current_spell = j;
			get_aim_dir(Ind);
			return;
                case 7: /* Detect Objects */
                        (void)detect_object(Ind);
                        break;

                case 8: /* Noxious Cloud */
			p_ptr->current_spell = j;
			get_aim_dir(Ind);
			return;
                case 9: /* Teleport */
                        teleport_player(Ind, plev * 5);
                        break;
                case 10: /* Beam of Light */
			p_ptr->current_spell = j;
			get_aim_dir(Ind);
			return;
                case 11: /* Sleep Monster */
			p_ptr->current_spell = j;
			get_aim_dir(Ind);
			return;
                case 12: /* Lightning Bolt */
			p_ptr->current_spell = j;
			get_aim_dir(Ind);
			return;
                case 13: /* Stone to Mud */
			p_ptr->current_spell = j;
			get_aim_dir(Ind);
			return;
                case 14: /* Frost Bolt */
			p_ptr->current_spell = j;
			get_aim_dir(Ind);
			return;
                case 15: /* Wraithform */
                        set_tim_wraith(Ind, p_ptr->tim_wraith + 10 + plev + randint(20));
                        break;
                case 15+64: /* Wraithform someone else */
			p_ptr->current_spell = 15;
			get_aim_dir(Ind);
                        return;

                case 16: /* Ethereal Eye */
                        map_area(Ind);
                        set_tim_invis(Ind, p_ptr->tim_invis + 20 + randint(20));
                        break;
                case 17: /* Fire Bolt */
			p_ptr->current_spell = j;
			get_aim_dir(Ind);
			return;
                case 18: /* Identify */
			(void)ident_spell(Ind);
                        break;
                case 19: /* Typhoon Daze */
                        fire_ball(Ind, GF_CONFUSION, 0, 30, 2);
                        break;
                case 20: /* Time Distortion */
                        slow_monsters(Ind);
                        break;
                case 21: /* Haste Self */
			if (!p_ptr->fast)
			{
                                (void)set_fast(Ind, randint(20 + (plev)) + plev);
			}
			else
			{
                                (void)set_fast(Ind, p_ptr->fast + randint(5));
			}
                        break;
                case 21+64: /* Haste others */
			p_ptr->current_spell = 21;
			get_aim_dir(Ind);
			return;
                case 22: /* Elemental Blast */
			p_ptr->current_spell = j;
			get_aim_dir(Ind);
			return;
                case 23: /* Teleport Away */
			p_ptr->current_spell = j;
			get_aim_dir(Ind);
			return;

                case 24: /* Summon Food */
                {
			(void)set_food(Ind, PY_FOOD_MAX - 1);
                        break;
                }
                case 25: /* Meditation */
		  //                	if (!p_ptr->tim_meditation)
                        	set_tim_meditation(Ind, p_ptr->tim_meditation + 20 + randint(20));
				//     else
                        	//set_tim_meditation(Ind, p_ptr->tim_meditation + 5);
                        break;
                case 26: /* Gravitic Distortion */
                        fire_ball(Ind, GF_GRAVITY, 0, 10, 2);
                        break;
                case 27: /* Collapse Cieling */
                {
			p_ptr->current_spell = j;
			get_aim_dir(Ind);
                        return;
                }
		       break;
                case 28: /* Firestorm */
			p_ptr->current_spell = j;
			get_aim_dir(Ind);
			return;
                case 29: /* Force Shield */
                	if (!p_ptr->shield)
                        	(void)set_shield(Ind, p_ptr->shield + randint(20) + 30);
                 	else
                        	(void)set_shield(Ind, p_ptr->shield + 10);
		        break;
                case 29+64: /* Force Shield others */
			p_ptr->current_spell = 29;
			get_aim_dir(Ind);
			return;
                case 30: /* Sunburn */
			p_ptr->current_spell = j;
			get_aim_dir(Ind);
                        return;
                case 31: /* Safeguard */
                {
                	int i, j, rad = 2 + (plev / 20);
                	
			msg_format_near(Ind, "%s murmurs, glyphs of warding erupt!", p_ptr->name);
                	
                	for (i = p_ptr->px - rad; i <= p_ptr->px + rad; i++)
                	for (j = p_ptr->py - rad; j <= p_ptr->py + rad; j++)
                	{
                		/* First we must be in the dungeon */
                		if (!in_bounds(p_ptr->dun_depth, j, i)) continue;
                		
                		/* Is it a naked grid ? */
                		if (!cave_naked_bold(p_ptr->dun_depth, j, i)) continue;
                		
                		/* Beware of the houses in town */
                		if ((p_ptr->dun_depth <= 0) && (cave[p_ptr->dun_depth][j][i].info & CAVE_ICKY)) continue;
                		
                		/* Now we want a circle */
                		if (distance(j, i, p_ptr->py, p_ptr->px) != rad) continue;
                		
                		/* Everything ok ? then put a glyph */
				cave[p_ptr->dun_depth][j][i].feat = FEAT_GLYPH;
				note_spot(Ind, j, i);
				everyone_lite_spot(p_ptr->dun_depth, j, i);
                	}
		        break;
                }

                case 32: /* Gravitic Beam */
			p_ptr->current_spell = j;
			get_aim_dir(Ind);
			return;
                case 33: /* Sanctuary */
                        fire_ball(Ind, GF_STONE_WALL, 0, 1, 1 + (plev / 40));
 		        break;
                case 34: /* Escape */
                {
			(void)stair_creation(Ind);
                        break;
                }
                case 35: /* Statis Cage */
			p_ptr->current_spell = j;
			get_aim_dir(Ind);
			return;
                case 36: /* Elemental Shield */
                        (void)set_oppose_acid(Ind, p_ptr->oppose_acid + randint(20) + 20);
                        (void)set_oppose_elec(Ind, p_ptr->oppose_elec + randint(20) + 20);
                        (void)set_oppose_fire(Ind, p_ptr->oppose_fire + randint(20) + 20);
                        (void)set_oppose_cold(Ind, p_ptr->oppose_cold + randint(20) + 20);
                        (void)set_oppose_pois(Ind, p_ptr->oppose_pois + randint(20) + 20);
                        break;
                case 37: /*  Recall */
			if (!p_ptr->word_recall)
			{
				set_recall_depth(p_ptr, o_ptr);
				p_ptr->word_recall = rand_int(20) + 15;
				msg_print(Ind, "The air about you becomes charged...");
			}
			else
			{
				p_ptr->word_recall = 0;
				msg_print(Ind, "A tension leaves the air around you...");
			}
                        break;
                case 37+64: /* Recall others */
			p_ptr->current_spell = 37;
			get_aim_dir(Ind);
			return;
                case 38: /* Force of the Elements */
                {
                        int i;

                        for (i = 1; i < 10; i++)
                        {
                                if (i-5)
                                {
                                        if(magik(50)) fire_bolt(Ind, GF_FIRE, i, damroll(15 + (plev / 10), 8));
                                        if(magik(50)) fire_bolt(Ind, GF_COLD, i, damroll(14 + (plev / 10), 8));
                                        if(magik(50)) fire_bolt(Ind, GF_ELEC, i, damroll(12 + (plev / 10), 8));
                                        if(magik(50)) fire_bolt(Ind, GF_ACID, i, damroll(18 + (plev / 10), 8));
                                }
                        }
                        break;
                }
                case 39: /* Disruption Shield */
			(void)set_tim_manashield(Ind, p_ptr->tim_manashield + randint(10) + 8);
                        break;

                case 40: /* Earthquake */
                        earthquake(p_ptr->dun_depth, p_ptr->py, p_ptr->px, 5);
                        break;
                case 41: /* Polymorph */
			p_ptr->current_spell = j;
			get_aim_dir(Ind);
			return;
                case 42: /* XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX */
                        break;
                case 43: /* Warp Space */
                        fire_ball(Ind, GF_GRAVITY, 0, 60, 4);
                        break;
                case 44: /* Chaos Blast */
                        fire_ball(Ind, GF_CHAOS, 0, 100 + (3 * plev), 5);
                        break;
                case 45: /* Wipe */
                	msg_format_near(Ind, "%s mumurs 'wipe'.", p_ptr->name);
                	wipe_spell(p_ptr->dun_depth, p_ptr->py, p_ptr->px, 10);
                        take_hit(Ind, 100, "the Wipe spell");
                        break;
                case 46: /* Pyrrhic Blast */
                        fire_ball(Ind, GF_MANA, 0, 600 + (2 * plev), 3);
                        take_hit(Ind, 90, "the heat of a Pyrrhic Blast");
                        break;
                case 47: /* Word of Destruction */
                        destroy_area(p_ptr->dun_depth, p_ptr->py, p_ptr->px, 15, TRUE);
                        break;

                case 48: /* Radiate Fear */
                        fire_ball(Ind, GF_TURN_ALL, 0, 20, 10);
                        break;
                case 49: /* Mind Vision */
		{
			  int Ind2 = get_player(Ind, o_ptr);

			  if (!Ind2) break;

			  if (Ind == Ind2)
			    {
			      msg_print(Ind, "You cannot mind vision yourself.");
			      break;
			    }
			  if (p_ptr->esp_link)
			    {
			      msg_print(Ind, "Your mind is already linked.");
			      break;
			    }
			  if (Players[Ind2]->esp_link)
			    {
			      msg_format(Ind, "%s mind is already linked.", Players[Ind2]->name);
			      break;
			    }

			  msg_format(Ind2, "%s infiltrates your mind.", p_ptr->name);
			  msg_format(Ind, "You infiltrate %s mind.", Players[Ind2]->name);
			  p_ptr->esp_link = Players[Ind2]->id;
			  p_ptr->esp_link_type = LINK_DOMINANT;
			  p_ptr->esp_link_flags = 0;

			  Players[Ind2]->esp_link = p_ptr->id;
			  Players[Ind2]->esp_link_type = LINK_DOMINATED;
			  Players[Ind2]->esp_link_flags = LINKF_VIEW;
			  break;
		}
                case 50: /* Forcefull Graze */
			p_ptr->current_spell = j;
			get_aim_dir(Ind);
			return;
                case 51: /* Recharging II */
                        recharge(Ind, 40);
                        break;
                case 52: /* Telekinesis */
		  telekinesis(Ind, o_ptr);
                        break;
                case 53: /* Self-Scan */
			self_knowledge(Ind);
                        break;
                case 54: /* Id II */
                        identify_fully(Ind);
                        break;
                case 55: /* Clairvoyance */
                        wiz_lite(Ind);
                        break;

                case 56: /* Force Channeling */
		  {      
		        int Ind2 = get_player(Ind, o_ptr);

			if (!Ind2) break;
			if (Ind == Ind2)
			  {
			    msg_print(Ind, "You must channel the force to another player.");
			    break;
			  }

			msg_format(Ind2, "You sense %s's Magic channeling through you.", p_ptr->name);
                        fire_ball(Ind2, GF_FORCE, 0, 250 + (2 * plev), 4);
                        break;
		  }
                case 57: /* Plasma Eruption */
                        fire_ball(Ind, GF_PLASMA, 0, 400 + (2 * plev), 4);
                        break;
                case 58: /* Annihilate */
			p_ptr->current_spell = j;
			get_aim_dir(Ind);
			return;
                case 59: /* Olbivion Blast */
                        mass_genocide(Ind);
                        break;
                case 60: /* Mana Spin */
                {
                        int i;

                        for (i = 1; i < 10; i++)
                        {
                                if (i-5)
                                {
                                        fire_bolt(Ind, GF_MANA, i, damroll(20 + (plev / 10), 10));
                                }
                        }
                        break;
                }
                case 61: /* Tidal Wave */
			p_ptr->current_spell = j;
			get_aim_dir(Ind);
			return;
                case 62: /* Anarchy Strike */
                {
                        int i, j;

                        for(j = 0; j < 5; j++)
                        {
                                i = randint(10);
                                while(i == 5) i = randint(10);
                                (void)fire_ball(Ind, GF_CHAOS, i, 250 + plev, 3);
                        }
                        break;
                }
                case 63: /* Mana Strike */
			p_ptr->current_spell = j;
			get_aim_dir(Ind);
			return;
		}

		/* A spell was cast */
		if (!((j < 32) ?
		      (p_ptr->spell_worked1 & (1L << j)) :
		      (p_ptr->spell_worked2 & (1L << (j - 32)))))
		{
			int e = s_ptr->sexp;

			/* The spell worked */
			if (j < 32)
			{
				p_ptr->spell_worked1 |= (1L << j);
			}
			else
			{
				p_ptr->spell_worked2 |= (1L << (j - 32));
			}

			/* Gain experience */
			gain_exp(Ind, e * s_ptr->slevel);

			/* Fix the spell info */
			p_ptr->window |= PW_SPELL;
		}
	}

	/* Take a turn */
	p_ptr->energy -= level_speed(p_ptr->dun_depth) / p_ptr->num_spell;

	/* Sufficient mana */
	if (s_ptr->smana <= p_ptr->csp)
	{
		/* Use some mana */
		p_ptr->csp -= s_ptr->smana;
	}

	/* Over-exert the player */
	else
	{
		int oops = s_ptr->smana - p_ptr->csp;

		/* No mana left */
		p_ptr->csp = 0;
		p_ptr->csp_frac = 0;

		/* Message */
		msg_print(Ind, "You faint from the effort!");

		/* Hack -- Bypass free action */
		(void)set_paralyzed(Ind, p_ptr->paralyzed + randint(5 * oops + 1));

		/* Damage CON (possibly permanently) */
		if (rand_int(100) < 50)
		{
			bool perm = (rand_int(100) < 25);

			/* Message */
			msg_print(Ind, "You have damaged your health!");

			/* Reduce constitution */
			(void)dec_stat(Ind, A_CON, 15 + randint(10), perm);
		}
	}

	/* Redraw mana */
	p_ptr->redraw |= (PR_MANA);

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);
}


/*
 * Finish casting a spell that required a direction --KLJ--
 */
void do_cmd_sorc_aux(int Ind, int dir)
{
	player_type *p_ptr = Players[Ind];

	int plev = p_ptr->lev;
	int beam = ((p_ptr->pclass == CLASS_SORCERER) ? plev : (plev / 2));

	magic_type *s_ptr = &p_ptr->mp_ptr->info[p_ptr->current_spell];

	/* Only fire in direction 5 if we have a target */
	if ((dir == 5) && !target_okay(Ind))
	{
		/* Reset current spell */
		p_ptr->current_spell = -1;

		/* Done */
		return;
	}

	/* We assume that the spell can be cast, and so forth */
	switch(p_ptr->current_spell)
	{
		case 0:
		{
			msg_format_near(Ind, "%s fires a magic missile.", p_ptr->name);
			fire_bolt_or_beam(Ind, beam-10, GF_MISSILE, dir,
						damroll(3 + ((plev - 1) / 5), 4 + (plev / 10)));
			break;
		}
	        case 6: /* Confuse Monster */
			msg_format_near(Ind, "%s looks mismerizing.", p_ptr->name);
                        (void)confuse_monster(Ind, dir, ( plev * 3) / 2);
			break;
                case 8: /* Noxious Cloud */
			msg_format_near(Ind, "%s fires a noxious cloud.", p_ptr->name);
			fire_ball(Ind, GF_POIS, dir, 10 + (plev / 2), 2);
                        break;
                case 10: /* Beam of Light */
			msg_format_near(Ind, "%s creates a line of blue shimmering light appears.", p_ptr->name);
                        lite_line(Ind, dir);
                        break;
                case 11: /* Sleep Monster */
                        (void)sleep_monster(Ind, dir);
                        break;
                case 12: /* Lightning Bolt */
			msg_format_near(Ind, "%s fires a lightning bolt.", p_ptr->name);
			fire_bolt_or_beam(Ind, beam-10, GF_ELEC, dir,
                                          damroll(5+((plev-5)/4), 8));
                        break;
                case 13: /* Stone to Mud */
			(void)wall_to_mud(Ind, dir);
                        break;
                case 14: /* Frost Bolt */
			msg_format_near(Ind, "%s fires a frost bolt.", p_ptr->name);
			fire_bolt_or_beam(Ind, beam-10, GF_COLD, dir, damroll(6+((plev-5)/4), 8));
                        break;
                case 15: /* Wraithform someone else */
			project_hook(Ind, GF_WRAITH_PLAYER, dir, 10 + plev + randint(20), PROJECT_STOP | PROJECT_KILL);
                        break;
                case 17: /* Fire Bolt */
			msg_format_near(Ind, "%s fires a fire bolt.", p_ptr->name);
			fire_bolt_or_beam(Ind, beam, GF_FIRE, dir,
                                damroll(9+((plev-5)/4), 8));
                        break;
                case 21: /* haste others */
			project_hook(Ind, GF_SPEED_PLAYER, dir, randint(20 + (plev)) + plev, PROJECT_STOP | PROJECT_KILL);
                        break;
                case 22: /* Elemental Blast */
			msg_format_near(Ind, "%s fires an elemental blast.", p_ptr->name);
			fire_bolt_or_beam(Ind, beam, GF_FIRE, dir,
                                damroll(5+((plev-5)/4), 8));
                        fire_bolt_or_beam(Ind, beam, GF_COLD, dir,
                                damroll(5+((plev-5)/4), 8));
                        fire_bolt_or_beam(Ind, beam, GF_ACID, dir,
                                damroll(5+((plev-5)/4), 8));
                        fire_bolt_or_beam(Ind, beam, GF_ELEC, dir,
                                damroll(5+((plev-5)/4), 8));
		       break;
                case 23: /* Teleport Away */
                        (void)fire_beam(Ind, GF_AWAY_ALL, dir, plev);
                        break;
                case 27: /* Collapse Cieling */
                        fire_bolt(Ind, GF_EARTHQUAKE, dir, 3);
                        break;

                case 28: /* Firestorm */
 			msg_format_near(Ind, "%s fires a firestorm.", p_ptr->name);
                        (void)fire_ball(Ind, GF_FIRE, dir, 100 + (3 * plev), 2);
		       break;
                case 29: /* force shield others */
			project_hook(Ind, GF_SHIELD_PLAYER, dir, randint(20) + 30, PROJECT_STOP | PROJECT_KILL);
                        break;
                case 30: /* Sunfire */
 			msg_format_near(Ind, "%s invokes the sunfire.", p_ptr->name);
                        (void)fire_ball(Ind, GF_LITE, dir, 130 + (plev * 5 / 2), 3);
		       break;
                case 32: /* Gravitic Beam */
			msg_format_near(Ind, "%s fires a gravitic beam", p_ptr->name);
                       (void)fire_beam(Ind, GF_GRAVITY, dir, 20);
		       break;
                case 35: /* Statis Cage */
			msg_format_near(Ind, "%s creates a static cage.", p_ptr->name);
                        (void)fire_ball(Ind, GF_OLD_SLEEP, dir, 100, 0);
                        break;
                case 37: /* Recall others */
			project_hook(Ind, GF_RECALL_PLAYER, dir, randint(20) + 15, PROJECT_STOP | PROJECT_KILL);
                        break;
                case 41: /* Polymorph */
			(void)poly_monster(Ind, dir);
                        break;
                case 50: /* Forcefull Graze */
			msg_format_near(Ind, "%s casts a forcefull graze.", p_ptr->name);
                        (void)fire_bolt(Ind, GF_SOUND, dir, 50);
                        break;
                case 58: /* Annihilate */
			msg_format_near(Ind, "%s fires an annihilation bolt.", p_ptr->name);
                        (void)fire_bolt(Ind, GF_MISSILE, dir, 500 + (2 * plev));
                        break;
                case 61: /* Tidal Wave */
			msg_format_near(Ind, "%s fires a tidal wave.", p_ptr->name);
                        (void)fire_ball(Ind, GF_WATER, dir, 450 + (2 * plev), 3);
                        break;
                case 63: /* Mana Strike */
			msg_format_near(Ind, "%s fires a mana strike.", p_ptr->name);
                        (void)fire_ball(Ind, GF_MANA, dir, 800, 0);
                        break;

		default:  /* For some reason we got called for a spell that */
		{         /* doesn't require a direction */
			msg_print(Ind, "SERVER ERROR: do_cmd_sorc_aux() called for non-directional spell!");
			p_ptr->current_spell = -1;
			return;
		}
	}	

	if (!((p_ptr->current_spell < 32) ?
		(p_ptr->spell_worked1 & (1L << p_ptr->current_spell)) :
		(p_ptr->spell_worked2 & (1L << (p_ptr->current_spell - 32)))))
	{
		int e = s_ptr->sexp;

		if (p_ptr->current_spell < 32)
		{
			p_ptr->spell_worked1 |= (1L << p_ptr->current_spell);
		}
		else
		{
			p_ptr->spell_worked2 |= (1L << (p_ptr->current_spell - 32));
		}

		gain_exp(Ind, e * s_ptr->slevel);

		/* Fix the spell info */
		p_ptr->window |= PW_SPELL;
	}

	p_ptr->energy -= level_speed(p_ptr->dun_depth) / p_ptr->num_spell;

	if (s_ptr->smana <= p_ptr->csp)
	{
		/* Use some mana */
		p_ptr->csp -= s_ptr->smana;
	}

	/* Over-exert the player */
	else
	{
		int oops = s_ptr->smana - p_ptr->csp;

		/* No mana left */
		p_ptr->csp = 0;
		p_ptr->csp_frac = 0;

		/* Message */
		msg_print(Ind, "You faint from the effort!");

		/* Hack -- bypass free action */
		(void)set_paralyzed(Ind, p_ptr->paralyzed + randint(5 * oops + 1));

		/* Damage CON (possibly permanently) */
		if (rand_int(100) < 50)
		{
			bool perm = (rand_int(100) < 25);

			/* Message */
			msg_print(Ind, "You have damaged your health!");

			/* Reduce constitution */
			(void)dec_stat(Ind, A_CON, 15 + randint(10), perm);
		}
	}

	/* Reset current spell */
	p_ptr->current_spell = -1;

	/* Resend mana */
	p_ptr->redraw |= (PR_MANA);

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);
}

	

/*
 * Brand the current weapon
 */
static void brand_weapon(int Ind)
{
	player_type *p_ptr = Players[Ind];

	object_type *o_ptr;

	o_ptr = &p_ptr->inventory[INVEN_WIELD];

	/* you can never modify artifacts / ego-items */
	/* you can never modify broken / cursed items */
	if ((o_ptr->k_idx) &&
	    (!artifact_p(o_ptr)) && (!ego_item_p(o_ptr)) &&
	    (!broken_p(o_ptr)) && (!cursed_p(o_ptr)))
	{
		cptr act = NULL;

		char o_name[80];

		if (rand_int(100) < 25)
		{
			act = "is covered in a fiery shield!";
			o_ptr->name2 = EGO_BRAND_FIRE;
		}

		else
		{
			act = "glows deep, icy blue!";
			o_ptr->name2 = EGO_BRAND_COLD;
		}

		object_desc(Ind, o_name, o_ptr, FALSE, 0);

		msg_format(Ind, "Your %s %s", o_name, act);

		enchant(Ind, o_ptr, rand_int(3) + 4, ENCH_TOHIT | ENCH_TODAM);
	}

	else
	{
		if (flush_failure) flush();
		msg_print(Ind, "The Branding failed.");
	}
}


/*
 * Pray a prayer
 *
 * See above for an explanation of the weirdness in this function --KLJ--
 */
void do_cmd_pray(int Ind, int book, int spell)
{
	player_type *p_ptr = Players[Ind];

	int item, sval, j, chance, i;
	int plev = p_ptr->lev;

	object_type	*o_ptr;

	magic_type  *s_ptr;

	byte spells[64], num = 0;

	bool antifail = FALSE;

	/* No anti-magic shields around ? */
	if (check_antimagic(Ind)) antifail = TRUE;

	/* Must use prayer books */
	if (p_ptr->mp_ptr->spell_book != TV_PRAYER_BOOK)
	{
		msg_print(Ind, "Pray hard enough and your prayers may be answered.");
		return;
	}

	/* Must have lite */
	if (p_ptr->blind || no_lite(Ind))
	{
		msg_print(Ind, "You cannot see!");
		return;
	}

	/* Must not be confused */
	if (p_ptr->confused)
	{
		msg_print(Ind, "You are too confused!");
		return;
	}


	/* Restrict choices */
	item_tester_tval = p_ptr->mp_ptr->spell_book;

	item = book;

	/* Get the item (in the pack) */
	if (item >= 0)
	{
		o_ptr = &p_ptr->inventory[item];
	}

	/* Get the item (on the floor) */
	else
	{
		o_ptr = &o_list[0 - item];
	}

	if (o_ptr->tval != p_ptr->mp_ptr->spell_book)
	{
		msg_print(Ind, "SERVER ERROR: Tried to pray prayer from bad book!");
		return;
	}
        if( check_guard_inscription( o_ptr->note, 'p' )) {
                msg_print(Ind, "The item's inscription prevents it");
                return;
        };

	/* Access the item's sval */
	sval = o_ptr->sval;

	for (i = 0; i < 64; i++)
	{
		/* Check for this spell */
		if ((i < 32) ?
			(spell_flags[p_ptr->mp_ptr->spell_type][sval][0] & (1L << i)) :
			(spell_flags[p_ptr->mp_ptr->spell_type][sval][1] & (1L << (i - 32))))
		{
			/* Collect this spell */
			spells[num++] = i;
		}
	}
	
	/* OK, this is a unsightly kludge to get some extra (heal other) 
	prayers without modifying the spell_flags... but it should work for 
	now. -AD- */
	
	/* normal prayers */
	if (spell < 64) j = spells[spell];
	
	/* heal other prayers */
	else j = spells[spell-64];
	
	if (!spell_okay(Ind, j, 1))
	{
		msg_print(Ind, "You cannot pray that prayer.");
		return;
	}
	
	/* get the spell info */
	s_ptr = &p_ptr->mp_ptr->info[j];
	
	/* Check mana */
	if (s_ptr->smana > p_ptr->csp)
	{
		msg_print(Ind, "You do not have enough mana.");
		return;
	}

	/* Spell failure chance */
	chance = spell_chance(Ind, s_ptr);

	/* Check for failure */
	if ((rand_int(100) < chance) || antifail)
	{
		if (flush_failure) flush();
		msg_print(Ind, "You failed to concentrate hard enough!");
	}

	/* Success */
	else
	{
		if (spell >= 64) j += 64;
		switch (j)
		{
			case 0:
			{
				(void)detect_evil(Ind);
				break;
			}

			case 1:
			{
				(void)hp_player(Ind, damroll(2, 10));
				(void)set_cut(Ind, p_ptr->cut - 10);				
				break;
			}

			/* cure light wounds projectile */
			case 1+64:
			{				
				p_ptr->current_spell = 1;
				get_aim_dir(Ind);
				
				return;
			}

			case 2:
			{
				(void)set_blessed(Ind, p_ptr->blessed + randint(12) + 12);
				break;
			}

			case 3:
			{
				(void)set_afraid(Ind, 0);
				break;
			}

			case 4:
			{
				msg_format_near(Ind, "%s calls light.", p_ptr->name);
				(void)lite_area(Ind, damroll(2, (plev / 2)), (plev / 10) + 1);
				break;
			}

			case 5:
			{
				(void)detect_trap(Ind);
				break;
			}

			case 6:
			{
				(void)detect_sdoor(Ind);
				break;
			}

			case 7:
			{
				(void)set_poisoned(Ind, p_ptr->poisoned / 2);
				break;
			}

			case 8:
			{
				p_ptr->current_spell = 8;
				get_aim_dir(Ind);
				return;
			}

			case 9:
			{
				msg_format_near(Ind, "%s blinks away!", p_ptr->name);
				teleport_player(Ind, plev * 3);
				break;
			}

			case 10:
			{
				(void)hp_player(Ind, damroll(4, 10));
				(void)set_cut(Ind, (p_ptr->cut / 2) - 20);
				break;
			}

			/* cure serious wounds projectile */
			case 10+64:
			{				
				p_ptr->current_spell = 10;
				get_aim_dir(Ind);
				return;
			}


			case 11:
			{
				(void)set_blessed(Ind, p_ptr->blessed + randint(24) + 24);
				break;
			}

			case 12:
			{
				(void)sleep_monsters_touch(Ind);
				break;
			}

			case 13:
			{
				(void)set_food(Ind, PY_FOOD_MAX - 1);
				break;
			}

			case 14:
			{
				remove_curse(Ind);
				break;
			}

			case 15:
			{
				(void)set_oppose_fire(Ind, p_ptr->oppose_fire + randint(10) + 10);
				(void)set_oppose_cold(Ind, p_ptr->oppose_cold + randint(10) + 10);
				break;
			}

			case 16:
			{
				(void)set_poisoned(Ind, 0);
				break;
			}

			case 17:
			{
				p_ptr->current_spell = 17;
				get_aim_dir(Ind);
				return;
			}

			case 18:
			{
				(void)hp_player(Ind, damroll(6, 10));
				(void)set_cut(Ind, 0);
				break;
			}

			/* cure critical wounds projectile */
			case 18+64:
			{
				p_ptr->current_spell = 18;
				get_aim_dir(Ind);
				return;
			}

			case 19:
			{
				(void)set_tim_invis(Ind, p_ptr->tim_invis + randint(24) + 24);
				break;
			}

			case 20:
			{
				(void)set_protevil(Ind, p_ptr->protevil + randint(25) + 3 * p_ptr->lev);
				break;
			}

			case 21:
			{
				msg_format_near(Ind, "%s murmurs, and the ground shakes!", p_ptr->name);
				earthquake(p_ptr->dun_depth, p_ptr->py, p_ptr->px, 10);
				break;
			}

			case 22:
			{
				map_area(Ind);
				break;
			}

			case 23:
			{
				(void)hp_player(Ind, damroll(8, 10));
				(void)set_stun(Ind, 0);
				(void)set_cut(Ind, 0);
				break;
			}

			case 24:
			{
				(void)turn_undead(Ind);
				break;
			}

			case 25:
			{
				(void)set_blessed(Ind, p_ptr->blessed + randint(48) + 48);
				break;
			}

			case 26:
			{
				(void)dispel_undead(Ind, plev * 3);
				break;
			}

			case 27:
			{
				(void)hp_player(Ind, 300);
				(void)set_stun(Ind, 0);
				(void)set_cut(Ind, 0);
				break;
			}

			case 28:
			{
				(void)dispel_evil(Ind, plev * 3);
				break;
			}

			case 29:
			{
				msg_format_near(Ind, "%s lays down a glyph of warding.", p_ptr->name);
				warding_glyph(Ind);
				break;
			}

			case 30:
			{
				(void)dispel_evil(Ind, plev * 4);
				(void)hp_player(Ind, 1000);
				(void)set_afraid(Ind, 0);
				(void)set_poisoned(Ind, 0);
				(void)set_stun(Ind, 0);
				(void)set_cut(Ind, 0);
				break;
			}

			case 31:
			{
				(void)detect_creatures(Ind);
				break;
			}

			case 32:
			{
				(void)detection(Ind);
				break;
			}

			case 33:
			{
				(void)ident_spell(Ind);
				break;
			}

			case 34:
			{
				(void)probing(Ind);
				break;
			}

			case 35:
			{
				wiz_lite(Ind);
				break;
			}

			case 36:
			{
				(void)hp_player(Ind, damroll(4, 10));
				(void)set_cut(Ind, 0);
				break;
			}

			case 37:
			{
				(void)hp_player(Ind, damroll(8, 10));
				(void)set_stun(Ind, 0);
				(void)set_cut(Ind, 0);
				break;
			}

			case 38:
			{
				(void)hp_player(Ind, 2000);
				(void)set_stun(Ind, 0);
				(void)set_cut(Ind, 0);
				break;
			}

			/* heal other spell */
			case 38+64:
			{
				p_ptr->current_spell = 38;
				get_aim_dir(Ind);
				return;
			}

			/* should ressurect ghosts now */
			case 39:
			{
				/* if we didnt ressurect anybody */
				if (!do_scroll_life(Ind))
				{
					(void)do_res_stat(Ind, A_STR);
					(void)do_res_stat(Ind, A_INT);
					(void)do_res_stat(Ind, A_WIS);
					(void)do_res_stat(Ind, A_DEX);
					(void)do_res_stat(Ind, A_CON);
					(void)do_res_stat(Ind, A_CHR);
				}
				break;
			}

			case 40:
			{
				if (!do_restoreXP_other(Ind))
				{
					(void)restore_level(Ind);
				}
				break;
			}

			case 41:
			{
				
				(void)dispel_undead(Ind, plev * 4);
				break;
			}

			case 42:
			{
				(void)dispel_evil(Ind, plev * 4);
				break;
			}

			case 43:
			{
				if (banish_evil(Ind, 100))
				{
					msg_print(Ind, "The power of your god banishes evil!");
				}
				break;
			}

			case 44:
			{
				msg_format_near(Ind, "%s unleashs a spell of great power!", p_ptr->name);
				destroy_area(p_ptr->dun_depth, p_ptr->py, p_ptr->px, 15, TRUE);
				break;
			}

			case 45:
			{
				p_ptr->current_spell = 45;
				get_aim_dir(Ind);
				return;
			}

			case 46:
			{
				(void)destroy_doors_touch(Ind);
				break;
			}

			case 47:
			{
				(void)recharge(Ind, 15);
				break;
			}

			case 48:
			{
				(void)remove_all_curse(Ind);
				break;
			}

			case 49:
			{
				(void)enchant_spell(Ind, rand_int(4) + 1, rand_int(4) + 1, 0);
				break;
			}

			case 50:
			{
				(void)enchant_spell(Ind, 0, 0, rand_int(3) + 2);
				break;
			}

			case 51:
			{
				brand_weapon(Ind);
				break;
			}

			case 52:
			{
				msg_format_near(Ind, "%s blinks away!", p_ptr->name);
				teleport_player(Ind, 10);
				break;
			}

			case 53:
			{
				msg_format_near(Ind, "%s teleports away!", p_ptr->name);
				teleport_player(Ind, plev * 8);
				break;
			}

			case 54:
			{
				p_ptr->current_spell = 54;
				get_aim_dir(Ind);
				return;
			}

			case 55:
			{
				teleport_player_level(Ind);
				break;
			}

			case 56:
			{
				if (p_ptr->word_recall == 0)
				{
					set_recall_depth(p_ptr, o_ptr);
					p_ptr->word_recall = rand_int(20) + 15;
					msg_print(Ind, "The air about you becomes charged...");
				}
				else
				{
					p_ptr->word_recall = 0;
					msg_print(Ind, "A tension leaves the air around you...");
				}
				break;
			}

			case 57:
			{
				// Temporily disabled until I have time to fix this.
				// -APD
				/*
				msg_print(Ind, "The world changes!");
				p_ptr->new_level_flag = TRUE;
				p_ptr->new_level_method = LEVEL_RAND;
				break;
				*/
				break;
			}
		}

		/* A prayer was prayed */
		if (!((j < 32) ?
		      (p_ptr->spell_worked1 & (1L << j)) :
		      (p_ptr->spell_worked2 & (1L << (j - 32)))))
		{
			int e = s_ptr->sexp;

			/* The spell worked */
			if (j < 32)
			{
				p_ptr->spell_worked1 |= (1L << j);
			}
			else
			{
				p_ptr->spell_worked2 |= (1L << (j - 32));
			}

			/* Gain experience */
			gain_exp(Ind, e * s_ptr->slevel);

			/* Fix the spell info */
			p_ptr->window |= PW_SPELL;
		}
	}

	/* Take a turn */
	p_ptr->energy -= level_speed(p_ptr->dun_depth) / p_ptr->num_spell;

	/* Sufficient mana */
	if (s_ptr->smana <= p_ptr->csp)
	{
		/* Use some mana */
		p_ptr->csp -= s_ptr->smana;
	}

	/* Over-exert the player */
	else
	{
		int oops = s_ptr->smana - p_ptr->csp;

		/* No mana left */
		p_ptr->csp = 0;
		p_ptr->csp_frac = 0;

		/* Message */
		msg_print(Ind, "You faint from the effort!");

		/* Hack -- Bypass free action */
		(void)set_paralyzed(Ind, p_ptr->paralyzed + randint(5 * oops + 1));

		/* Damage CON (possibly permanently) */
		if (rand_int(100) < 50)
		{
			bool perm = (rand_int(100) < 25);

			/* Message */
			msg_print(Ind, "You have damaged your health!");

			/* Reduce constitution */
			(void)dec_stat(Ind, A_CON, 15 + randint(10), perm);
		}
	}

	/* Redraw mana */
	p_ptr->redraw |= (PR_MANA);

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);
}

void do_cmd_pray_aux(int Ind, int dir)
{
	player_type *p_ptr = Players[Ind];

	int plev = p_ptr->lev;
	
	magic_type *s_ptr = &p_ptr->mp_ptr->info[p_ptr->current_spell];

	/* Only fire in direction 5 if we have a target */
	if ((dir == 5) && !target_okay(Ind))
	{
		/* Reset current spell */
		p_ptr->current_spell = -1;

		/* Done */
		return;
	}

	/* We assume that the spell can be cast, and so forth */
	switch(p_ptr->current_spell)
	{
		
		case 1:
		{
			(void)cure_light_wounds_proj(Ind, dir);
			break;
		}
	
		case 8:
		{
			(void)fear_monster(Ind, dir, plev);
			break;
		}

		case 10:
		{
			(void)cure_serious_wounds_proj(Ind, dir);
			break;
		}

		case 17:
		{
			msg_format_near(Ind, "%s fires a holy orb!", p_ptr->name);
			fire_ball(Ind, GF_HOLY_ORB, dir,
				(damroll(3, 6) + plev +
				 (plev / ((p_ptr->pclass == 2) ? 2 : 4))),
				 ((plev < 30) ? 2 : 3));
			break;
		}

		case 18: 
		{
			(void)cure_critical_wounds_proj(Ind, dir);
			break;	
		}

		case 38:
		{
			heal_other_proj(Ind,dir);
			break;
		}

		case 45:
		{
			msg_print(Ind, "Gilthoniel A Elbereth!");
			msg_format_near(Ind, "%s shouts 'Gilthoniel A Elbereth!'.", p_ptr->name);
			fire_ball(Ind, GF_HOLY_ORB, dir,
			          (plev * 5), (plev / 7) + 2);
			(void)hp_player(Ind, 500);
			break;
		}

		case 54:
		{
			(void)teleport_monster(Ind, dir);
			break;
		}

		default:   /* Something went wrong */
		{
			msg_print(Ind, "SERVER ERROR: do_cmd_prayer_aux() called for non-directional prayer!");
			p_ptr->current_spell = -1;
		}
	}

	if (!((p_ptr->current_spell < 32) ?
		(p_ptr->spell_worked1 & (1L << p_ptr->current_spell)) :
		(p_ptr->spell_worked2 & (1L << (p_ptr->current_spell - 32)))))
	{
		int e = s_ptr->sexp;

		if (p_ptr->current_spell < 32)
		{
			p_ptr->spell_worked1 |= (1L << p_ptr->current_spell);
		}
		else
		{
			p_ptr->spell_worked2 |= (1L << (p_ptr->current_spell - 32));
		}

		gain_exp(Ind, e * s_ptr->slevel);

		/* Fix the spell info */
		p_ptr->window |= PW_SPELL;
	}

	p_ptr->energy -= level_speed(p_ptr->dun_depth) / p_ptr->num_spell;

	if (s_ptr->smana <= p_ptr->csp)
	{
		/* Use some mana */
		p_ptr->csp -= s_ptr->smana;
	}

	/* Over-exert the player */
	else
	{
		int oops = s_ptr->smana - p_ptr->csp;

		/* No mana left */
		p_ptr->csp = 0;
		p_ptr->csp_frac = 0;

		/* Message */
		msg_print(Ind, "You faint from the effort!");

		/* Hack -- bypass free action */
		(void)set_paralyzed(Ind, p_ptr->paralyzed + randint(5 * oops - 1));

		/* Damage CON (possibly permanently) */
		if (rand_int(100) < 50)
		{
			bool perm = (rand_int(100) < 25);

			/* Message */
			msg_print(Ind, "You have damaged your health!");

			/* Reduce constitution */
			(void)dec_stat(Ind, A_CON, 15 + randint(10), perm);
		}
	}

	/* Reset current spell */
	p_ptr->current_spell = -1;

	/* Resend mana */
	p_ptr->redraw |= (PR_MANA);

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);
}


/*
 * Send the ghost spell info to the client.
 */
void show_ghost_spells(int Ind)
{
	magic_type *s_ptr;
	int i, j = 0;
	char out_val[80];
	cptr comment = "";

	/* Check each spell */
	for (i = 0; i < 64; i++)
	{
		s_ptr = &ghost_spells[i];

		/* Check for existance */
		if (s_ptr->slevel >= 99) continue;

		/* Format information */
		sprintf(out_val, "  %c) %-30s%2d %4d %3d%%%s",
		        I2A(j), spell_names[GHOST_SPELLS][i], s_ptr->slevel, s_ptr->smana, 0, comment);

		/* Send it */
		Send_spell_info(Ind, 0, j, out_val);

		/* Next spell */
		j++;
	}
}

/*
 * Use a ghostly ability. --KLJ--
 */
void do_cmd_ghost_power(int Ind, int ability)
{
	player_type *p_ptr = Players[Ind];
	magic_type *s_ptr = &ghost_spells[ability];
	int plev = p_ptr->lev;
	int i, j = 0;

	/* Check for ghost-ness */
	if (!p_ptr->ghost) return;

	/* Must not be confused */
	if (p_ptr->confused)
	{
		/* Message */
		msg_print(Ind, "You are too confused!");
		return;
	}

	/* Check spells */
	for (i = 0; i < 64; i++)
	{
		s_ptr = &ghost_spells[i];

		/* Check for existance */
		if (s_ptr->slevel >= 99) continue;

		/* Next spell */
		if (j++ == ability) break;
	}

	/* Check for level */
	if (s_ptr->slevel > plev)
	{
		/* Message */
		msg_print(Ind, "You aren't powerful enough to use that ability.");
		return;
	}

	/* Spell effects */
	switch(i)
	{
		case 0:
		{
			teleport_player(Ind, 10);
			break;
		}
		case 1:
		{
			get_aim_dir(Ind);
			p_ptr->current_spell = 1;
			return;
		}
		case 2:
		{
			get_aim_dir(Ind);
			p_ptr->current_spell = 2;
			return;
		}
		case 3:
		{
			teleport_player(Ind, plev * 8);
			break;
		}
		case 4:
		{
			get_aim_dir(Ind);
			p_ptr->current_spell = 4;
			return;
		}
		case 5:
		{
			get_aim_dir(Ind);
			p_ptr->current_spell = 5;
			return;
		}
		case 6:
		{
			get_aim_dir(Ind);
			p_ptr->current_spell = 6;
			return;
		}
	}

	/* Take a turn */
	p_ptr->energy -= level_speed(p_ptr->dun_depth);

	/* Take some experience */
	p_ptr->max_exp -= s_ptr->slevel * s_ptr->smana;
	p_ptr->exp -= s_ptr->slevel * s_ptr->smana;

	/* Too much can kill you */
	if (p_ptr->exp < 0) take_hit(Ind, 5000, "the strain of ghostly powers");

	/* Check experience levels */
	check_experience(Ind);

	/* Redraw experience */
	p_ptr->redraw |= (PR_EXP);

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);
}


/*
 * Directional ghost ability
 */
void do_cmd_ghost_power_aux(int Ind, int dir)
{
	player_type *p_ptr = Players[Ind];
	int plev = p_ptr->lev;
	magic_type *s_ptr;
	
	/* Verify spell number */
	if (p_ptr->current_spell < 0)
		return;

	/* Acquire spell pointer */
	s_ptr = &ghost_spells[p_ptr->current_spell];

	/* We assume everything is still OK to cast */
	switch (p_ptr->current_spell)
	{
		case 1:
		{
			(void)fear_monster(Ind, dir, plev);
			break;
		}
		case 2:
		{
			confuse_monster(Ind, dir, plev);
			break;
		}
		case 4:
		{
			fire_bolt_or_beam(Ind, plev * 2, GF_NETHER, dir, 50 + damroll(5, 5) + plev);
			break;
		}
		case 5:
		{
			fire_ball(Ind, GF_NETHER, dir, 100 + 2 * plev, 2);
			break;
		}
		case 6:
		{
			fire_ball(Ind, GF_DARK, dir, plev * 5 + damroll(10, 10), 3);
			break;
		}
	}

	/* No more spell */
	p_ptr->current_spell = -1;

	/* Take a turn */
	p_ptr->energy -= level_speed(p_ptr->dun_depth);

	/* Take some experience */
	p_ptr->max_exp -= s_ptr->slevel * s_ptr->smana;
	p_ptr->exp -= s_ptr->slevel * s_ptr->smana;

	/* Too much can kill you */
	if (p_ptr->exp < 0) take_hit(Ind, 5000, "the strain of ghostly powers");

	/* Check experience levels */
	check_experience(Ind);

	/* Redraw experience */
	p_ptr->redraw |= (PR_EXP);

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);
}


/*
 * Use a warrior fighting technic
 */
void do_cmd_fight(int Ind, int book, int spell)
{
	player_type *p_ptr = Players[Ind];

	int			i, j, sval;
	int			chance;
	int			plev = p_ptr->lev;

	object_type		*o_ptr;

	magic_type		*s_ptr;

	byte spells[64], num = 0;

	/* Require spell ability */
	if (p_ptr->mp_ptr->spell_book != TV_FIGHT_BOOK)
	{
		msg_print(Ind, "You cannot use fighting technics!");
		return;
	}

	/* Require lite */
	if (p_ptr->blind || no_lite(Ind))
	{
		msg_print(Ind, "You cannot see!");
		return;
	}

	/* Not when confused */
	if (p_ptr->confused)
	{
		msg_print(Ind, "You are too confused!");
		return;
	}


	/* Restrict choices to spell books */
	item_tester_tval = p_ptr->mp_ptr->spell_book;

	/* Get the item (in the pack) */
	if (book >= 0)
	{
		o_ptr = &p_ptr->inventory[book];
	}

	/* Get the item (on the floor) */
	else
	{
		o_ptr = &o_list[0 - book];
	}

	if (o_ptr->tval != p_ptr->mp_ptr->spell_book)
	{
		msg_print(Ind, "SERVER ERROR: Tried to use technic from bad book!");
		return;
	}

        if(check_guard_inscription(o_ptr->note, 'n')) {
                msg_print(Ind, "The item's inscription prevents it");
                return;
        };

	/* Access the item's sval */
	sval = o_ptr->sval;

	for (i = 0; i < 64; i++)
	{
		/* Check for this spell */
		if ((i < 32) ?
			(spell_flags[p_ptr->mp_ptr->spell_type][sval][0] & (1L << i)) :
			(spell_flags[p_ptr->mp_ptr->spell_type][sval][1] & (1L << (i - 32))))
		{
			/* Collect this spell */
			spells[num++] = i;
		}
	}

	/* Set the spell number */
	j = spells[spell];

	if (!spell_okay(Ind, j, 1))
	{
		msg_print(Ind, "You cannot use that technic.");
		return;
	}

	/* Access the spell */
	s_ptr = &p_ptr->mp_ptr->info[j];

	/* Check mana */
	if (s_ptr->smana > p_ptr->csp)
	{
		msg_print(Ind, "You do not have enough stamina.");
		return;
	}

	/* Spell failure chance */
	chance = spell_chance(Ind, s_ptr);

	/* Failed spell */
	if (rand_int(100) < chance)
	{
		if (flush_failure) flush();
		msg_print(Ind, "You failed to get the technic off!");
	}

	/* Process spell */
	else
	{
		/* Spells.  */
		switch (j)
		{
			/* Confusing Strike */
			case 0:
			{
				if (p_ptr->confusing == FALSE)
				{
					msg_print(Ind, "Your hands begin to glow.");
					p_ptr->confusing = TRUE;
				}
				break;
			}
			
			/* Heroism */
			case 1:
			{
				msg_format_near(Ind, "%s enters in a battle rage.", p_ptr->name);
				set_hero(Ind, p_ptr->hero + 20 + randint(20));
				break;
			}
			
			/* Jump */
			case 2:
			{
				get_aim_dir(Ind);
				p_ptr->current_spell = j;
				return;
			}
			
			/* Stunning Blow */
			case 3:
			{
				if (p_ptr->stunning == FALSE)
				{
					msg_print(Ind, "Your hands get heavy.");
					p_ptr->stunning = TRUE;
				}
				break;
			}
			
			/* Berserk */
			case 4:
			{
				msg_format_near(Ind, "%s enters in a battle RAGE.", p_ptr->name);
				set_shero(Ind, p_ptr->hero + 20 + randint(20));
				break;
			}
			
			/* Iron Skin */
			case 8:
			{
				msg_format_near(Ind, "%s skin turns into iron.", p_ptr->name);
				set_shield(Ind, p_ptr->shield + 20 + randint(20) + plev);
				break;
			}

                        /* Dig */
                        case 9:
			{
				get_aim_dir(Ind);
				p_ptr->current_spell = j;
				return;
			}
			
			/* Furry */
                        case 10:
			{
				msg_format_near(Ind, "%s enters a battle *RAGE*.", p_ptr->name);
				set_shield(Ind, p_ptr->furry + 10 + randint(10) + (plev / 2));
				break;
			}
			
                        /* Spin */
                        case 10:
			{
                                int d;

                                for (d = 1; d <= 9; d++)
                                {
                                        int x, y;

                                        if (d == 5) continue;

                                        x = p_ptr->px + ddirx[d];
                                        y = p_ptr->py + ddiry[d];

                                        if (!in_bounds(p_ptr->dun_depth, y, x)) continue;
                                        py_attack(Ind, y, x);
                                }
				break;
			}
		}

		/* A spell was cast */
		if (!((j < 32) ?
		      (p_ptr->spell_worked1 & (1L << j)) :
		      (p_ptr->spell_worked2 & (1L << (j - 32)))))
		{
			int e = s_ptr->sexp;

			/* The spell worked */
			if (j < 32)
			{
				p_ptr->spell_worked1 |= (1L << j);
			}
			else
			{
				p_ptr->spell_worked2 |= (1L << (j - 32));
			}

			/* Gain experience */
			gain_exp(Ind, e * s_ptr->slevel);

			/* Fix the spell info */
			p_ptr->window |= PW_SPELL;
		}
	}

	/* Take a turn */
	p_ptr->energy -= level_speed(p_ptr->dun_depth);

	/* Sufficient mana */
	if (s_ptr->smana <= p_ptr->csp)
	{
		/* Use some mana */
		p_ptr->csp -= s_ptr->smana;
	}

	/* Over-exert the player */
	else
	{
		int oops = s_ptr->smana - p_ptr->csp;

		/* No mana left */
		p_ptr->csp = 0;
		p_ptr->csp_frac = 0;

		/* Message */
		msg_print(Ind, "You faint from the effort!");

		/* Hack -- Bypass free action */
		(void)set_paralyzed(Ind, p_ptr->paralyzed + randint(5 * oops + 1));

		/* Damage CON (possibly permanently) */
		if (rand_int(100) < 50)
		{
			bool perm = (rand_int(100) < 25);

			/* Message */
			msg_print(Ind, "You have damaged your health!");

			/* Reduce constitution */
			(void)dec_stat(Ind, A_CON, 15 + randint(10), perm);
		}
	}

	/* Redraw mana */
	p_ptr->redraw |= (PR_MANA);

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);
}

/*
 * Finish using a technic that required a direction --KLJ--
 */
void do_cmd_fight_aux(int Ind, int dir)
{
	player_type *p_ptr = Players[Ind];

	int plev = p_ptr->lev;

	magic_type *s_ptr = &p_ptr->mp_ptr->info[p_ptr->current_spell];

	/* Only fire in direction 5 if we have a target */
	if ((dir == 5) && !target_okay(Ind))
	{
		/* Reset current spell */
		p_ptr->current_spell = -1;

		/* Done */
		return;
	}

	/* We assume that the technic can be used, and so forth */
	switch(p_ptr->current_spell)
	{
                case 2:
		{
			int ty, tx, rad = 2 + (plev / 10);
		
			/* Use the given direction */
			tx = p_ptr->px + rad * ddx[dir];
			ty = p_ptr->py + rad * ddy[dir];

			/* Hack -- Use an actual "target" */
			if ((dir == 5) && target_okay(Ind))
			{
				tx = p_ptr->target_col;
				ty = p_ptr->target_row;
			
				if (distance(ty, tx, p_ptr->py, p_ptr->px) > rad)
				{
					msg_print(Ind, "You can not jump that far.");
					return;
				}
			}
		
			msg_format_near(Ind, "%s jumps.", p_ptr->name);
			teleport_player_to(Ind, ty, tx);
			break;
		}
                /* Dig */
                case 9:
		{
                        int ty, tx;
		
			/* Use the given direction */
                        tx = p_ptr->px + ddx[dir];
                        ty = p_ptr->py + ddy[dir];

			/* Hack -- Use an actual "target" */
			if ((dir == 5) && target_okay(Ind))
			{
				tx = p_ptr->target_col;
				ty = p_ptr->target_row;
			
                                if (distance(ty, tx, p_ptr->py, p_ptr->px) > 1)
				{
                                        msg_print(Ind, "You can not dig that far.");
					return;
				}
			}
		
                        msg_format_near(Ind, "%s hacks at the wall.", p_ptr->name);
                        project(0 - Ind, 0, p_ptr->dun_depth, ty, tx, 1, GF_KILL_WALL, PROJECT_BEAM | PROJECT_GRID);
			break;
		}
		default:  /* For some reason we got called for a technic that */
		{         /* doesn't require a direction */
			msg_print(Ind, "SERVER ERROR: do_cmd_fight_aux() called for non-directional technic!");
			p_ptr->current_spell = -1;
			return;
		}
	}	

	if (!((p_ptr->current_spell < 32) ?
		(p_ptr->spell_worked1 & (1L << p_ptr->current_spell)) :
		(p_ptr->spell_worked2 & (1L << (p_ptr->current_spell - 32)))))
	{
		int e = s_ptr->sexp;

		if (p_ptr->current_spell < 32)
		{
			p_ptr->spell_worked1 |= (1L << p_ptr->current_spell);
		}
		else
		{
			p_ptr->spell_worked2 |= (1L << (p_ptr->current_spell - 32));
		}

		gain_exp(Ind, e * s_ptr->slevel);

		/* Fix the spell info */
		p_ptr->window |= PW_SPELL;
	}

	p_ptr->energy -= level_speed(p_ptr->dun_depth);

	if (s_ptr->smana <= p_ptr->csp)
	{
		/* Use some mana */
		p_ptr->csp -= s_ptr->smana;
	}

	/* Over-exert the player */
	else
	{
		int oops = s_ptr->smana - p_ptr->csp;

		/* No mana left */
		p_ptr->csp = 0;
		p_ptr->csp_frac = 0;

		/* Message */
		msg_print(Ind, "You faint from the effort!");

		/* Hack -- bypass free action */
		(void)set_paralyzed(Ind, p_ptr->paralyzed + randint(5 * oops + 1));

		/* Damage CON (possibly permanently) */
		if (rand_int(100) < 50)
		{
			bool perm = (rand_int(100) < 25);

			/* Message */
			msg_print(Ind, "You have damaged your health!");

			/* Reduce constitution */
			(void)dec_stat(Ind, A_CON, 15 + randint(10), perm);
		}
	}

	/* Reset current spell */
	p_ptr->current_spell = -1;

	/* Resend mana */
	p_ptr->redraw |= (PR_MANA);

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);
}

/*
 * Cast a shadow spell
 */
void do_cmd_shad(int Ind, int book, int spell)
{
	player_type *p_ptr = Players[Ind];

	int			i, j, sval;
	int			chance, beam;
	int			plev = p_ptr->lev;

	object_type		*o_ptr;

	magic_type		*s_ptr;

	byte spells[64], num = 0;

	bool antifail = FALSE;

	/* No anti-magic shields around ? */
	if (check_antimagic(Ind)) antifail = TRUE;

	/* Require spell ability */
	if (p_ptr->mp_ptr->spell_book != TV_SHADOW_BOOK)
	{
		msg_print(Ind, "You cannot cast spells!");
		return;
	}

	/* Require lite */
	if (p_ptr->blind || no_lite(Ind))
	{
		msg_print(Ind, "You cannot see!");
		return;
	}

	/* Not when confused */
	if (p_ptr->confused)
	{
		msg_print(Ind, "You are too confused!");
		return;
	}


	/* Restrict choices to spell books */
	item_tester_tval = p_ptr->mp_ptr->spell_book;

	/* Get the item (in the pack) */
	if (book >= 0)
	{
		o_ptr = &p_ptr->inventory[book];
	}

	/* Get the item (on the floor) */
	else
	{
		o_ptr = &o_list[0 - book];
	}

	if (o_ptr->tval != p_ptr->mp_ptr->spell_book)
	{
		msg_print(Ind, "SERVER ERROR: Tried to cast spell from bad book!");
		return;
	}

        if( check_guard_inscription( o_ptr->note, 'm' )) {
                msg_print(Ind, "The item's inscription prevents it");
                return;
        };

	/* Access the item's sval */
	sval = o_ptr->sval;

	for (i = 0; i < 64; i++)
	{
		/* Check for this spell */
		if ((i < 32) ?
			(spell_flags[p_ptr->mp_ptr->spell_type][sval][0] & (1L << i)) :
			(spell_flags[p_ptr->mp_ptr->spell_type][sval][1] & (1L << (i - 32))))
		{
			/* Collect this spell */
			spells[num++] = i;
		}
	}

	/* Set the spell number */
	j = spells[spell];

	if (!spell_okay(Ind, j, 1))
	{
		msg_print(Ind, "You cannot cast that spell.");
		return;
	}

	/* Access the spell */
	s_ptr = &p_ptr->mp_ptr->info[j];

	/* Check mana */
	if (s_ptr->smana > p_ptr->csp)
	{
		msg_print(Ind, "You do not have enough mana.");
		return;
	}

	/* Spell failure chance */
	chance = spell_chance(Ind, s_ptr);

	/* Failed spell */
	if ((rand_int(100) < chance) || antifail)
	{
		if (flush_failure) flush();
		msg_print(Ind, "You failed to get the spell off!");
	}

	/* Process spell */
	else
	{
		/* Hack -- chance of "beam" instead of "bolt" */
		beam = plev / 2;

		/* Spells.  */
		switch (j)
		{
			case 0: /* Blink */
			{
				teleport_player(Ind, 10);
				break;
			}
			case 1: /* Stink cloud */
			{
				p_ptr->current_spell = j;
				get_aim_dir(Ind);
				return;
			}
			case 2: /* Light */
			{
				msg_format_near(Ind, "%s calls light.", p_ptr->name);
				(void)lite_area(Ind, damroll(2, (plev / 2)), (plev / 10) + 1);
				break;
			}
			case 3: /* M detect */
			{
				detect_creatures(Ind);
				break;
			}
			case 4: /* O detect */
			{
				detect_object(Ind);
				break;
			}
			case 5: /* T/D detect */
			{
				detect_trap(Ind);
				detect_sdoor(Ind);
				break;
			}
			case 6: /* Treasure detect */
			{
				detect_treasure(Ind);
				break;
			}
			case 7: /* T/D dest */
			{
				destroy_doors_touch(Ind);
				break;
			}
		
			case 8: /* create doors */
			{
				door_creation(Ind);
				break;
			}
			case 9: /* recharge */
			{
				recharge(Ind, 30 + plev * 3);
				break;
			}
			case 10: /* id */
			{
				ident_spell(Ind);
				break;
			}
			case 11: /* telep */
			{
				teleport_player(Ind, 200);
				break;
			}
			case 12: /* deep nights */
			{
				wiz_dark(Ind);
				break;
			}
			case 13: /* invis */
			{
				set_invis(Ind, p_ptr->tim_invisibility + 20 + randint(20) + plev, plev * 8 / 6);
				break;
			}
			case 14: /* create food */
			{
				(void)set_food(Ind, PY_FOOD_MAX - 1);
				break;
			}
			case 15: /* create stairs */
			{
				stair_creation(Ind);
				break;
			}		
			case 16: /* disarm beam */
			{
				p_ptr->current_spell = j;
				get_aim_dir(Ind);
				return;
			}
			
			case 18: /* Cloak of Changement */
			{
				int what;
				int i, tries = 200;
				
				while(tries--)
				{
					player_type *q_ptr;
				
					/* 1 < i < NumPlayers */
					i = randint(NumPlayers);
					
					q_ptr = Players[i];

					/* Disguising into a rogue is .. mhh ... stupid */
					if (q_ptr->pclass == CLASS_ROGUE) continue;
					
					/* Ok we found a good class lets mimic */
					what = q_ptr->pclass;
					break;
				}
				
				/* Arg nothing .. bah be a warrior */
				if (!tries) what = CLASS_WARRIOR;
			
				set_mimic(Ind, p_ptr->tim_mimic + 20 + randint(20) + plev, what);
				break;
			}
			
		case 19: /* trap craetion */
		  {
		    trap_creation(Ind);
		  }
			case 20: /* fear monsters */
			{
				(void)fear_monsters(Ind);
				break;
			}
			case 21: /* sleep monsters */
			{
				(void)sleep_monsters(Ind);
				break;
			}
		
			case 22: /* stun monsters */
			{
				(void)stun_monsters(Ind);
				break;
			}
			
		case 23: /* Summon Foes */
		  {
		    int k;

		    msg_format_near(Ind, "%s summons some monsters!", p_ptr->name);

		    for (k = 0; k < (p_ptr->lev / 10); k++)
		      {
			summon_specific(p_ptr->dun_depth, p_ptr->py, p_ptr->px, p_ptr->dun_depth, 0);
		      }
		  }
			
			case 24: /* banish curses */
			{
				remove_curse(Ind);
				break;
			}
			case 25: /* curse */
			{
//			
				break;
			}
			case 26: /* id ball */
			{
				fire_ball(Ind, GF_IDENTIFY, 0, 1, 2 + (plev / 40));
				break;
			}
			case 27: /* ench weap */
			{
				(void)enchant_spell(Ind, rand_int(4) + 1, rand_int(4) + 1, 0);
				break;
			}
			case 28: /* ench arm */
			{
				(void)enchant_spell(Ind, 0, 0, rand_int(3) + 2);
				break;
			}
			case 29: /* *banish curse* */
			{
				remove_all_curse(Ind);
				break;
			}		
			case 30: /* *id* */
			{
				identify_fully(Ind);
				break;
			}
			
			case 32: /* telep lvl */				
			{
				teleport_player_level(Ind);
				break;
			}
			case 33: /* tele to */
			{
				get_aim_dir(Ind);
				p_ptr->current_spell = j;
				return;
			}
			case 34: /* recall */
			{
				if (!p_ptr->word_recall)
				{
					set_recall_depth(p_ptr, o_ptr);
					p_ptr->word_recall = rand_int(20) + 15;
					msg_print(Ind, "The air about you becomes charged...");
				}
				else
				{
					p_ptr->word_recall = 0;
					msg_print(Ind, "A tension leaves the air around you...");
				}
				break;
			}
		
			case 35: /* f recall */
			{
				if (!p_ptr->word_recall)
				{
					set_recall_depth(p_ptr, o_ptr);
					p_ptr->word_recall = rand_int(5) + 1;
					msg_print(Ind, "The air about you becomes charged...");
				}
				else
				{
					p_ptr->word_recall = 0;
					msg_print(Ind, "A tension leaves the air around you...");
				}
				break;
			}
			
			case 40: /* armor of night */
			{
				set_shield(Ind, p_ptr->shield + randint(20) + 20 + plev);
				break;
			}
			case 41: /* day of misrule */
			{
				int i = randint(30) + 30;
				cptr p = ((!p_ptr->male) ? "Daughters" : "Sons");
				msg_format(Ind, "%s of Night rejoice!  It's the Day of Misrule!", p);
				(void)set_fast(Ind, i);
				(void)set_shero(Ind, i);
				break;
			}
			case 42: /* inner sight */
			{
				map_area(Ind);
				break;
			}
			case 43: /* avoid traps */
			{
				set_tim_traps(Ind, p_ptr->tim_traps + 30 + randint(20) + plev);
				break;
			}
		}

		/* A spell was cast */
		if (!((j < 32) ?
		      (p_ptr->spell_worked1 & (1L << j)) :
		      (p_ptr->spell_worked2 & (1L << (j - 32)))))
		{
			int e = s_ptr->sexp;

			/* The spell worked */
			if (j < 32)
			{
				p_ptr->spell_worked1 |= (1L << j);
			}
			else
			{
				p_ptr->spell_worked2 |= (1L << (j - 32));
			}

			/* Gain experience */
			gain_exp(Ind, e * s_ptr->slevel);

			/* Fix the spell info */
			p_ptr->window |= PW_SPELL;
		}
	}

	/* Take a turn */
	p_ptr->energy -= level_speed(p_ptr->dun_depth) / p_ptr->num_spell;

	/* Sufficient mana */
	if (s_ptr->smana <= p_ptr->csp)
	{
		/* Use some mana */
		p_ptr->csp -= s_ptr->smana;
	}

	/* Over-exert the player */
	else
	{
		int oops = s_ptr->smana - p_ptr->csp;

		/* No mana left */
		p_ptr->csp = 0;
		p_ptr->csp_frac = 0;

		/* Message */
		msg_print(Ind, "You faint from the effort!");

		/* Hack -- Bypass free action */
		(void)set_paralyzed(Ind, p_ptr->paralyzed + randint(5 * oops + 1));

		/* Damage CON (possibly permanently) */
		if (rand_int(100) < 50)
		{
			bool perm = (rand_int(100) < 25);

			/* Message */
			msg_print(Ind, "You have damaged your health!");

			/* Reduce constitution */
			(void)dec_stat(Ind, A_CON, 15 + randint(10), perm);
		}
	}

	/* Redraw mana */
	p_ptr->redraw |= (PR_MANA);

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);
}


/*
 * Cast a hunt spell
 */
void do_cmd_hunt(int Ind, int book, int spell)
{
	player_type *p_ptr = Players[Ind];

	int			i, j, sval;
	int			chance, beam;
	int			plev = p_ptr->lev;

	object_type		*o_ptr;

	magic_type		*s_ptr;

	byte spells[64], num = 0;

	bool antifail = FALSE;

	/* No anti-magic shields around ? */
	if (check_antimagic(Ind)) antifail = TRUE;

	/* Require spell ability */
	if (p_ptr->mp_ptr->spell_book != TV_HUNT_BOOK)
	{
		msg_print(Ind, "You cannot cast spells!");
		return;
	}

	/* Require lite */
	if (p_ptr->blind || no_lite(Ind))
	{
		msg_print(Ind, "You cannot see!");
		return;
	}

	/* Not when confused */
	if (p_ptr->confused)
	{
		msg_print(Ind, "You are too confused!");
		return;
	}


	/* Restrict choices to spell books */
	item_tester_tval = p_ptr->mp_ptr->spell_book;

	/* Get the item (in the pack) */
	if (book >= 0)
	{
		o_ptr = &p_ptr->inventory[book];
	}

	/* Get the item (on the floor) */
	else
	{
		o_ptr = &o_list[0 - book];
	}

	if (o_ptr->tval != p_ptr->mp_ptr->spell_book)
	{
		msg_print(Ind, "SERVER ERROR: Tried to cast spell from bad book!");
		return;
	}

        if( check_guard_inscription( o_ptr->note, 'm' )) {
                msg_print(Ind, "The item's inscription prevents it");
                return;
        };

	/* Access the item's sval */
	sval = o_ptr->sval;

	for (i = 0; i < 64; i++)
	{
		/* Check for this spell */
		if ((i < 32) ?
			(spell_flags[p_ptr->mp_ptr->spell_type][sval][0] & (1L << i)) :
			(spell_flags[p_ptr->mp_ptr->spell_type][sval][1] & (1L << (i - 32))))
		{
			/* Collect this spell */
			spells[num++] = i;
		}
	}

	/* Set the spell number */
	j = spells[spell];

	if (!spell_okay(Ind, j, 1))
	{
		msg_print(Ind, "You cannot cast that spell.");
		return;
	}

	/* Access the spell */
	s_ptr = &p_ptr->mp_ptr->info[j];

	/* Check mana */
	if (s_ptr->smana > p_ptr->csp)
	{
		msg_print(Ind, "You do not have enough mana.");
		return;
	}

	/* Spell failure chance */
	chance = spell_chance(Ind, s_ptr);

	/* Failed spell */
	if ((rand_int(100) < chance) || antifail)
	{
		if (flush_failure) flush();
		msg_print(Ind, "You failed to get the spell off!");
	}

	/* Process spell */
	else
	{
		/* Hack -- chance of "beam" instead of "bolt" */
		beam = plev / 2;

		/* Spells.  */
		switch (j)
		{
			case 0: /* Elec Shots */
			{
			  if (p_ptr->bow_brand == BOW_BRAND_ELEC)
			    set_bow_brand(Ind, p_ptr->bow_brand + plev, BOW_BRAND_ELEC, 3);
			  else
			    {
			      p_ptr->bow_brand = 0;
			      set_bow_brand(Ind, plev + randint(20), BOW_BRAND_ELEC, 3);
			    }
			  break;
			}
			case 1: /* Sense */
			{
				(void)detect_creatures(Ind);
				break;
			}
			case 2: /* Cold Shots */
			{
			  if (p_ptr->bow_brand == BOW_BRAND_COLD)
			    set_bow_brand(Ind, p_ptr->bow_brand + plev, BOW_BRAND_COLD, 5);
			  else
			    {
			      p_ptr->bow_brand = 0;
			      set_bow_brand(Ind, plev + randint(20), BOW_BRAND_COLD, 5);
			    }
			  break;
			}
			case 3: /* Create Light */
			{
			  (void)lite_area(Ind, damroll(2, (plev / 2)), (plev / 10) + 1);
				break;
			}
			case 4: /* Fire Shots */
			{
			  if (p_ptr->bow_brand == BOW_BRAND_FIRE)
			    set_bow_brand(Ind, p_ptr->bow_brand + plev, BOW_BRAND_FIRE, 7);
			  else
			    {
			      p_ptr->bow_brand = 0;
			      set_bow_brand(Ind, plev + randint(20), BOW_BRAND_FIRE, 7);
			    }
			  break;
			}

			case 8: /* Acid Shots */
			{
			  if (p_ptr->bow_brand == BOW_BRAND_ACID)
			    set_bow_brand(Ind, p_ptr->bow_brand + plev, BOW_BRAND_ACID, 8);
			  else
			    {
			      p_ptr->bow_brand = 0;
			      set_bow_brand(Ind, plev + randint(20), BOW_BRAND_ACID, 8);
			    }
			  break;
			}
			case 9: /*  */
			{
				break;
			}
			case 10: /* Conf Shots */
			{
			  if (p_ptr->bow_brand == BOW_BRAND_CONF)
			    set_bow_brand(Ind, p_ptr->bow_brand + plev, BOW_BRAND_CONF, 5);
			  else
			    {
			      p_ptr->bow_brand = 0;
			      set_bow_brand(Ind, plev + randint(20), BOW_BRAND_CONF, 5);
			    }
			  break;
			}
			case 11: /* Pois Shots */
			{
			  if (p_ptr->bow_brand == BOW_BRAND_POIS)
			    set_bow_brand(Ind, p_ptr->bow_brand + plev, BOW_BRAND_POIS, 11);
			  else
			    {
			      p_ptr->bow_brand = 0;
			      set_bow_brand(Ind, plev + randint(20), BOW_BRAND_POIS, 11);
			    }
			  break;
			}
			case 12: /* Sharp Shots */
			{
			  if (p_ptr->bow_brand == BOW_BRAND_SHARP)
			    set_bow_brand(Ind, p_ptr->bow_brand + plev, BOW_BRAND_SHARP, 1);
			  else
			    {
			      p_ptr->bow_brand = 0;
			      set_bow_brand(Ind, plev + randint(20), BOW_BRAND_SHARP, 1);
			    }
			  break;
			}

			case 16: /*  */
			{
				break;
			}
			case 17: /*  */
			{
				break;
			}
			case 18: /*  */
			{
				break;
			}
			case 19: /*  */
			{
				break;
			}
			case 20: /* Power Shots */
			{
			  if (p_ptr->bow_brand == BOW_BRAND_MANA)
			    set_bow_brand(Ind, p_ptr->bow_brand + plev, BOW_BRAND_MANA, 25);
			  else
			    {
			      p_ptr->bow_brand = 0;
			      set_bow_brand(Ind, plev + randint(20), BOW_BRAND_MANA, 25);
			    }
			  break;
				break;
			}
		}

		/* A spell was cast */
		if (!((j < 32) ?
		      (p_ptr->spell_worked1 & (1L << j)) :
		      (p_ptr->spell_worked2 & (1L << (j - 32)))))
		{
			int e = s_ptr->sexp;

			/* The spell worked */
			if (j < 32)
			{
				p_ptr->spell_worked1 |= (1L << j);
			}
			else
			{
				p_ptr->spell_worked2 |= (1L << (j - 32));
			}

			/* Gain experience */
			gain_exp(Ind, e * s_ptr->slevel);

			/* Fix the spell info */
			p_ptr->window |= PW_SPELL;
		}
	}

	/* Take a turn */
	p_ptr->energy -= level_speed(p_ptr->dun_depth) / p_ptr->num_spell;

	/* Sufficient mana */
	if (s_ptr->smana <= p_ptr->csp)
	{
		/* Use some mana */
		p_ptr->csp -= s_ptr->smana;
	}

	/* Over-exert the player */
	else
	{
		int oops = s_ptr->smana - p_ptr->csp;

		/* No mana left */
		p_ptr->csp = 0;
		p_ptr->csp_frac = 0;

		/* Message */
		msg_print(Ind, "You faint from the effort!");

		/* Hack -- Bypass free action */
		(void)set_paralyzed(Ind, p_ptr->paralyzed + randint(5 * oops + 1));

		/* Damage CON (possibly permanently) */
		if (rand_int(100) < 50)
		{
			bool perm = (rand_int(100) < 25);

			/* Message */
			msg_print(Ind, "You have damaged your health!");

			/* Reduce constitution */
			(void)dec_stat(Ind, A_CON, 15 + randint(10), perm);
		}
	}

	/* Redraw mana */
	p_ptr->redraw |= (PR_MANA);

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);
}


/*
 * Finish casting a spell that required a direction --KLJ--
 */
void do_cmd_shad_aux(int Ind, int dir)
{
	player_type *p_ptr = Players[Ind];

	int plev = p_ptr->lev;
	int beam = ((p_ptr->pclass == 1) ? plev : (plev / 2));

	magic_type *s_ptr = &p_ptr->mp_ptr->info[p_ptr->current_spell];

	/* Only fire in direction 5 if we have a target */
	if ((dir == 5) && !target_okay(Ind))
	{
		/* Reset current spell */
		p_ptr->current_spell = -1;

		/* Done */
		return;
	}

	/* We assume that the spell can be cast, and so forth */
	switch(p_ptr->current_spell)
	{
		case 1:
			msg_format_near(Ind, "%s fires a stinking cloud.", p_ptr->name);
			fire_ball(Ind, GF_POIS, dir, 8 + (plev / 2), 2);
			break;
		case 16:
			disarm_trap(Ind, dir);
			break;
		case 33:
		{
			int ty, tx, rad = 10 + (plev / 5);
		
			/* Hack -- Use an actual "target" */
			if ((dir == 5) && target_okay(Ind))
			{
				tx = p_ptr->target_col;
				ty = p_ptr->target_row;
			
				if (distance(ty, tx, p_ptr->py, p_ptr->px) > rad)
				{
					msg_print(Ind, "You can not blink that far.");
					return;
				}
			}
			else
			{
				msg_print(Ind, "You must have a target.");
				return;
			}
		
			teleport_player_to(Ind, ty, tx);
			break;
		}
		default:  /* For some reason we got called for a spell that */
		{         /* doesn't require a direction */
			msg_print(Ind, "SERVER ERROR: do_cmd_shad_aux() called for non-directional spell!");
			p_ptr->current_spell = -1;
			return;
		}
	}	

	if (!((p_ptr->current_spell < 32) ?
		(p_ptr->spell_worked1 & (1L << p_ptr->current_spell)) :
		(p_ptr->spell_worked2 & (1L << (p_ptr->current_spell - 32)))))
	{
		int e = s_ptr->sexp;

		if (p_ptr->current_spell < 32)
		{
			p_ptr->spell_worked1 |= (1L << p_ptr->current_spell);
		}
		else
		{
			p_ptr->spell_worked2 |= (1L << (p_ptr->current_spell - 32));
		}

		gain_exp(Ind, e * s_ptr->slevel);

		/* Fix the spell info */
		p_ptr->window |= PW_SPELL;
	}

	p_ptr->energy -= level_speed(p_ptr->dun_depth) / p_ptr->num_spell;

	if (s_ptr->smana <= p_ptr->csp)
	{
		/* Use some mana */
		p_ptr->csp -= s_ptr->smana;
	}

	/* Over-exert the player */
	else
	{
		int oops = s_ptr->smana - p_ptr->csp;

		/* No mana left */
		p_ptr->csp = 0;
		p_ptr->csp_frac = 0;

		/* Message */
		msg_print(Ind, "You faint from the effort!");

		/* Hack -- bypass free action */
		(void)set_paralyzed(Ind, p_ptr->paralyzed + randint(5 * oops + 1));

		/* Damage CON (possibly permanently) */
		if (rand_int(100) < 50)
		{
			bool perm = (rand_int(100) < 25);

			/* Message */
			msg_print(Ind, "You have damaged your health!");

			/* Reduce constitution */
			(void)dec_stat(Ind, A_CON, 15 + randint(10), perm);
		}
	}

	/* Reset current spell */
	p_ptr->current_spell = -1;

	/* Resend mana */
	p_ptr->redraw |= (PR_MANA);

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);
}

static void do_mimic_power(int Ind, int power)
{
  player_type *p_ptr = Players[Ind];
  monster_race *r_ptr = &r_info[p_ptr->body_monster];
  int rlev = r_ptr->level,
      plev = p_ptr->lev;
  int j, chance;
  magic_type *s_ptr = &innate_powers[power];

  j = power;

	/* Check mana */
	if (s_ptr->smana > p_ptr->csp)
	{
		msg_print(Ind, "You do not have enough mana.");
		return;
	}

	/* Spell failure chance */
	chance = spell_chance(Ind, s_ptr);

	/* Failed spell */
	if (rand_int(100) < chance)
	{
		if (flush_failure) flush();
		msg_print(Ind, "You failed to use the power!");
	}
	else
	{

  /* 0-31 = RF4, 32-63 = RF5, 64-95 = RF6 */
  switch(j)
    {
    case 0:
      msg_print(Ind, "You emit a high pitched humming noise.");
      aggravate_monsters(Ind, 1);
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
//#define RF4_ARROW_1			0x00000010	/* Fire an arrow (light) */
    case 4:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
//#define RF4_ARROW_2			0x00000020	/* Fire an arrow (heavy) */
    case 5:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
//#define RF4_ARROW_3			0x00000040	/* Fire missiles (light) */
    case 6:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
//#define RF4_ARROW_4			0x00000080	/* Fire missiles (heavy) */
    case 7:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
//#define RF4_BR_ACID			0x00000100	/* Breathe Acid */
    case 8:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
//#define RF4_BR_ELEC			0x00000200	/* Breathe Elec */
    case 9:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
//#define RF4_BR_FIRE			0x00000400	/* Breathe Fire */
    case 10:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
//#define RF4_BR_COLD			0x00000800	/* Breathe Cold */
    case 11:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
//#define RF4_BR_POIS			0x00001000	/* Breathe Poison */
    case 12:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
//#define RF4_BR_NETH			0x00002000	/* Breathe Nether */
    case 13:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
//#define RF4_BR_LITE			0x00004000	/* Breathe Lite */
    case 14:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
//#define RF4_BR_DARK			0x00008000	/* Breathe Dark */
    case 15:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
//#define RF4_BR_CONF			0x00010000	/* Breathe Confusion */
    case 16:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
//#define RF4_BR_SOUN			0x00020000	/* Breathe Sound */
    case 17:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
//#define RF4_BR_CHAO			0x00040000	/* Breathe Chaos */
    case 18:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
//#define RF4_BR_DISE			0x00080000	/* Breathe Disenchant */
    case 19:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
//#define RF4_BR_NEXU			0x00100000	/* Breathe Nexus */
    case 20:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
//#define RF4_BR_TIME			0x00200000	/* Breathe Time */
    case 21:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
//#define RF4_BR_INER			0x00400000	/* Breathe Inertia */
    case 22:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
//#define RF4_BR_GRAV			0x00800000	/* Breathe Gravity */
    case 23:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
//#define RF4_BR_SHAR			0x01000000	/* Breathe Shards */
    case 24:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
//#define RF4_BR_PLAS			0x02000000	/* Breathe Plasma */
    case 25:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
//#define RF4_BR_WALL			0x04000000	/* Breathe Force */
    case 26:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
//#define RF4_BR_MANA			0x08000000	/* Breathe Mana */
    case 27:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
//#define RF4_XXX5			0x10000000
    case 28:
      break;
//#define RF4_XXX6			0x20000000
    case 29:
      break;
//#define RF4_XXX7			0x40000000
    case 30:
      break;
// #define RF4_XXX8			0x80000000
    case 31:
      break;

/* RF5 */

// RF5_BA_ACID			0x00000001	/* Acid Ball */
    case 32:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_BA_ELEC			0x00000002	/* Elec Ball */
    case 33:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_BA_FIRE			0x00000004	/* Fire Ball */
    case 34:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_BA_COLD			0x00000008	/* Cold Ball */
    case 35:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_BA_POIS			0x00000010	/* Poison Ball */
    case 36:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_BA_NETH			0x00000020	/* Nether Ball */
    case 37:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_BA_WATE			0x00000040	/* Water Ball */
    case 38:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_BA_MANA			0x00000080	/* Mana Storm */
    case 39:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_BA_DARK			0x00000100	/* Darkness Storm */
    case 40:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_DRAIN_MANA		0x00000200	/* Drain Mana */
    case 41:
      msg_print(Ind, "Haha, you wish ... :)");
      break;
// RF5_MIND_BLAST		0x00000400	/* Blast Mind */
    case 42:
      msg_print(Ind, "Haha, you wish ... :)");
      break;
// RF5_BRAIN_SMASH		0x00000800	/* Smash Brain */
    case 43:
      msg_print(Ind, "Haha, you wish ... :)");
      break;
// RF5_CAUSE_1			0x00001000	/* Cause Light Wound */
    case 44:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_CAUSE_2			0x00002000	/* Cause Serious Wound */
    case 45:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_CAUSE_3			0x00004000	/* Cause Critical Wound */
    case 46:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_CAUSE_4			0x00008000	/* Cause Mortal Wound */
    case 47:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_BO_ACID			0x00010000	/* Acid Bolt */
    case 48:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_BO_ELEC			0x00020000	/* Elec Bolt (unused) */
    case 49:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_BO_FIRE			0x00040000	/* Fire Bolt */
    case 50:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_BO_COLD			0x00080000	/* Cold Bolt */
    case 51:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_BO_POIS			0x00100000	/* Poison Bolt (unused) */
    case 52:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_BO_NETH			0x00200000	/* Nether Bolt */
    case 53:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_BO_WATE			0x00400000	/* Water Bolt */
    case 54:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_BO_MANA			0x00800000	/* Mana Bolt */
    case 55:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_BO_PLAS			0x01000000	/* Plasma Bolt */
    case 56:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_BO_ICEE			0x02000000	/* Ice Bolt */
    case 57:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_MISSILE			0x04000000	/* Magic Missile */
    case 58:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_SCARE			0x08000000	/* Frighten Player */
    case 59:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_BLIND			0x10000000	/* Blind Player */
    case 60:
      msg_print(Ind, "Haha, you wish ... :)");
      break;
// RF5_CONF			0x20000000	/* Confuse Player */
    case 61:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_SLOW			0x40000000	/* Slow Player */
    case 62:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF5_HOLD			0x80000000	/* Paralyze Player */
    case 63:
      msg_print(Ind, "Haha, you wish ... :)");
      break;

// RF6_HASTE			0x00000001	/* Speed self */
    case 64:
      if(!p_ptr->fast) set_fast(Ind, 10 + (rlev / 2));
      break;
// RF6_XXX1			0x00000002	/* Speed a lot (?) */
    case 65:
      break;
// RF6_HEAL			0x00000004	/* Heal self */
    case 66:
      hp_player(Ind, rlev * 2);
      break;
// RF6_XXX2			0x00000008	/* Heal a lot (?) */
    case 67:
      break;
// RF6_BLINK			0x00000010	/* Teleport Short */
    case 68:
      teleport_player(Ind, 10);
      break;
// RF6_TPORT			0x00000020	/* Teleport Long */
    case 69:
      teleport_player(Ind, 200);
      break;
// RF6_XXX3			0x00000040	/* Move to Player (?) */
    case 70:
      break;
// RF6_XXX4			0x00000080	/* Move to Monster (?) */
    case 71:
      break;
// RF6_TELE_TO			0x00000100	/* Move player to monster */
    case 72:
      msg_print(Ind, "Haha, you wish ... :)");
      break;
// RF6_TELE_AWAY		0x00000200	/* Move player far away */
    case 73:
      get_aim_dir(Ind);
      p_ptr->current_spell = j;
      return;
      break;
// RF6_TELE_LEVEL		0x00000400	/* Move player vertically */
    case 74:
      teleport_player_level(Ind);
      break;
// RF6_XXX5			0x00000800	/* Move player (?) */
    case 75:
      break;
// RF6_DARKNESS		0x00001000	/* Create Darkness */
    case 76:
      msg_print(Ind, "Haha, you wish ... :)");
      break;
// RF6_TRAPS			0x00002000	/* Create Traps */
    case 77:
      msg_print(Ind, "Haha, you wish ... :)");
      break;
// RF6_FORGET			0x00004000	/* Cause amnesia */
    case 78:
      msg_print(Ind, "Haha, you wish ... :)");
      break;

    default:
      msg_format(Ind, "Bad innate power %d.", power);
      break;
    }
	}

    p_ptr->energy -= level_speed(p_ptr->dun_depth);
	if (s_ptr->smana <= p_ptr->csp)
	{
		/* Use some mana */
		p_ptr->csp -= s_ptr->smana;
	}

	/* Over-exert the player */
	else
	{
		int oops = s_ptr->smana - p_ptr->csp;

		/* No mana left */
		p_ptr->csp = 0;
		p_ptr->csp_frac = 0;

		/* Message */
		msg_print(Ind, "You faint from the effort!");

		/* Hack -- bypass free action */
		(void)set_paralyzed(Ind, p_ptr->paralyzed + randint(5 * oops + 1));

		/* Damage CON (possibly permanently) */
		if (rand_int(100) < 50)
		{
			bool perm = (rand_int(100) < 25);

			/* Message */
			msg_print(Ind, "You have damaged your health!");

			/* Reduce constitution */
			(void)dec_stat(Ind, A_CON, 15 + randint(10), perm);
		}
	}
}

/*
 * Finish casting a spell that required a direction --KLJ--
 */
void do_mimic_power_aux(int Ind, int dir)
{
	player_type *p_ptr = Players[Ind];
	monster_race *r_ptr = &r_info[p_ptr->body_monster];
	int rad;
	int plev = p_ptr->lev;
	int rlev = r_ptr->level;
	magic_type *s_ptr = &innate_powers[p_ptr->current_spell];

	/* Determine the radius of the blast */
	rad = (r_ptr->flags2 & RF2_POWERFUL) ? 3 : 2;

	/* Only fire in direction 5 if we have a target */
	if ((dir == 5) && !target_okay(Ind))
	{
		/* Reset current spell */
		p_ptr->current_spell = -1;

		/* Done */
		return;
	}

	/* We assume that the spell can be cast, and so forth */
	switch(p_ptr->current_spell)
	{
//#define RF4_ARROW_1			0x00000010	/* Fire an arrow (light) */
    case 4:
      fire_bolt(Ind, GF_ARROW, dir, damroll(1, 6));
      break;
//#define RF4_ARROW_2			0x00000020	/* Fire an arrow (heavy) */
    case 5:
      fire_bolt(Ind, GF_ARROW, dir, damroll(3, 6));
      break;
//#define RF4_ARROW_3			0x00000040	/* Fire missiles (light) */
    case 6:
      fire_bolt(Ind, GF_ARROW, dir, damroll(5, 6));
      break;
//#define RF4_ARROW_4			0x00000080	/* Fire missiles (heavy) */
    case 7:
      fire_bolt(Ind, GF_ARROW, dir, damroll(7, 6));
      break;
//#define RF4_BR_ACID			0x00000100	/* Breathe Acid */
    case 8:
      fire_ball(Ind, GF_ACID, dir, ((p_ptr->chp / 2) > 500) ? 500 : (p_ptr->chp / 2), rad);
      break;
//#define RF4_BR_ELEC			0x00000200	/* Breathe Elec */
    case 9:
      fire_ball(Ind, GF_ELEC, dir, ((p_ptr->chp / 2) > 500) ? 500 : (p_ptr->chp / 2) , rad);
      break;
//#define RF4_BR_FIRE			0x00000400	/* Breathe Fire */
    case 10:
      fire_ball(Ind, GF_FIRE, dir, ((p_ptr->chp / 2) > 500) ? 500 : (p_ptr->chp / 2), rad );
      break;
//#define RF4_BR_COLD			0x00000800	/* Breathe Cold */
    case 11:
      fire_ball(Ind, GF_COLD, dir, ((p_ptr->chp / 2) > 500) ? 500 : (p_ptr->chp / 2), rad );
      break;
//#define RF4_BR_POIS			0x00001000	/* Breathe Poison */
    case 12:
      fire_ball(Ind, GF_POIS, dir, ((p_ptr->chp / 2) > 500) ? 500 : (p_ptr->chp / 2), rad );
      break;
//#define RF4_BR_NETH			0x00002000	/* Breathe Nether */
    case 13:
      fire_ball(Ind, GF_NETHER, dir, ((p_ptr->chp / 3) > 500) ? 500 : (p_ptr->chp / 3), rad );
      break;
//#define RF4_BR_LITE			0x00004000	/* Breathe Lite */
    case 14:
      fire_ball(Ind, GF_LITE, dir, ((p_ptr->chp / 3) > 400) ? 400 : (p_ptr->chp / 3), rad );
      break;
//#define RF4_BR_DARK			0x00008000	/* Breathe Dark */
    case 15:
      fire_ball(Ind, GF_DARK, dir, ((p_ptr->chp / 3) > 400) ? 400 : (p_ptr->chp / 3) , rad);
      break;
//#define RF4_BR_CONF			0x00010000	/* Breathe Confusion */
    case 16:
      fire_ball(Ind, GF_CONFUSION, dir, ((p_ptr->chp / 3) > 400) ? 400 : (p_ptr->chp / 3), rad );
      break;
//#define RF4_BR_SOUN			0x00020000	/* Breathe Sound */
    case 17:
      fire_ball(Ind, GF_SOUND, dir, ((p_ptr->chp / 4) > 350) ? 350 : (p_ptr->chp / 4), rad );
      break;
//#define RF4_BR_CHAO			0x00040000	/* Breathe Chaos */
    case 18:
      fire_ball(Ind, GF_CHAOS, dir, ((p_ptr->chp / 4) > 500) ? 500 : (p_ptr->chp / 4), rad );
      break;
//#define RF4_BR_DISE			0x00080000	/* Breathe Disenchant */
    case 19:
      fire_ball(Ind, GF_DISENCHANT, dir, ((p_ptr->chp / 4) > 400) ? 400 : (p_ptr->chp / 4), rad );
      break;
//#define RF4_BR_NEXU			0x00100000	/* Breathe Nexus */
    case 20:
      fire_ball(Ind, GF_NEXUS, dir, ((p_ptr->chp / 3) > 250) ? 250 : (p_ptr->chp / 3), rad );
      break;
//#define RF4_BR_TIME			0x00200000	/* Breathe Time */
    case 21:
      fire_ball(Ind, GF_TIME, dir, ((p_ptr->chp / 4) > 200) ? 200 : (p_ptr->chp / 4), rad );
      break;
//#define RF4_BR_INER			0x00400000	/* Breathe Inertia */
    case 22:
      fire_ball(Ind, GF_INERTIA, dir, ((p_ptr->chp / 4) > 200) ? 200 : (p_ptr->chp / 4) , rad);
      break;
//#define RF4_BR_GRAV			0x00800000	/* Breathe Gravity */
    case 23:
      fire_ball(Ind, GF_GRAVITY, dir, ((p_ptr->chp / 3) > 200) ? 200 : (p_ptr->chp / 3) , rad);
      break;
//#define RF4_BR_SHAR			0x01000000	/* Breathe Shards */
    case 24:
      fire_ball(Ind, GF_SHARDS, dir, ((p_ptr->chp / 4) > 300) ? 300 : (p_ptr->chp / 4) , rad);
      break;
//#define RF4_BR_PLAS			0x02000000	/* Breathe Plasma */
    case 25:
      fire_ball(Ind, GF_PLASMA, dir, ((p_ptr->chp / 4) > 150) ? 150 : (p_ptr->chp / 4) , rad);
      break;
//#define RF4_BR_WALL			0x04000000	/* Breathe Force */
    case 26:
      fire_ball(Ind, GF_FORCE, dir, ((p_ptr->chp / 4) > 200) ? 200 : (p_ptr->chp / 4) , rad);
      break;
//#define RF4_BR_MANA			0x08000000	/* Breathe Mana */
    case 27:
      fire_ball(Ind, GF_MANA, dir, ((p_ptr->chp / 3) > 500) ? 500 : (p_ptr->chp / 3) , rad);
      break;

/* RF5 */

// RF5_BA_ACID			0x00000001	/* Acid Ball */
    case 32:
      fire_ball(Ind, GF_ACID, dir, randint(rlev * 3) + 15 , rad);
      break;
// RF5_BA_ELEC			0x00000002	/* Elec Ball */
    case 33:
      fire_ball(Ind, GF_ELEC, dir, randint(rlev * 3 / 2) + 8 , rad);
      break;
// RF5_BA_FIRE			0x00000004	/* Fire Ball */
    case 34:
      fire_ball(Ind, GF_FIRE, dir, randint(rlev * 7 / 0) + 10 , rad);
      break;
// RF5_BA_COLD			0x00000008	/* Cold Ball */
    case 35:
      fire_ball(Ind, GF_COLD, dir, randint(rlev * 3 / 2) + 10 , rad);
      break;
// RF5_BA_POIS			0x00000010	/* Poison Ball */
    case 36:
      fire_ball(Ind, GF_POIS, dir, damroll(12, 2) , rad);
      break;
// RF5_BA_NETH			0x00000020	/* Nether Ball */
    case 37:
      fire_ball(Ind, GF_NETHER, dir, 50  + damroll(10, 10) + rlev , rad);
      break;
// RF5_BA_WATE			0x00000040	/* Water Ball */
    case 38:
      fire_ball(Ind, GF_WATER, dir, randint(rlev * 5 / 2) + 50 , rad);
      break;
// RF5_BA_MANA			0x00000080	/* Mana Storm */
    case 39:
      fire_ball(Ind, GF_MANA, dir, damroll(10, 10) + (rlev * 2) , rad);
      break;
// RF5_BA_DARK			0x00000100	/* Darkness Storm */
    case 40:
      fire_ball(Ind, GF_DARK, dir, damroll(10, 10) + (rlev * 2) , rad);
      break;
// RF5_CAUSE_1			0x00001000	/* Cause Light Wound */
    case 44:
      fire_bolt(Ind, GF_MANA, dir, damroll(3, 8));
      break;
// RF5_CAUSE_2			0x00002000	/* Cause Serious Wound */
    case 45:
      fire_bolt(Ind, GF_MANA, dir, damroll(8, 8));
      break;
// RF5_CAUSE_3			0x00004000	/* Cause Critical Wound */
    case 46:
      fire_bolt(Ind, GF_MANA, dir, damroll(10, 15));
      break;
// RF5_CAUSE_4			0x00008000	/* Cause Mortal Wound */
    case 47:
      fire_bolt(Ind, GF_MANA, dir, damroll(15, 15));
      break;
// RF5_BO_ACID			0x00010000	/* Acid Bolt */
    case 48:
      fire_bolt(Ind, GF_ACID, dir, damroll(7, 8) + (rlev / 3));
      break;
// RF5_BO_ELEC			0x00020000	/* Elec Bolt (unused) */
    case 49:
      fire_bolt(Ind, GF_ELEC, dir, damroll(4, 8) + (rlev / 3));
      break;
// RF5_BO_FIRE			0x00040000	/* Fire Bolt */
    case 50:
      fire_bolt(Ind, GF_FIRE, dir, damroll(9, 8) + (rlev / 3));
      break;
// RF5_BO_COLD			0x00080000	/* Cold Bolt */
    case 51:
      fire_bolt(Ind, GF_COLD, dir, damroll(6, 8) + (rlev / 3));
      break;
// RF5_BO_POIS			0x00100000	/* Poison Bolt (unused) */
    case 52:
      fire_bolt(Ind, GF_POIS, dir, damroll(3, 8) + (rlev / 3));
      break;
// RF5_BO_NETH			0x00200000	/* Nether Bolt */
    case 53:
      fire_bolt(Ind, GF_NETHER, dir, 30 + damroll(5, 5) + (rlev * 3) / 2);
      break;
// RF5_BO_WATE			0x00400000	/* Water Bolt */
    case 54:
      fire_bolt(Ind, GF_WATER, dir, damroll(10, 10) + (rlev));
      break;
// RF5_BO_MANA			0x00800000	/* Mana Bolt */
    case 55:
      fire_bolt(Ind, GF_MANA, dir, randint(rlev * 7 / 4) + 50);
      break;
// RF5_BO_PLAS			0x01000000	/* Plasma Bolt */
    case 56:
      fire_bolt(Ind, GF_PLASMA, dir, 10 + damroll(8, 7) + (rlev));
      break;
// RF5_BO_ICEE			0x02000000	/* Ice Bolt */
    case 57:
      fire_bolt(Ind, GF_ICE, dir, damroll(6, 6) + (rlev));
      break;
// RF5_MISSILE			0x04000000	/* Magic Missile */
    case 58:
      fire_bolt(Ind, GF_MISSILE, dir, damroll(2, 6) + (rlev / 3));
      break;
// RF5_SCARE			0x08000000	/* Frighten Player */
    case 59:
      break;
// RF5_CONF			0x20000000	/* Confuse Player */
    case 61:
      break;
// RF5_SLOW			0x40000000	/* Slow Player */
    case 62:
      break;
// RF6_TELE_AWAY
	case 73:
          (void)fire_beam(Ind, GF_AWAY_ALL, dir, rlev);
	  break;

		default:  /* For some reason we got called for a spell that */
		{         /* doesn't require a direction */
			msg_format(Ind, "SERVER ERROR: do_mimic_power_aux() called for non-directional power %d!", p_ptr->current_spell);
			p_ptr->current_spell = -1;
			return;
		}
	}	

	p_ptr->energy -= level_speed(p_ptr->dun_depth);

	if (s_ptr->smana <= p_ptr->csp)
	{
		/* Use some mana */
		p_ptr->csp -= s_ptr->smana;
	}

	/* Over-exert the player */
	else
	{
		int oops = s_ptr->smana - p_ptr->csp;

		/* No mana left */
		p_ptr->csp = 0;
		p_ptr->csp_frac = 0;

		/* Message */
		msg_print(Ind, "You faint from the effort!");

		/* Hack -- bypass free action */
		(void)set_paralyzed(Ind, p_ptr->paralyzed + randint(5 * oops + 1));

		/* Damage CON (possibly permanently) */
		if (rand_int(100) < 50)
		{
			bool perm = (rand_int(100) < 25);

			/* Message */
			msg_print(Ind, "You have damaged your health!");

			/* Reduce constitution */
			(void)dec_stat(Ind, A_CON, 15 + randint(10), perm);
		}
	}

	/* Reset current spell */
	p_ptr->current_spell = -1;

	/* Resend mana */
	p_ptr->redraw |= (PR_MANA);

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);
}

void do_mimic_change(int Ind, int r_idx)
{
  player_type *p_ptr = Players[Ind];

  if (r_info[r_idx].level > p_ptr->lev * 2)
    {
      msg_print(Ind, "You're not high level enough to use that shape.");
      return;
    }

  p_ptr->body_monster = r_idx;
  p_ptr->body_changed = TRUE;
	
  msg_format(Ind, "You polymorph into a %s !", r_info[r_idx].name + r_name);
  msg_format_near(Ind, "%s polymorphs a % !", p_ptr->name, r_info[r_idx].name + r_name);

  p_ptr->update |= (PU_BONUS);

  /* Recalculate mana */
  p_ptr->update |= (PU_MANA | PU_HP);

  /* Window stuff */
  p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_PLAYER);
}

void do_cmd_mimic(int Ind, int r_idx, int spell)
{
	player_type *p_ptr = Players[Ind];
	int i;
	
	/* No anti-magic shields around ? */
	if (check_antimagic(Ind))
	  {
	    p_ptr->energy -= level_speed(p_ptr->dun_depth);
	    return;
	  }

	if (!spell)
	  {
	    do_mimic_change(Ind, r_idx);
	    p_ptr->energy -= level_speed(p_ptr->dun_depth);
	  }
	else
	  {
	    do_mimic_power(Ind, spell - 1);
	  }
}


/*
 * Use a psi power
 */
void do_cmd_psi(int Ind, int book, int spell)
{
	player_type *p_ptr = Players[Ind];

	int			i, j, sval;
	int			chance, beam;
	int			plev = p_ptr->lev;
	int Ind2;  /* Ind2 = Dominant(caster), Ind = Dominated */
	player_type *p_ptr2; /* p_ptr = Dominant(caster), p_ptr2 = Dominated */

	object_type		*o_ptr;

	magic_type		*s_ptr;

	byte spells[64], num = 0;

	/* Require spell ability */
	if (p_ptr->mp_ptr->spell_book != TV_PSI_BOOK)
	{
		if (p_ptr->esp_link_type && p_ptr->esp_link && (p_ptr->esp_link_flags & LINKF_MISC))
		  {
		    Ind2 = find_player(p_ptr->esp_link);
		    
		    if (!Ind2)
		      {
			p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_SPELL | PW_PLAYER);
			p_ptr->update |= (PU_BONUS | PU_VIEW | PU_MANA | PU_HP);
			p_ptr->redraw |= (PR_BASIC | PR_EXTRA | PR_MAP);
			msg_print(Ind, "Ending mind link.");
			p_ptr->esp_link = 0;
			p_ptr->esp_link_type = 0;
			p_ptr->esp_link_flags = 0;
		      }
		    else
		      {
			p_ptr2 = p_ptr;
			p_ptr = Players[Ind2];
		      }
		  }
		
		if (!Ind2 || (p_ptr->mp_ptr->spell_book != TV_PSI_BOOK))
		  {
		    msg_print(Ind, "You cannot use psi powers!");
		    return;
		  }
	}
	else
	  {
	    p_ptr2 = p_ptr;
	    Ind2 = Ind;
	  }

	plev = p_ptr->lev;

	/* Not when confused */
	if (p_ptr->confused)
	{
		msg_print(Ind, "You are too confused!");
		return;
	}


	/* Restrict choices to spell books */
	item_tester_tval = p_ptr->mp_ptr->spell_book;

	/* Get the item (in the pack) */
	if (book >= 0)
	{
		o_ptr = &p_ptr->inventory[book];
	}

	/* Get the item (on the floor) */
	else
	{
		o_ptr = &o_list[0 - book];
	}

	if (o_ptr->tval != p_ptr->mp_ptr->spell_book)
	{
		msg_print(Ind, "SERVER ERROR: Tried to use psi power from bad crystal!");
		return;
	}

        if( check_guard_inscription( o_ptr->note, 'm' )) {
                msg_print(Ind, "The item's inscription prevents it");
                return;
        };

	/* Access the item's sval */
	sval = o_ptr->sval;

	for (i = 0; i < 64; i++)
	{
		/* Check for this spell */
		if ((i < 32) ?
			(spell_flags[p_ptr->mp_ptr->spell_type][sval][0] & (1L << i)) :
			(spell_flags[p_ptr->mp_ptr->spell_type][sval][1] & (1L << (i - 32))))
		{
			/* Collect this spell */
			spells[num++] = i;
		}
	}

	/* Set the spell number */
	j = spells[spell];

	if (!spell_okay(Ind2, j, 1))
	{
		msg_print(Ind, "You cannot use that power.");
		return;
	}

	/* Access the spell */
	s_ptr = &p_ptr->mp_ptr->info[j];

	/* Check mana */
	if (s_ptr->smana > p_ptr2->csp)
	{
		msg_print(Ind, "You do not have enough mana.");
		return;
	}

	/* Spell failure chance */
	chance = spell_chance(Ind2, s_ptr);

	/* Failed spell */
	if (rand_int(100) < chance)
	{
		if (flush_failure) flush();
		msg_print(Ind, "You failed to get the spell off!");
	}

	/* Process spell */
	else
	{
	  /* Hack -- chance of "beam" instead of "bolt" */
	  beam = plev / 2;

	  /* Spells.  */
	  switch (j)
	    {
	      
	    case 0: /* Mind Fusion */
	      {
		int Ind2 = get_player(Ind, o_ptr);
		
		if (!Ind2) break;
		
		if (Ind == Ind2)
		  {
		    msg_print(Ind, "You cannot mind control yourself.");
		    break;
		  }
		if (p_ptr->esp_link)
		  {
		    msg_print(Ind, "Your mind is already linked.");
		    break;
		  }
		if (Players[Ind2]->esp_link)
		  {
		    msg_format(Ind, "%s mind is already linked.", Players[Ind2]->name);
		    break;
		  }
		if (!(Players[Ind2]->esp_link_flags & LINKF_OPEN))
		  {
		    msg_format(Ind, "%s mind is not open.", Players[Ind2]->name);
		    break;
		  }
		if (abs(Players[Ind2]->lev - p_ptr->lev) > 5)
		  {
		    msg_print(Ind, "Level difference too high, sorry.");
		    break;
		  }
		
		msg_format(Ind2, "%s mind fusions with your mind.", p_ptr->name);
		msg_format(Ind, "Your mind fusion with %s mind.", Players[Ind2]->name);
		p_ptr->esp_link = Players[Ind2]->id;
		p_ptr->esp_link_type = LINK_DOMINANT;
		p_ptr->esp_link_flags = LINKF_MOV | LINKF_OBJ;
		p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_SPELL | PW_PLAYER);
		p_ptr->update |= (PU_BONUS | PU_VIEW | PU_MANA | PU_HP);
		p_ptr->redraw |= (PR_BASIC | PR_EXTRA | PR_MAP);
		
		Players[Ind2]->esp_link = p_ptr->id;
		Players[Ind2]->esp_link_type = LINK_DOMINATED;
		Players[Ind2]->esp_link_flags = LINKF_VIEW | LINKF_MISC | LINKF_PAIN;
		Players[Ind2]->window |= (PW_INVEN | PW_EQUIP | PW_SPELL | PW_PLAYER);
		Players[Ind2]->update |= (PU_BONUS | PU_VIEW | PU_MANA | PU_HP);
		Players[Ind2]->redraw |= (PR_BASIC | PR_EXTRA | PR_MAP);
		break;
	      }
	    case 1: /* Mind Thrust */
	      {
		get_aim_dir(Ind);
		p_ptr2->current_mind = j;
		return;
	      }
	    case 2: /* Mind Vision */
	      {
		int Ind2 = get_player(Ind, o_ptr);
		
		if (!Ind2) break;
		
		if (Ind == Ind2)
		  {
		    msg_print(Ind, "You cannot mind control yourself.");
		    break;
		  }
		if (p_ptr->esp_link)
		  {
		    msg_print(Ind, "Your mind is already linked.");
		    break;
		  }
		if (Players[Ind2]->esp_link)
		  {
		    msg_format(Ind, "%s mind is already linked.", Players[Ind2]->name);
		    break;
		  }

		msg_format(Ind2, "%s mind infiltrates your mind.", p_ptr->name);
		msg_format(Ind, "Your mind infiltrate %s mind.", Players[Ind2]->name);
		p_ptr->esp_link = Players[Ind2]->id;
		p_ptr->esp_link_type = LINK_DOMINANT;
		p_ptr->esp_link_flags = 0;
		p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_SPELL | PW_PLAYER);
		p_ptr->update |= (PU_BONUS | PU_VIEW | PU_MANA | PU_HP);
		p_ptr->redraw |= (PR_BASIC | PR_EXTRA | PR_MAP);
		
		Players[Ind2]->esp_link = p_ptr->id;
		Players[Ind2]->esp_link_type = LINK_DOMINATED;
		Players[Ind2]->esp_link_flags = LINKF_VIEW;
		Players[Ind2]->window |= (PW_INVEN | PW_EQUIP | PW_SPELL | PW_PLAYER);
		Players[Ind2]->update |= (PU_BONUS | PU_VIEW | PU_MANA | PU_HP);
		Players[Ind2]->redraw |= (PR_BASIC | PR_EXTRA | PR_MAP);
		break;
		break;
	      }
	      
	    case 3: /* Mind Control */
	      {
		int Ind2 = get_player(Ind, o_ptr);
		
		if (!Ind2) break;
		
		if (Ind == Ind2)
		  {
		    msg_print(Ind, "You cannot mind control yourself.");
		    break;
		  }
		if (p_ptr->esp_link)
		  {
		    msg_print(Ind, "Your mind is already linked.");
		    break;
		  }
		if (Players[Ind2]->esp_link)
		  {
		    msg_format(Ind, "%s mind is already linked.", Players[Ind2]->name);
		    break;
		  }
		if (!(Players[Ind2]->esp_link_flags & LINKF_OPEN))
		  {
		    msg_format(Ind, "%s mind is not open.", Players[Ind2]->name);
		    break;
		  }
		
		msg_format(Ind2, "%s mind infiltrates your mind.", p_ptr->name);
		msg_format(Ind, "Your mind infiltrate %s mind.", Players[Ind2]->name);
		p_ptr->esp_link = Players[Ind2]->id;
		p_ptr->esp_link_type = LINK_DOMINANT;
		p_ptr->esp_link_flags = LINKF_MOV | LINKF_OBJ;
		p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_SPELL | PW_PLAYER);
		p_ptr->update |= (PU_BONUS | PU_VIEW | PU_MANA | PU_HP);
		p_ptr->redraw |= (PR_BASIC | PR_EXTRA | PR_MAP);
		
		Players[Ind2]->esp_link = p_ptr->id;
		Players[Ind2]->esp_link_type = LINK_DOMINATED;
		Players[Ind2]->esp_link_flags = LINKF_VIEW | LINKF_MISC;
		Players[Ind2]->window |= (PW_INVEN | PW_EQUIP | PW_SPELL | PW_PLAYER);
		Players[Ind2]->update |= (PU_BONUS | PU_VIEW | PU_MANA | PU_HP);
		Players[Ind2]->redraw |= (PR_BASIC | PR_EXTRA | PR_MAP);
		break;
	      }
	      
	  //	  "Hold",
	  //	  "Dominate",
	  
	      /* Psychoportation */
	    case 8: /* Blink */
	      {
		teleport_player(Ind, 10);
		break;
	      }
	    case 9: /* Teleport */
	      {
		teleport_player(Ind, 200);
		break;
	      }
	    case 10: /* Probability Travel */
	      {
		set_prob_travel(Ind, p_ptr2->prob_travel + plev + randint(20));
		break;
	      }
	    case 11: /* Recall */
	      {
		if (!p_ptr2->word_recall)
		  {
		    set_recall_depth(p_ptr2, o_ptr);
		    p_ptr2->word_recall = rand_int(20) + 15;
		    msg_print(Ind, "The air about you becomes charged...");
		  }
		else
		  {
		    p_ptr2->word_recall = 0;
		    msg_print(Ind, "A tension leaves the air around you...");
		  }
		break;
	      }
	    case 12: /* Anihilate */
	      {
 		fire_ball(Ind, GF_AWAY_ALL, 0, p_ptr->lev * 2, 3 + (plev / 11));
		break;
	      }
	    case 13: /* Space/Time Anchor */
	      {
		set_st_anchor(Ind, p_ptr2->st_anchor + plev + randint(20));
		break;
	      }
	      /* Time Bolt */
	    case 14: /* Dig */
	      {
		get_aim_dir(Ind);
		p_ptr2->current_mind = j;
		return;
	      }
	      //	  "",

	      /* Telekinesis */
	    case 16: /* Dig */
	      {
		get_aim_dir(Ind);
		p_ptr2->current_mind = j;
		return;
	      }
	    case 17: /* Telekinesis */
	      {
		telekinesis(Ind, o_ptr);
		break;
	      }
	    case 18: /* Force Shield */
	      {
		msg_format_near(Ind, "%s forms a shield of pure force.", p_ptr2->name);
		(void)set_shield(Ind, p_ptr2->shield + randint(20) + 30);
		break;
	      }
	    case 19: /* Prismatic Shield */
	      {
		msg_format_near(Ind, "%s forms a shield of mental energy.", p_ptr2->name);
		(void)set_oppose_acid(Ind, p_ptr2->oppose_acid + randint(20) + 30);
		(void)set_oppose_elec(Ind, p_ptr2->oppose_elec + randint(20) + 30);
		(void)set_oppose_fire(Ind, p_ptr2->oppose_fire + randint(20) + 30);
		(void)set_oppose_cold(Ind, p_ptr2->oppose_cold + randint(20) + 30);
		(void)set_oppose_pois(Ind, p_ptr2->oppose_pois + randint(20) + 30);
		break;
	      }
	    case 20: /* Force Ray */
	      {
		get_aim_dir(Ind);
		p_ptr2->current_mind = j;
		return;
	      }
	    case 21: /* Sonic boom */
	      {
		msg_format_near(Ind, "%s emanates sound.", p_ptr2->name);
 		fire_ball(Ind, GF_SOUND, 0, 150 + (p_ptr->lev * 3), 3 + (plev / 10));
		break;
	      }
//	  "",
	    case 23: /* Inertia Wave */
	      {
		msg_format_near(Ind, "%s radiates a powerfull wave of inertia.", p_ptr2->name);
 		project_hack(Ind, GF_INERTIA, 200 + (p_ptr->lev * 2));
		break;
	      }
	  
	      /* Clairsentience */
	    case 24: /* Detect Monsters */
	      {
		(void)detect_creatures(Ind);
		break;
	      }
	    case 25: /* Reveal Secrets */
	      {
		(void)detect_treasure(Ind);
		(void)detect_object(Ind);
		(void)detect_sdoor(Ind);
		(void)detect_trap(Ind);
		break;
	      }
	    case 26: /* Read Object */
	      {
		(void)ident_spell(Ind);
		break;
	      }
	    case 27: /* Clairvoyance */
	      wiz_lite(Ind);
	      break;
	      //	  "",
	      //	  "",
	      //	  "",
	      //	  "",
	      
	    case 32: /* Share pain */
	      {
		int Ind2 = get_player(Ind, o_ptr);
		
		if (!Ind2) break;
		
		if (Ind == Ind2)
		  {
		    msg_print(Ind, "You cannot share pain with yourself yourself.");
		    break;
		  }
		if (p_ptr->esp_link)
		  {
		    msg_print(Ind, "Your mind is already linked.");
		    break;
		  }
		if (Players[Ind2]->esp_link)
		  {
		    msg_format(Ind, "%s mind is already linked.", Players[Ind2]->name);
		    break;
		  }
		if (!(Players[Ind2]->esp_link_flags & LINKF_OPEN))
		  {
		    msg_format(Ind, "%s mind is not open.", Players[Ind2]->name);
		    break;
		  }
	      	if (abs(Players[Ind2]->lev - p_ptr->lev) > 5)
		  {
		    msg_print(Ind, "Level difference too high, sorry.");
		    break;
		  }

		msg_format(Ind2, "%s share your pains.", p_ptr->name);
		msg_format(Ind, "Your share %s pains.", Players[Ind2]->name);
		p_ptr->esp_link = Players[Ind2]->id;
		p_ptr->esp_link_type = LINK_DOMINANT;
		p_ptr->esp_link_flags = 0;
		p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_SPELL | PW_PLAYER);
		p_ptr->update |= (PU_BONUS | PU_VIEW | PU_MANA | PU_HP);
		p_ptr->redraw |= (PR_BASIC | PR_EXTRA | PR_MAP);
		
		Players[Ind2]->esp_link = p_ptr->id;
		Players[Ind2]->esp_link_type = LINK_DOMINATED;
		Players[Ind2]->esp_link_flags = LINKF_PAIN;
		Players[Ind2]->window |= (PW_INVEN | PW_EQUIP | PW_SPELL | PW_PLAYER);
		Players[Ind2]->update |= (PU_BONUS | PU_VIEW | PU_MANA | PU_HP);
		Players[Ind2]->redraw |= (PR_BASIC | PR_EXTRA | PR_MAP);
		break;
	      }
	    case 33: /* Awareness */
	      {
		(void)set_tim_esp(Ind, p_ptr->shield + randint(10) + plev);
		break;
	      }
	      //	  "",
	      //	  "",
	      //	  "",
	      //	  "",
	      //	  "",
	      //	  "",

	      /* Psychometabolism */
	    case 40: /* Cell Adjustment */
	      {
		(void)hp_player(Ind, (damroll((plev / 2), 6)));
		(void)set_cut(Ind, p_ptr2->cut - 15);
		break;
	      }
			
	    case 41: /* Light Control */
	      {
		(void)lite_area(Ind, damroll(2, (plev / 2)), (plev / 10) + 1);
		break;
	      }
	    case 42: /* Satisfy Hunger */
	      {
		msg_print(Ind, "You start converting your body's energy into food.");
		(void) set_food(Ind, PY_FOOD_MAX - 1);
		break;
	      }
	    case 43: /* Adrenaline Control */
	      {
		(void)set_adrenaline(Ind, p_ptr2->adrenaline+ randint(20) + (plev/2));
		break;
	      }
	    case 44: /* Biofeedback */
	      {
		(void)set_biofeedback(Ind, p_ptr2->biofeedback + randint((10 + plev)));
		break;
	      }
	    case 45: /* Complete Healing */
	      {
		msg_print(Ind, "You enter a trance and your body begins to rapidly recover.");
		(void)hp_player(Ind, 5000);
		(void)set_poisoned(Ind, 0);
		(void)set_stun(Ind, 0);
		(void)set_cut(Ind, 0);
		p_ptr2->energy -= 350 - plev * 2;
		break;
	      }
	    }
	
	
	  /* A spell was cast */
	  if (!((j < 32) ?
		(p_ptr->spell_worked1 & (1L << j)) :
		(p_ptr->spell_worked2 & (1L << (j - 32)))))
	    {
	      int e = s_ptr->sexp;
	      
	      /* The spell worked */
	      if (j < 32)
		{
		  p_ptr->spell_worked1 |= (1L << j);
		}
	      else
		{
		  p_ptr->spell_worked2 |= (1L << (j - 32));
		}
	      
	      /* Gain experience */
	      gain_exp(Ind, e * s_ptr->slevel);
	      
	      /* Fix the spell info */
	      p_ptr->window |= PW_SPELL;
	    }
	}
	
	/* Take a turn */
	p_ptr->energy -= level_speed(p_ptr->dun_depth) / p_ptr2->num_spell;

	/* Sufficient mana */
	if (s_ptr->smana <= p_ptr2->csp)
	{
		/* Use some mana */
		p_ptr2->csp -= s_ptr->smana;
	}

	/* Over-exert the player */
	else
	{
		int oops = s_ptr->smana - p_ptr2->csp;

		/* No mana left */
		p_ptr2->csp = 0;
		p_ptr2->csp_frac = 0;

		/* Message */
		msg_print(Ind, "You faint from the effort!");

		/* Hack -- Bypass free action */
		(void)set_paralyzed(Ind, p_ptr2->paralyzed + randint(5 * oops + 1));

		/* Damage CON (possibly permanently) */
		if (rand_int(100) < 50)
		{
			bool perm = (rand_int(100) < 25);

			/* Message */
			msg_print(Ind, "You have damaged your health!");

			/* Reduce constitution */
			(void)dec_stat(Ind, A_CON, 15 + randint(10), perm);
		}
	}

	/* Redraw mana */
	p_ptr2->redraw |= (PR_MANA);

	/* Window stuff */
	p_ptr2->window |= (PW_PLAYER);
}


/*
 * Finish casting a spell that required a direction --KLJ--
 */
void do_cmd_psi_aux(int Ind, int dir)
{
	player_type *p_ptr = Players[Ind];

	int plev = p_ptr->lev;
	int beam = ((p_ptr->pclass == 1) ? plev : (plev / 2));
	int Ind2;  /* Ind2 = Dominant(caster), Ind = Dominated */
	player_type *p_ptr2; /* p_ptr = Dominant(caster), p_ptr2 = Dominated */
	magic_type *s_ptr;

	/* Require spell ability */
	if (p_ptr->mp_ptr->spell_book != TV_PSI_BOOK)
	{
		if (p_ptr->esp_link_type && p_ptr->esp_link && (p_ptr->esp_link_flags & LINKF_MISC))
		  {
		    Ind2 = find_player(p_ptr->esp_link);
		    
		    if (!Ind2)
		      {
			p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_SPELL | PW_PLAYER);
			p_ptr->update |= (PU_BONUS | PU_VIEW | PU_MANA | PU_HP);
			p_ptr->redraw |= (PR_BASIC | PR_EXTRA | PR_MAP);
			msg_print(Ind, "Ending mind link.");
			p_ptr->esp_link = 0;
			p_ptr->esp_link_type = 0;
			p_ptr->esp_link_flags = 0;
		      }
		    else
		      {
			p_ptr2 = p_ptr;
			p_ptr = Players[Ind2];
		      }
		  }
		
		if (!Ind2 || (p_ptr->mp_ptr->spell_book != TV_PSI_BOOK))
		  {
		    msg_print(Ind, "You cannot use psi powers!");
		    return;
		  }
	}
	else
	  {
	    p_ptr2 = p_ptr;
	    Ind2 = Ind;
	  }


	s_ptr = &p_ptr->mp_ptr->info[p_ptr->current_mind];

	/* Only fire in direction 5 if we have a target */
	if ((dir == 5) && !target_okay(Ind))
	{
		/* Reset current spell */
		p_ptr2->current_mind = -1;

		/* Done */
		return;
	}

	/* We assume that the spell can be cast, and so forth */
	switch(p_ptr2->current_mind)
	{
		case 1:
			msg_format_near(Ind, "%s hurls a mind thrust.", p_ptr2->name);
			fire_bolt(Ind, GF_PSI, dir,
						damroll(3 + ((plev - 1) / 5), 4 + (plev / 5)));
			break;
		case 14:
			msg_format_near(Ind, "%s breaks the space/time continuum.", p_ptr2->name);
			if (p_ptr->st_anchor)
			  {
			    if (!rand_int(3))
			      {
				msg_print(Ind, "The space/time anchor stops your time bolt!");
				break;
			      }
			    else if (!rand_int(3))
			      {
				msg_print(Ind, "Your space/time anchor collapses!");
				p_ptr->st_anchor = 0;
			      }
			  }
			  
			fire_bolt_or_beam(Ind, plev, GF_TIME, dir, damroll(10, plev));
			break;
		case 16:
			(void)wall_to_mud(Ind, dir);
			break;
		case 20:
			msg_format_near(Ind, "%s hurls a force ray.", p_ptr2->name);
			fire_beam(Ind, GF_FORCE, dir, 100 + randint(plev * 2));
			break;
		default:  /* For some reason we got called for a spell that */
		{         /* doesn't require a direction */
			msg_print(Ind, "SERVER ERROR: do_cmd_psi_aux() called for non-directional spell!");
			p_ptr2->current_mind = -1;
			return;
		}
	}	

	if (!((p_ptr2->current_mind < 32) ?
		(p_ptr->spell_worked1 & (1L << p_ptr->current_mind)) :
		(p_ptr->spell_worked2 & (1L << (p_ptr->current_mind - 32)))))
	{
		int e = s_ptr->sexp;

		if (p_ptr2->current_mind < 32)
		{
			p_ptr->spell_worked1 |= (1L << p_ptr->current_mind);
		}
		else
		{
			p_ptr->spell_worked2 |= (1L << (p_ptr->current_mind - 32));
		}

		gain_exp(Ind, e * s_ptr->slevel);

		/* Fix the spell info */
		p_ptr->window |= PW_SPELL;
	}

	p_ptr->energy -= level_speed(p_ptr->dun_depth) / p_ptr2->num_spell;

	if (s_ptr->smana <= p_ptr2->csp)
	{
		/* Use some mana */
		p_ptr2->csp -= s_ptr->smana;
	}

	/* Over-exert the player */
	else
	{
		int oops = s_ptr->smana - p_ptr2->csp;

		/* No mana left */
		p_ptr2->csp = 0;
		p_ptr2->csp_frac = 0;

		/* Message */
		msg_print(Ind, "You faint from the effort!");

		/* Hack -- bypass free action */
		(void)set_paralyzed(Ind, p_ptr2->paralyzed + randint(5 * oops + 1));

		/* Damage CON (possibly permanently) */
		if (rand_int(100) < 50)
		{
			bool perm = (rand_int(100) < 25);

			/* Message */
			msg_print(Ind, "You have damaged your health!");

			/* Reduce constitution */
			(void)dec_stat(Ind, A_CON, 15 + randint(10), perm);
		}
	}

	/* Reset current spell */
	p_ptr2->current_mind = -1;

	/* Resend mana */
	p_ptr2->redraw |= (PR_MANA);

	/* Window stuff */
	p_ptr2->window |= (PW_PLAYER);
}
