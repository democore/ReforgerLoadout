class BEAR_LoadoutAction : ScriptedUserAction
{
	protected ref BEAR_LoadoutItemListConfig m_LoadoutConfig;
	
	override event bool GetActionNameScript(out string outName)
	{
		outName = "Get Legion Loadout";
		return true;
	};
	
	override event bool GetActionDescriptionScript(out string outName) 
	{ 
		outName = "Premade Loadout";
		return true;
	};
	
	// These 2 overrides and their return values ensure, that only the server calls PerformAction.
    override event bool HasLocalEffectOnlyScript() { return false; };
    override event bool CanBroadcastScript() { return false; };
	
	override event void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		DoHandleLoadout(pUserEntity);
	}	
	
	// Only called on server, see above why
	protected void DoHandleLoadout(IEntity player)
	{		
		// Load a config that contains the loadout we want to equip on the player.
		m_LoadoutConfig = BEAR_LoadoutItemListConfig.Cast(BaseContainerTools.CreateInstanceFromContainer(Resource.Load("{0697686259BB81E3}Configs/Loadout_MTC.conf").GetResource().ToBaseContainer()));
		
		// Delete all Equipment on the player.
		DeleteAllItems(player);
		RemoveAllWeapons(player);
		
		SpawnAndEquip(m_LoadoutConfig.GetRifleResource(), player);
		
		SpawnAndEquip(m_LoadoutConfig.GetHelmetResource(), player);
		SpawnAndEquip(m_LoadoutConfig.GetShirtResource(), player);
		SpawnAndEquip(m_LoadoutConfig.GetVestResource(), player);
		SpawnAndEquip(m_LoadoutConfig.GetPantsResource(), player);
		SpawnAndEquip(m_LoadoutConfig.GetShoesResource(), player);	
		SpawnAndEquip(m_LoadoutConfig.GetCompassResource(), player);	
		SpawnAndEquip(m_LoadoutConfig.GetBinocularsResource(), player);	
		SpawnAndEquip(m_LoadoutConfig.GetMapResource(), player);	
		SpawnAndEquip(m_LoadoutConfig.GetBackpackResource(), player);
		
		foreach(ResourceName name : m_LoadoutConfig.GetOther())
		{
			SpawnAndEquip(Resource.Load(name), player);
		}
	}
	
	// Go through all the weapons of the player and delete their entitys.
	// This works because this is always called from the server.
	protected void RemoveAllWeapons(IEntity player)
	{
		BaseWeaponManagerComponent weaponManager = BaseWeaponManagerComponent.Cast(player.FindComponent(BaseWeaponManagerComponent));
		
		array<IEntity> weapons = {};
		weaponManager.GetWeaponsList(weapons);
		
		foreach(IEntity weapon: weapons)
		{
			InventoryItemComponent pInvComp = InventoryItemComponent.Cast( weapon.FindComponent( InventoryItemComponent ) );
			if (!pInvComp)
				continue;
			InventoryStorageSlot parentSlot = pInvComp.GetParentSlot();
			if (!parentSlot)
				continue;
			
			// Caution, this will ONLY work when called on the server because deleting a prefabis only allowed on the server.
			SCR_EntityHelper.DeleteEntityAndChildren(weapon);
		}
	}
	
	// Spawn an item on the server and tell the client to equip it.
	protected void SpawnAndEquip(Resource itemResource, IEntity player)
	{
		// It is important to move the items to the players location, so that they will not be ignored because of distance
		// Note, that we can't move the items with item.SetWorldTransform after creating them, because this requires a NwkMovementComponent on their entitys, which they usually don't have.
		EntitySpawnParams spawnParams = new EntitySpawnParams();
		vector mat[4];
		player.GetWorldTransform(mat);
		spawnParams.Transform = mat;
		
		// Caution, this will ONLY work when called on the server because creating a prefab is only allowed on the server.
		IEntity item = GetGame().SpawnEntityPrefab(itemResource, null, spawnParams);
				
		BEAR_LoadoutPlayerNetworkFixComponent loadoutNetwork = BEAR_LoadoutPlayerNetworkFixComponent.Cast(player.FindComponent(BEAR_LoadoutPlayerNetworkFixComponent));
		loadoutNetwork.EquipFromServer(item);
	}
	
	// Delete all entities of equipped items on the player.
	// This works because this is always called from the server.
	protected void DeleteAllItems(IEntity player)
	{
		SCR_CharacterInventoryStorageComponent StorageManager = SCR_CharacterInventoryStorageComponent.Cast(player.FindComponent(SCR_CharacterInventoryStorageComponent));
		
		array<IEntity> items = {};
		StorageManager.GetAll(items);
		foreach(IEntity currentItem: items)
		{
			// Caution, this will ONLY work when called on the server because it deletes a prefab.
			SCR_EntityHelper.DeleteEntityAndChildren(currentItem);
		}
	}
}