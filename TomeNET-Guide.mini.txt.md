.==========================================================================+--.
|                              TomeNET Guide                               |
+==========================================================================+-
| Latest update: 8. September 2024 - written by C. Blue (c_blue@gmx.net)   |
| for TomeNET version v4.9.2 - official websites are:                      :
| https://www.tomenet.eu/ (official main site, formerly www.tomenet.net)
| https://muuttuja.org/tomenet/ (Mikael's TomeNET site)
| Runes & Runemastery sections by Kurzel (kurzel.tomenet@gmail.com)
|
| You should always keep this guide up to date: Either go to www.tomenet.eu
| to obtain the latest copy or simply run the TomeNET-Updater.exe in your
| TomeNET installation folder (desktop shortcut should also be available)
| to update it.
|
| If your text editor cannot display the guide properly (needs fixed-width
| font like for example Courier), simply open it in any web browser instead.
+---
| Welcome to this guide!
| Although I'm trying, I give no guarantee that this guide
| a) contains really every detail/issue about TomeNET and
| b) is all the time 100% accurate on every occasion.
| Don't blame me if something differs or is missing; it shouldn't though.
|
| If you have any suggestions about the guide or the game, please use the
| /rfe command in the game or write to the official forum on www.tomenet.eu.
:
\


Contents
--------
(0) Quickstart (If you don't like to read much :)
    (0.1)   Start & play (the basics)
    (0.1a)  Character validation
    (0.1b)  Colours and colour blindness
    (0.1c)  Photosensitivity / Epilepsy issues
    (0.1d)  Readability improvements
    (0.2)   Command reference & keysets
    (0.2a)  Text editing keys
    (0.2b)  Slash commands
    (0.2c)  IRC bot commands
    (0.3)   Important inscriptions, Word of Recall
    (0.3a)  The auto-inscription feature
    (0.4)   Typical slang/abbreviations
    (0.5)   Some hints and warnings (READ!)
(1) Introduction
    (1.1)   What is TomeNET? (Where can I get the latest sources?)
    (1.1a)  Where do I get in contact?
    (1.1b)  How do I set up IRC chat?
    (1.2)   What is a rogue-like game?
    (1.2a)  Where do I find more information about rogue-like games?
    (1.3)   Goal and progress of TomeNET players
    (1.3a)  Sauron, Morgoth and the Nazgul
    (1.3b)  How to encounter the final boss: Morgoth, Lord of Darkness
    (1.4)   History of TomeNET development
(2) Installation & setup
    (2.1)   Installing/updating TomeNET (Windows/Linux/OSX)
    (2.1a)  Windows (custom/compilation)
    (2.1b)  Linux (custom/compilation)
    (2.1c)  Mac OS X (custom/compilation)
    (2.1d)  FreeBSD (custom/compilation)
    (2.1e)  Android (custom/compilation)
    (2.2)   Installing sound and music packs
    (2.2a)  Audio controls and config options
    (2.2b)  Editing sound and music configuration
    (2.3)   "Promoting" TomeNET on Steam ;)
    (2.4)   Starting TomeNET and getting to play on an internet server
    (2.5)   Client options
    (2.5a)  Client options configuration files
    (2.6)   Installing voice chat
    (2.7)   Fonts
    (2.7a)  Custom fonts
    (2.7b)  Graphical tilesets
(3) Creating a character & starting to play
    (3.1)   Creating a new character
    (3.1a)  Character modes
    (3.1b)  Body Modifications
    (3.1c)  Character names, collisions and reservations
    (3.2)   The display (user interface)
    (3.2a)  The encumberment bar
    (3.3)   Chat, chat messages and chat modes
    (3.4)   Inventory and equipment
    (3.4a)  Subinventory aka Bags
    (3.4b)  Start-up equipment & how to use it
    (3.4c)  Item basics
    (3.4d)  Item trading rules and exceptions
    (3.4e)  Money
    (3.5)   Preparing the first time for fighting enemies
    (3.6)   Creating basic macros & inscribing items, predefined macros
    (3.7)   Creating macros for ranged attacks & abilities (spells)
    (3.7a)  Ability codes [contained within (3.7)]
    (3.7b)  Quick'n'dirty macros
    (3.7c)  The macro wizard
    (3.7d)  Specifying directions and targets
    (3.8)   Example macro sets
    (3.9)   Macros with special keys
    (3.9a)  Creating a chat macro for your party
    (3.9b)  Alternative macros: Calling objects by their name
    (3.10)  Experience, level-ups and skill points
    (3.10a) Experience points table
    (3.11)  Death, ghosts and looting
    (3.12)  Character timeout and account timeout
