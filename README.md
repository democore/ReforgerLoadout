# ReforgerLoadout

This is an Arma Reforger Addon that redresses the player in an equipment loadout.
It's a neat example of how to spawn items on the server and then have the client equip them, as this has a few quirks you have to know about with the current arma reforger patch.

HUGE thanks to klamacz from BI, who tirelessly helped my figure all this out. Thanks so much, you're a legend!

The code is here:
https://github.com/democore/ReforgerLoadout/tree/master/Scripts/Game/LoadoutManager

The current workflow is

- Server spawns items at the player location
- Tells the player they were spawned with Rpc
- The client then waits until the items are replicated
- Once the items are replicated, they are equip on client side
