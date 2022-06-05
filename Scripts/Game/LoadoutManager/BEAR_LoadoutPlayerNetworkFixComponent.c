[ComponentEditorProps(category: "BEAR/Components", description: "Sends the request to change loadouts to the server.", color: "0 0 255 255")]
class BEAR_LoadoutPlayerNetworkFixComponentClass: ScriptComponentClass
{
};


// This component it put on the prefab of each player. 
class BEAR_LoadoutPlayerNetworkFixComponent : ScriptComponent
{	
	protected ref array<RplId> _waitForArray = new array<RplId>();
	
	// This method is called from the server and tells the client to equip an item.
	void EquipFromServer(IEntity item)
	{
		RplComponent itemRpl = RplComponent.Cast(item.FindComponent(RplComponent));
		RplId itemId = itemRpl.Id();
		int playerId = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(GetOwner());
		
		// Call locally in case it's singleplayer
		Rpc_HandleEquip(itemId, playerId);
		// Call as Rpc Broadcast in case this is the server
		Rpc(Rpc_HandleEquip, itemId, playerId);
	}
	
	
	// Broadcast makes the rpc call be executed on all clients. 
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void Rpc_HandleEquip(RplId itemId, int playerId)
	{
		// We need to check for the playerId, so that not all connected clients suddenly change the loadout
		int localPlayerId = GetGame().GetPlayerController().GetPlayerId();
		if(playerId != localPlayerId) return;
		
		// We don't already equip the stuff here. Instead we cache the items that need to be equipped.
		// This is because the items do not exist on the client yet. Check the EOnFrame method for equipping.
		_waitForArray.Insert(itemId);
		Print("Client now waiting for ItemId " + itemId);
	}
	
	// We need the OnPostInit and EOnInit for the EOnFrame to be called.
	protected override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		SetEventMask(owner, EntityEvent.INIT);
	}
	protected override void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		SetEventMask(owner, EntityEvent.FRAME);
	}
	
	// This method is called every frame.
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		array<RplId> toRemove = {};
		
		foreach(RplId id : _waitForArray)
		{
			Managed anything = Replication.FindItem(id);
			
			// Check if the client has spawned the item that was created on the server yet.
			if(anything == null) continue;
			
			RplComponent itemRpl = RplComponent.Cast(anything);
			IEntity item = itemRpl.GetEntity();
			if(item != null)
			{
				SCR_CharacterInventoryStorageComponent StorageManager = SCR_CharacterInventoryStorageComponent.Cast(GetOwner().FindComponent(SCR_CharacterInventoryStorageComponent));
				SCR_InventoryStorageManagerComponent InventoryManager = SCR_InventoryStorageManagerComponent.Cast(GetOwner().FindComponent(SCR_InventoryStorageManagerComponent));
				
				if(item.FindComponent(WeaponComponent))
				{
					// Quirky fix for animation bug when deleting a weapon entity on the server.
					CharacterControllerComponent.Cast(GetOwner().FindComponent(CharacterControllerComponent)).SelectWeapon(null);
					
					// equip as weapon if the item has a WeaponComponent
					InventoryManager.EquipWeapon(item);
					toRemove.Insert(id);
				}
				else
				{
					bool successItem = InventoryManager.TryInsertItem(item);
					bool successEquip = InventoryManager.EquipAny(StorageManager, item);
					
					if(successItem || successEquip) 
					{
						toRemove.Insert(id);
					}
				}
			}
		}
		
		
		foreach(RplId id : toRemove)
		{
			_waitForArray.RemoveItem(id);
		}
	}
}