(4) The world
    (4.1)   Running, picking up items, fighting
    (4.1a)  Auto-retaliation
    (4.2)   Level and owner of items
    (4.3)   Dungeon and tower types
    (4.3a)  The jail
    (4.4)   Terrain
    (4.5)   Towns, shops, the dungeons
    (4.5a)  Shops and their services
    (4.5a2) The casino
    (4.5b)  Towns & town dungeons
    (4.5c)  Dungeons
    (4.5c2) Example dungeon depth relations
    (4.5d)  Dungeon bosses
    (4.6)   Pits and vaults
    (4.7)   Traps & trap kits
    (4.7a)  Trap kit load effects
    (4.8)   Dungeon, floor & vault feelings
    (4.9)   Navigating on maps
    (4.10)  Houses
    (4.10a) Player stores
    (4.11)  Malicious effects players can suffer
    (4.11a) Drain effects
    (4.11b) Common recurring combat buffs
    (4.12)  Malicious effects monsters can suffer
    (4.13)  Extermination orders, events and Ironman Challenge
    (4.14)  Time systems, speed
    (4.14a) In-game metrics
    (4.15)  Swimming
    (4.16)  Teleportation
(5) Item and flag details, elements
    (5.1)   Melee weapons, brands
    (5.1a)  Melee weapon types
    (5.2)   Ranged weapons, brands
    (5.2a)  Fire-till-kill mode
    (5.2b)  Throwing items
    (5.3)   Slaying vs brands
    (5.3a)  Martial Arts, brands
    (5.4)   Application of attack-affecting item mods
    (5.5)   Weight of weapons and armour, weight limits
    (5.6)   Resistances & caps, immunities, susceptibilities
    (5.7)   Qualitative classification, identifying & selling items
    (5.7a)  Types of identification
    (5.7b)  The EASY_KNOW flag
    (5.8)   Special abilities of items
    (5.9)   What does item xxx do exactly?
    (5.9a)  Activatable items
    (5.9b)  Artifacts and artifact resets
    (5.10)  Destruction of items by elemental attacks
    (5.10a) Equipment damage from elemental attacks
    (5.10b) Item damage on the floor
    (5.11)  Earthquakes
    (5.12)  Curses
    (5.13)  Vampirism
    (5.14)  Invisibility
    (5.15)  How abilities, resistances and boni stack
    (5.16)  Experience-draining items
    (5.17)  Digging
    (5.17a) Digging - exact calculation
    (5.17b) Digging - common example values
    (5.17c) Digging - further information
    (5.18)  Recharging
    (5.19)  Enchanting
    (5.20)  Wraithform
(6) Monster details
    (6.1)  Monsters
    (6.1a) Normal, boss & unique monsters and how they spawn
    (6.2)  Monster flags
    (6.3)  Loot tables, treasure classes
    (6.3a) Special item drops of monsters
    (6.4)  Monster attack mechanism, armour class, damage caps
    (6.5)  Monster attack types and brands
    (6.6)  Dispelling/banishing/turning monsters
    (6.7)  Monster auras
