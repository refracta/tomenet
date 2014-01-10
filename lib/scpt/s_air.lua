-- handle the air school

NOXIOUSCLOUD = add_spell
{
	["name"] = 	"Noxious Cloud",
        ["school"] = 	{SCHOOL_AIR},
        ["level"] = 	3,
        ["mana"] = 	3,
        ["mana_max"] = 	30,
        ["fail"] = 	20,
        ["direction"] = TRUE,
        ["spell"] = 	function(args)
			local type

                        if get_level(Ind, NOXIOUSCLOUD, 50) >= 30 then type = GF_UNBREATH
                        else type = GF_POIS end
--			type = GF_POIS
--		        fire_cloud(Ind, type, args.dir, ((1 + get_level(Ind, NOXIOUSCLOUD, 75)) + (get_level(Ind,MANATHRUST,50) * 2)), 3, 5 + get_level(Ind, NOXIOUSCLOUD, 40), " fires a noxious cloud of")
--		        fire_cloud(Ind, type, args.dir, (1 + get_level(Ind, NOXIOUSCLOUD, 75)), 3, 5 + get_level(Ind, NOXIOUSCLOUD, 40), 10, " fires a noxious cloud of")
--		        fire_cloud(Ind, type, args.dir, (1 + get_level(Ind, NOXIOUSCLOUD, 113)), 3, 5 + get_level(Ind, NOXIOUSCLOUD, 10), 10, " fires a noxious cloud of")
--1.5		        fire_cloud(Ind, type, args.dir, (1 + get_level(Ind, NOXIOUSCLOUD, 113)), 3, 5 + get_level(Ind, NOXIOUSCLOUD, 14), 9, " fires a noxious cloud of")
		        fire_cloud(Ind, type, args.dir, (1 + get_level(Ind, NOXIOUSCLOUD, 156)), 3, 5 + get_level(Ind, NOXIOUSCLOUD, 14), 9, " fires a noxious cloud of")
	end,
	["info"] = 	function()
			return "dam "..(1 + (get_level(Ind, NOXIOUSCLOUD, 156))).." rad 3 dur "..(5 + get_level(Ind, NOXIOUSCLOUD, 14))
	end,
        ["desc"] =	{
        		"Creates a cloud of poison",
                        "The cloud will persist for some turns, damaging all monsters passing by",
                        "At level 30 it turns into a thick gas preventing living beings from breathing"
        }
}

function get_lightningbolt_dam()
        return 3 + get_level(Ind, LIGHTNINGBOLT, 25), 5 + get_level(Ind, LIGHTNINGBOLT, 25) - 1
end
LIGHTNINGBOLT = add_spell
{
        ["name"] =      "Lightning Bolt",
        ["school"] =    SCHOOL_AIR,
        ["level"] =     6,
        ["mana"] =      2,
        ["mana_max"] =  10,
        ["fail"] =      -10,
        ["direction"] = TRUE,
        ["ftk"] = 1,
        ["spell"] =     function(args)
                        fire_bolt(Ind, GF_ELEC, args.dir, damroll(get_lightningbolt_dam()), " casts a lightning bolt for")
        end,
        ["info"] =      function()
                        local x, y

                        x, y = get_lightningbolt_dam()
                        return "dam "..x.."d"..y
        end,
        ["desc"] =      {
                        "Conjures up a powerful lightning bolt",
                }
}

AIRWINGS = add_spell
{
	["name"] = 	"Wings of Winds",
--note: is dual-school, but doesn't occur in conveyance school (still ok i think, maybe reduce level req slightly) - C. Blue
        ["school"] = 	{SCHOOL_AIR, SCHOOL_CONVEYANCE},
--        ["level"] = 	22,
        ["level"] = 	16,
        ["mana"] = 	30,
        ["mana_max"] = 	40,
        ["fail"] = 	70,
        ["spell"] = 	function()
			if get_level(Ind, AIRWINGS, 50) >= 16 then set_tim_fly(Ind, randint(10) + 5 + get_level(Ind, AIRWINGS, 25))
                        else set_tim_ffall(Ind, randint(10) + 5 + get_level(Ind, AIRWINGS, 25))
                        end
	end,
	["info"] = 	function()
			return "dur "..(5 + get_level(Ind, AIRWINGS, 25)).."+d10"
	end,
        ["desc"] =	{
        		"Grants the power of feather falling",
                        "At level 16 it grants the power of levitation"
        }
}

INVISIBILITY = add_spell
{
	["name"] = 	"Invisibility",
        ["school"] = 	{SCHOOL_AIR},
        ["level"] = 	30,
        ["mana"] = 	20,
        ["mana_max"] = 	50,
        ["fail"] = 	70,
        ["spell"] = 	function()
--                       	if player.tim_invisibility == 0 then set_invis(Ind, randint(20) + 15 + get_level(Ind, INVISIBILITY, 50), 20 + get_level(Ind, INVISIBILITY, 50)) end
			set_invis(Ind, randint(20) + 15 + get_level(Ind, INVISIBILITY, 50), 20 + get_level(Ind, INVISIBILITY, 50))
       	end,
	["info"] = 	function()
			return "dur "..(15 + get_level(Ind, INVISIBILITY, 50)).."+d20 power "..(20 + get_level(Ind, INVISIBILITY, 50))
	end,
        ["desc"] =	{
                        "Grants invisibility"
        }
}

POISONBLOOD = add_spell
{
	["name"] = 	"Poison Blood",
        ["school"] = 	{SCHOOL_AIR},
        ["level"] = 	22,
        ["mana"] = 	10,
        ["mana_max"] = 	20,
        ["fail"] = 	70,
        ["spell"] = 	function()
                       	set_oppose_pois(Ind, randint(30) + 25 + get_level(Ind, POISONBLOOD, 25))
                      	if get_level(Ind, POISONBLOOD, 50) >= 10 then set_brand(Ind, randint(30) + 25 + get_level(Ind, POISONBLOOD, 25), BRAND_POIS, 10) end
       	end,
	["info"] = 	function()
			return "dur "..(25 + get_level(Ind, POISONBLOOD, 25)).."+d30"
	end,
        ["desc"] =	{
                        "Grants poison resistance",
                        "At level 10 it provides poison branding to wielded weapon"
        }
}

THUNDERSTORM = add_spell
{
	["name"] = 	"Thunderstorm",
--was dual-school, but doesn't occur in Tree tome/nature school! so making it single-school for now (C. Blue) - SCHOOL_NATURE},
        ["school"] = 	{SCHOOL_AIR},
        ["level"] = 	25,
        ["mana"] = 	40,
        ["mana_max"] = 	60,
-- DEBUG- Eddio claimed this was way too high, I dunno, just changing it like that - C. Blue        ["fail"] = 	70,
        ["fail"] = 	50,
        ["spell"] = 	function()
			set_tim_thunder(Ind, randint(10) + 10 + get_level(Ind, THUNDERSTORM, 25), 5 + get_level(Ind, THUNDERSTORM, 10), 10 + get_level(Ind, THUNDERSTORM, 25))
       	end,
	["info"] = 	function()
			return "dam "..(5 + get_level(Ind, THUNDERSTORM, 10)).."d"..(10 + get_level(Ind, THUNDERSTORM, 25)).." dur "..(10 + get_level(Ind, THUNDERSTORM, 25)).."+d10"
	end,
        ["desc"] =	{
                        "Charges up the air around you with electricity",
                        "Each turn it will throw a thunder bolt at a random monster in sight",
                        "The thunder does 3 types of damage, one third of lightning",
                        "one third of sound and one third of light"
        }
}
