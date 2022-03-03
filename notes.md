# Notes

## Particle Priority

Each particle is given a "super" (short for 'superiority') rating that determines which particles it is allowed to be painted over, effectively replacing those particles.

For the most part, the rule is that solids can be painted over liquids, and 'Empty' can be painted over anything.

For convenience, I've split the materials up into groups. A particle can paint over any particle in a group lower than its own, as well as the particles in its own group.

```
if (this.super >= other.super) { ... }
```

Ah yes, everyone knows the states of matter. Solid, liquid, gas, powder, and weirdo. You know, science.

1. Meta
	- Empty - Literally nothing
2. Weirdos
	- Nanobots - Destroys everything
	- Fire - Burns flammable things
3. Solids
	- Stone - Doesn't move or do anything fun
	- Plant - Grows when watered, fairly flammable
4. Powders
	- Sand - Displaces water
	- Salt - Dissolves in water
5. Liquids
	- Water - Non-flammable, can be evaporated
	- Oil - Highly flammable liquid
6. Gasses
	- Smoke - By-product of fire + anything flammable
	- Steam - By-product of fire + water)