(7) Character details
    (7.1)   Attributes
    (7.2)   Skills, opposed skills
    (7.2a)  Maiar initiation effects
    (7.2b)  Demolition charges (Blast charges)
    (7.3)   Abilities
    (7.4)   Races
    (7.5)   Classes/professions
    (7.5a)  Start-up blows/round table (BpR)
    (7.5b)  Titles
    (7.5c)  Power levels
    (7.6)   Sanity (or insanity)
    (7.7)   Mimicry details
    (7.7a)  Polymorph rings
    (7.7b)  Useful low-level forms for weapon-based mimicry and more
    (7.7c)  Druid forms
    (7.8)   Spells and prayers
    (7.8a)  Mimic powers
    (7.8b)  Runes & Runemastery
    (7.8c)  Runespell tables
    (7.8d)  Spell failure rate
    (7.9)   Parties, Iron Teams, guilds
    (7.9a)  Guild halls
    (7.10)  Magic devices, skill and ability
    (7.11)  Parrying and blocking
    (7.12)  Fighting/shooting techniques
    (7.13)  PvP mode
    (7.14)  Miscellaneous character properties
    (7.15)  Character sheet boni page
    (7.16)  Food consumption
(8) Tactics & strategy
    (8.1)   Preparing the inventory for diving into the dungeon
    (8.2)   Preparing the equipment for diving into the dungeon
    (8.3)   Surviving critical situations
    (8.3a)  When you are stuck
    (8.4)   Spoiler files
    (8.5)   Monster tactics
    (8.6)   Town/dungeon related hints
(9) Miscellanous
    (9.1)   'The Art Not To Die'
    (9.1a)  The worst kind of help
    (9.2)   Character dumps
    (9.2a)  High Score
    (9.3)   Golem creation
    (9.4)   Artifact creation
    (9.5)   Reward item creation
    (9.6)   Alternative ruleset 'Ironman Server'
    (9.6a)  Special server type 'Arcade Server'
    (9.7)   Game engine limits
    (9.8)   Troubleshooting



(0) Quickstart
==============
This chapter is basically the heavily compressed version of chapters (2) which
are about installing and chapters (3) which are about logging on and creating a
character. If you like to read in more detail, maybe switch to (2) now instead.

(0.1) Start & play (the basics)
-------------------------------
A warning first, about character timeout:
If you do not log in a character at least once every 365 days the system will
automatically delete it along with its houses, to keep the database clean.
So make sure you don't let your characters time out this way!
(There will be an expiry warning about 60 days before this happens when you
log on with any of your characters, to remind you.)
Depending on the server settings, there might be a backup that allows
restoration of deleted characters and their house contents to some extent.
More information about character and account timeout at (3.12).

If you are using Linux, you will find additional information about how to
compile the game under '(2.1) Installing/updating TomeNET (Windows/Linux/OSX)'.

If you use a foreign IME setting, eg korean or japanese, switch it to US-EN IME
(compare problem 54 in the troubleshooting section (9.8)) or you might not be
able to use most keys aside from arrow keys for basic movement.

