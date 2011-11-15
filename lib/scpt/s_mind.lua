-- handle the mind school

--[[ Enable when we get pets
CHARM = add_spell
{
	["name"] = 	"Charm",
        ["school"] = 	{SCHOOL_MIND},
        ["level"] = 	1,
        ["mana"] = 	1,
        ["mana_max"] = 	20,
        ["fail"] = 	10,
        ["direction"] = function () if get_level(Ind, CHARM, 50) >= 35 then return FALSE else return TRUE end end,
        ["spell"] = 	function(args)
                        if get_level(Ind, CHARM, 50) >= 35 then
                                project_los(Ind, GF_CHARM, 10 + get_level(Ind, CHARM, 150), "mumbles softly")
                        elseif get_level(Ind, CHARM, 50) >= 15 then
                                fire_ball(Ind, GF_CHARM, args.dir, 10 + get_level(Ind, CHARM, 150), 3, "mumbles softly")
                        else
                                fire_bolt(Ind, GF_CHARM, args.dir, 10 + get_level(Ind, CHARM, 150), "mumbles softly")
                        end
	end,
	["info"] = 	function()
                	return "power "..(10 + get_level(Ind, CHARM, 150))
	end,
        ["desc"] =	{
        		"Tries to manipulate the mind of a monster to make it friendly",
                        "At level 15 it turns into a ball",
                        "At level 35 it affects all monsters in sight"
        }
}
]]
CONFUSE = add_spell
{
	["name"] = 	"Confusion",
        ["school"] = 	{SCHOOL_MIND},
        ["level"] = 	5,
        ["mana"] = 	5,
        ["mana_max"] = 	30,
        ["fail"] = 	10,
        ["direction"] = function () if get_level(Ind, CONFUSE, 50) >= 35 then return FALSE else return TRUE end end,
        ["spell"] = 	function(args)
                        if get_level(Ind, CONFUSE, 50) >= 35 then
                                project_los(Ind, GF_OLD_CONF, 5 + get_level(Ind, CONFUSE, 100), "focusses on your mind")
                        elseif get_level(Ind, CONFUSE, 50) >= 15 then
                                fire_ball(Ind, GF_OLD_CONF, args.dir, 5 + get_level(Ind, CONFUSE, 100), 2, "focusses on your mind")
                        else
                                fire_bolt(Ind, GF_OLD_CONF, args.dir, 5 + get_level(Ind, CONFUSE, 100), "focusses on your mind")
                        end
	end,
	["info"] = 	function()
			if get_level(Ind, CONFUSE, 50) < 35 and get_level(Ind, CONFUSE, 50) >= 15 then
	                	return "power "..(5 + get_level(Ind, CONFUSE, 100)).." rad 2"
                	else
	                	return "power "..(5 + get_level(Ind, CONFUSE, 100))
    	            	end
	end,
        ["desc"] =	{
        		"Tries to manipulate the mind of a monster to confuse it",
                        "At level 15 it turns into a ball",
                        "At level 35 it affects all monsters in sight"
        }
}

STUN = add_spell
{
	["name"] = 	"Stun",
        ["school"] = 	{SCHOOL_MIND},
        ["level"] = 	15,
        ["mana"] = 	10,
        ["mana_max"] = 	90,
        ["fail"] = 	10,
        ["direction"] = TRUE,
        ["spell"] = 	function(args)
                        if get_level(Ind, STUN, 50) >= 25 then
                                fire_ball(Ind, GF_STUN, args.dir, 10 + get_level(Ind, STUN, 50), 2, "")
                        else
                                fire_bolt(Ind, GF_STUN, args.dir, 10 + get_level(Ind, STUN, 50), "")
                        end
	end,
	["info"] = 	function()
                	return ""
                	--return "power "..(10 + get_level(Ind, STUN, 150))
	end,
        ["desc"] =	{
                        "Tries to manipulate the mind of a monster to stun it",
                        "At level 25 it turns into a ball",
        }
}

TELEKINESIS = add_spell
{
	["name"] = 	"Telekinesis I",
        ["school"] = 	{SCHOOL_MIND, SCHOOL_CONVEYANCE},
        ["level"] = 	30,
        ["mana"] = 	25,
        ["mana_max"] = 	25,
        ["fail"] =      20,
        ["get_item"] =  {
                        ["prompt"] = 	"Teleport which object? ",
                        ["inven"] = 	TRUE,
                        ["get"] = 	function (obj)
	                                if obj.weight * obj.number <= 4 + get_level(Ind, TELEKINESIS, 250, 0) then
        	                        	return TRUE
                	                end
                        	        return FALSE
                        end,
        },
        ["spell"] = 	function(args)
                        if args.item == -1 then return end
			if player.inventory[1 + args.item].weight * player.inventory[1 + args.item].number <= 4 + get_level(Ind, TELEKINESIS, 250, 0) then
                        	player.current_telekinesis = player.inventory[1 + args.book]
                                telekinesis_aux(Ind, args.item)
                        else
                                msg_print(Ind, "Pfft trying to hack your client ? pretty lame ...")
                        end
	end,
	["info"] = 	function()
			return "max wgt "..((4 + get_level(Ind, TELEKINESIS, 250, 0)) / 10).."."..(imod(4 + get_level(Ind, TELEKINESIS, 250, 0), 10))
	end,
        ["desc"] =	{
        		"Inscribe your book with @Pplayername, cast it, select an item",
                        "and the item will be teleported to that player whereever he/she might",
                        "be in the Universe",
        }
}

SENSEMONSTERS = add_spell
{
	["name"] = "Sense Minds",
        ["school"] = {SCHOOL_MIND},
	["level"] = 30,
	["mana"] = 15,
        ["mana_max"] = 20,
        ["fail"] = -15,
        ["spell"] = function()
	        set_tim_esp(Ind, 22 + randint(10) + get_level(Ind, SENSEMONSTERS, 28))
		end,
	["info"] = function()
		return "dur "..(22 + get_level(Ind, SENSEMONSTERS, 28)).."+d10"
		end,
	["desc"] = {
		"Sense all monsters' minds for a while.",
	}
}
