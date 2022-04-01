# Notes

## Particle Priority (not yet implemented in rev. 2)

Each particle is given a "super" (short for 'superiority') rating that determines which particles it is allowed to be painted over, effectively replacing those particles.

For the most part, the rule is that solids can be painted over liquids, and 'Empty' can be painted over anything.

For convenience, I've split the materials up into groups. A particle can paint over any particle in a group lower than its own, as well as the particles in its own group.

```
if (this.super >= other.super) { ... }
```

Ah yes, everyone knows the states of matter; solid, liquid, gas, powder, meta, and weirdo. You know, science.


### Some Notes

- Solids have been split into "movable" and "immovable" categories
	- Mercury is classified as a movable solid, because it is liquid when heated and solid when cooled


### Material Classifications

1. Meta
	- Empty
		- Can be painted over anything, acts as an eraser tool
2. Weirdos
	- Nanobots
		- Attempts to infect any neighboring material
		- Can die if no infectable materials are found
3. Movable Solids
	- Mercury
		- Liquid when heated, solid when cooled
		- Starts off heated, painted like a liquid
		- Extremely heavy, displaces everything but immovable solids
4. Immovable Solids
	- Stone
		- Non-moving, mostly unbreakable
		- Can be turned into lava when sufficiently heated, can create smoke
	- Plant
		- Can spread when contacting water
		- Can be killed by saltwater, does not spread to saltwater
		- Lower chance to spread to distilled water
	- Glass
		- Created when sand is sufficiently heated, can create smoke
		- Fragile, can be broken by impact
5. Powders
	- Sand
		- Settles into piles, must be disturbed to start moving again
		- Displaces liquids at medium speed
		- Can turn into glass when sufficiently heated, can create smoke
	- Salt
		- Does not settle, forms pyramid-like piles
		- Displaces liquids at medium-slow speed
		- Can dissolve in water, turning it into saltwater
		- Can dissolve in saltwater, creating nothing
		- Can dissolve in distilled water, turning it into water
6. Liquids
	- Water
		- Disperses at a high speed
		- Can be displaced by most powders
		- Can turn into steam when sufficiently heated
		- Can turn into stone when touching lava, creating steam
	- Oil
		- Dispserses at a medium-high speed
		- Can be displaced by most powders
		- Very flammable, creates smoke when burned
		- Less dense than water/saltwater/distilled water
	- Slime
		- Disperses at a medium speed
		- Can be displaced by most powders
		- Cannot be displaced by movable solids
	- Lava
		- Created when stone is sufficiently heated, can create smoke
		- Disperses at a slow speed
		- Can be displaced by very heavy materials
		- Can turn into stone when sufficiently cooled
7. Gasses
	- Fire
		- Heats materials it touches
		- Burns and spreads to nearby flammable materials, creates smoke
		- Can create smoke
	- Smoke
		- Created when flammable materials are burned
		- Chance to be created randomly by fire
	- Steam
		- Created when water is sufficiently heated or when fire touches water
		- Can turn into distilled water when cooled