-Get the latest client from the project's main official site www.tomenet.eu.
 or from  https://muuttuja.org/tomenet/ (Mikael's site, which features a
 monster-search and other stuff).
 If you're on Microsoft Windows, it is recommended to get the installer package
 instead of the zip archives.

 Note that the Nullsoft Installer framework (NSIS) TomeNET uses is an open
 source project (https://sourceforge.net/projects/nsis/), yet annoyingly there
 are a few badly maintained virus scanners that may give a wrong positive
 warnings for NSIS. In the past Avira was notorious for it but it seems they
 fixed it, nowadays it seems to be Acronis instead, at least at the time of
 writing these lines. Anyway, you can safely ignore those *ahem*.
 A fun fact: The TomeNET Winwdows version has never seen an actual Windows
 system but was created purely on Linux.

-If you used the installer you will have a TomeNET icon on your desktop to
 start the game (and another icon for the TomeNET Updater).

 If you used the ZIP archive instead of the installer, you can run TomeNET
 from the unzipped folder by either double-clicking 'TomeNET.exe' or (this is a
 safe-fail in case your internet provider ever has DNS problems and you cannot
 connect to the server) by double-clicking 'TomeNET-direct.bat' to directly
 connect to official main server's IP address (37.187.75.24).

 NOTE: The official main server's hostname is "europe.tomenet.eu".
 In 2022, an official server located in the APAC (Asia-Pacific) region was
 added, to provide playable ping times for players from that region, its
 hostname is "apac.tomenet.eu".

-You should now see a list of available TomeNET servers (except if you used
 TomeNET-direct.bat, which will always connect you directly to the first main
 server).
 Pick the first official server by pressing the 'a' key (or instead choose the
 APAC region server if your latency is better for that region).
 Now you are prompted to enter your account information: Name and password.
 Since you are a new player, you don't have these yet. There is no separate
 'registration' process, just type in a name and a password of your choice!
 Make sure you remember your password and enter it carefully so you don't
 make a typo. The TomeNET client will, however, automatically store these
 crecedentials so next time you log in you can just hit ENTER key to confirm
 the saved name and password which will be automatically filled in for you.

-Now you are presented with the account overview screen. It's just the list of
 all your existing characters. Since you don't have any yet, it's empty.
 Press 'N' (shift+n) to create a new character.

-In this example we create a Half-Troll warrior, as it is easy for beginners:
 It can take a lot of punishment and at the same time dish out a lot of it too!
 The server should now allow you to manually choose your stats.
 In case you chose a (Half-Troll) warrior, try this:
  STR _at least_ 18/30 (to get a high number of attacks per round - the game
  client will even suggest 18/40 for minimum STR),
  CON as high as possible, INT as low as possible,
  spare points into DEX (especially!), WIS or CHR.
 If you decided to play a paladin make sure you have at least DEX 10.
  In case you can't reach STR 18/30, go for STR 18/20 instead, which should
  give you 2 attacks with a war hammer, your starting weapon. Then increase
  CON and WIS as high as possible, INT as low as possible, spare points (you
  shouldn't have any ;)) into CHR. (Paladins use WIS for casting spells.)
  If for some reason you can't even reach STR 18/20, at least go for STR 18/10,
  again DEX not below 10, then right after spawning in town discard the war
  hammer you start with and instead go buy a whip from store '3' (weaponsmith)
  or '4' (temple). This should give you a solid 2 attacks per round.
 If you decided to create a rogue, ensure that you have at least a STR of 15,
  and a pretty high DEX (the rogue's main stat) of 18/10. This will give you
  3 attacks per round when dual-wielding your light starter weapons.
  Rest goes into CON or INT (rogue's main stat for spell-casting).
 If you made a druid, put couple of points into all, DEX, STR and CON, and keep
  your WIS relatively high since it's important for druidic spell-casting.
  Some druids might value high starting WIS more than any of the other stats
  for that reason.
 Explanation:
   CON affects your hit points, more CON -> more HP.
   STR (and DEX) determine your number of attacks per round. 18/30 in STR is
   enough for warriors to get 3 attacks per round with very light weapons
   (daggers or whips) regardless of how you modify your DEX.
   To get the 3 blows per round (BpR) you'll need to sell your heavy sword and
   buy a light weapon (3.0 lb or less) like a dagger or whip. That is more
   efficient than 1 or 2 BpR with your original sword! Later on when your
   character becomes more powerful you can switch back to heavier weaponry.
   Warriors don't really need INT so you should reduce it to spend the points
   on other attributes. WIS isn't really needed either, but it can be useful
   to endure sanity-reducing effects and increase your saving throw against
   certain detrimental effects.
 Details about how STR and DEX affect startup blows/round are given in (7.5a).)


 ---- PLEASE NOTE: ----
 If you are new to the game, it is NOT recommended to choose one of these:
  Istar, Priest, Shaman, Adventurer, Runemaster.
 The reason is that these classes are hard to play in the beginning -
  compare introductory explanation in (7.5)!
 Recommended for new players are:
  Warrior, Rogue, Paladin, Druid and maybe Archer.


-Make sure you DO NOT chose PvP mode. It's a restricted special thing that
 has nothing to do with the normal gameplay at all! (See (7.13) for info).

-It's strongly recommended to not choose 'fruit bat' body either :).

 Now you're in the game and should see the town of Bree.

 Note that all following keys mentioned assume that you use the normal
 (aka non-roguelike) keyset! If you use rogue-like keyset, refer to (0.2) for
 a full list of command keys under that setting.

-To chat, press the colon key ':' . Don't hesitate to ask other players
 if you have any problems with getting started.
 (For details about chat features see (3.3).)

-To run, press SHIFT + <direction key> where direction keys are on
 the number pad 7,8,9, 4,6, 1,2,3. Running often requires that
 Numlock is turned off (press the 'Num' key above your number key pad on the
 right side of your keyboard, so the green light is off).
 If SHIFT+<direction> doesn't work on your system, you can press the dot '.'
 followed by a direction key. (If you use rogue-like keys, see section (0.2)!)
 You can also use the arrow keys to move, but the numpad is better since it
 allows you to move diagonally too, which is very important.

-You can wear/wield your armour/weapons by pressing 'w' and choosing them
 from your inventory which is displayed in the bottom right window.
 To take off an equipped item, press 't'. The item will be returned to your
 backpack (aka your inventory).

-You can pick up items if you stand on them and press 'g'. Items that are
 especially valuable in the beginning are magic wands and staves, so make sure
 to pick those up, identify them with a scroll of identify, and sell them to
 the magic shop in town.

-You can drop items with 'd' or destroy items with 'k'.

-You can enter stairs down/upwards by standing on them and pressing the
 corresponding key '<' or '>'.

-You can press '?' to get a short in-game help. The actual list of commands
 and further info are contained in this guide.

-Nearby the starter town (Bree) there's often a 'training tower' which is a
 good place to start because if you die there nothing will happen except a
 slight loss of experience and gold. Ask people where the dark-green '<' symbol
 (the tower entrance) is found. Usually to the lower-right of the town centre.

-If you die outside of the training tower you will usually turn into a ghost.
 This ghost cant wear any items, so it's a good idea to care about being
 revived. To get revived, either visit the temple in town (it's a green '4')
 or ask another player to read a 'Scroll of Life' while standing next to you.
 While you're a ghost you can freely float up/down by pressing '<' and '>'.

-If you have any parchments like 'Guide to Middle Earth' or 'The Gazette' in
 your inventory (bottom right window) you can read those by pressing 'r' to
 read. They don't do anything, just a short read containing some information.

-If your character gains a new level it also gains skill points. To distribute
 those, invoke the skill screen with SHIFT+G. (See (3.10), (7.2) and (8.5).)

-If you don't like your character and want to delete it, you may commit suicide
 by pressing SHIFT+q or CTRL+k and double-confirming the following questions
 with 'y' and '@'.

-Press '@' key to see a list of players who're currently online.

 A grey '(O)' is in front of a player name, it means that player is using an
 outdated client. It's very important to keep your client up to date!

 If another player's character level is displayed in light blue colour, that
 means he is within sensible level range of you to form a party! Press SHIFT+P
 for party menu. One of you has to create the party and add the other one.

 A name displayed in dark blue is an admin.
 A name displayed in purple is a 'winner', ie a player who managed to beat the
 game boss, Morgoth, the Lord of Darkness (and hasn't died since).
 A name displayed in red is a ghost, ie a player who died. :(
 A name displayed in dark grey is a player who is currently inside the special
 'Ironman Deep Dive Challenge' dungeon.

-A few other important keys are:
 ~   knowledge menu (all of your character's knowledge, plus artifact/monster
     spoilers under ~6 and ~7 respectively.) Or type "/know" in chat.
 =   game options menu.
 %   macro menu ('macros' are hotkeys, allowing you to bind certain actions to
     a single key press). (More about macros in (3.6) up to (3.9b).)
 SHIFT+P  form a party with your buddies. You share experience in a party and
          you can always see each other on the map and your team mates' current
          world location in the player list (accessed with '@' key).
          (SHIFT+O if you are using rogue-like keyset.)
 SHIFT+L  look around the map. Very useful to locate team mates ;).
          (CTRL+W if you are using rogue-like keyset.)
 CTRL+U   the audio mixer menu (the hotkeys for toggling master/music work
          outside of this menu too).
          (CTRL+F if you are using rogue-like keyset.)
 CTRL+Q   quit the game. Safest way is to quit in a town, not in the dungeon!
