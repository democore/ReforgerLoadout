# ReforgerLoadout

This is an Arma Reforger Addon that redresses the player in an equipment loadout.
It's a neat example of how to spawn items on the server and then have the client equip them, as this has a few quirks you have to know about with the current arma reforger patch.

HUGE thanks to klamacz from BI, who tirelessly helped my figure all this out. Thanks so much, you're a legend!

The code is here:
https://github.com/democore/ReforgerLoadout/tree/master/Scripts/Game/LoadoutManager

The current workflow is

- User presses F on a entity that has the [BEAR_LoadoutAction](https://github.com/democore/ReforgerLoadout/blob/master/Scripts/Game/LoadoutManager/BEAR_LoadoutAction.c)
- Server receives the PerformAction and starts logic in [BEAR_LoadoutAction.c](https://github.com/democore/ReforgerLoadout/blob/ee109bff8924cf09a70a23f2cc86234ce47100e4/Scripts/Game/LoadoutManager/BEAR_LoadoutAction.c#L21)
- Server spawns items at the player location [BEAR_LoadoutAction.c](https://github.com/democore/ReforgerLoadout/blob/ee109bff8924cf09a70a23f2cc86234ce47100e4/Scripts/Game/LoadoutManager/BEAR_LoadoutAction.c#L88)
- Tells the player they were spawned with Rpc [BEAR_LoadoutAction.c](https://github.com/democore/ReforgerLoadout/blob/ee109bff8924cf09a70a23f2cc86234ce47100e4/Scripts/Game/LoadoutManager/BEAR_LoadoutAction.c#L91)
- The client then waits until the items are replicated [BEAR_LoadoutPlayerNetworkFixComponent.c](https://github.com/democore/ReforgerLoadout/blob/ee109bff8924cf09a70a23f2cc86234ce47100e4/Scripts/Game/LoadoutManager/BEAR_LoadoutPlayerNetworkFixComponent.c#L56)
- Once the items are replicated, they are equip on client side [BEAR_LoadoutPlayerNetworkFixComponent.c](https://github.com/democore/ReforgerLoadout/blob/ee109bff8924cf09a70a23f2cc86234ce47100e4/Scripts/Game/LoadoutManager/BEAR_LoadoutPlayerNetworkFixComponent.c#L70)


Note:
As of right now, this addon uses 4 dependencies from the workshop. I'll remove them when I have time. Until then, you can manually remove them by removing them from the addons settings and the resources from the [config file](https://github.com/democore/ReforgerLoadout/blob/master/Configs/Loadout_MTC.conf).
