class TKY_VehicleLoad : ScriptedUserAction
{
	private ref array<IEntity> surroundingItems = new array<IEntity>;
	
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		// get vic storage
		SCR_VehicleInventoryStorageManagerComponent storageMgrComp = SCR_VehicleInventoryStorageManagerComponent.Cast(pOwnerEntity.FindComponent(SCR_VehicleInventoryStorageManagerComponent));
		//BaseInventoryStorageComponent storageComp = BaseInventoryStorageComponent.Cast(pOwnerEntity.FindComponent(BaseInventoryStorageComponent));

		private ref array<BaseInventoryStorageComponent> storages = new array<BaseInventoryStorageComponent>;
		storageMgrComp.GetStorages(storages);
		
		if (storages.Count() < 1) 
		{
			// no storage i guess
			// error
			Print("no storage componen");
			return;
		}
				
		GetGame().GetWorld().QueryEntitiesBySphere(
	        pOwnerEntity.GetOrigin(),
	        10.0,
	        CheckItemAddToArray,
	        FilterItems,
	        EQueryEntitiesFlags.DYNAMIC
    	);
		
		if (surroundingItems.Count() < 1)
		{
			Print("all items picked up");
			return;
		}
		
		foreach (IEntity item : surroundingItems)
		{
			//bool canInsert = storageMgrComp.CanInsertItem(item);
			//bool inserted = storageMgrComp.TryMoveItemToStorage(item, storages.Get(0));
			bool inserted = storageMgrComp.TryInsertItem(item);
			
			if (!inserted)
			{
				//Print("couldnt insert " + item);
			}
		}
		
		surroundingItems.Clear();
	}
	
	bool CheckItemAddToArray(IEntity entity) {
		//Print(entity);
	    if (entity) {
	        surroundingItems.Insert(entity);
	        return true;
	    }
	    return true; // idk we want to always continue i think? maybe extra check for if no space then dont cont?
	}
	
	protected bool FilterItems(IEntity entity) 
	{	
		if (GenericEntity.Cast(entity))
		{
			return true;
		}
		return false;
	}
}


class TKY_VehicleUnload : ScriptedUserAction
{
	private ref array<IEntity> availableArsenals = new array<IEntity>;
	
	protected bool FilterItems(IEntity entity) 
	{	
		if (GenericEntity.Cast(entity))
		{
			return true;
		}
		return false;
	}
	
	bool CheckArsenalAddToArray(IEntity entity) {
	    if (entity && entity.FindComponent(TKY_InventoryStorageManagerComponent)) {
	        availableArsenals.Insert(entity);
        	return false;
	    }
	    return true; // giga stupid i dont get how this shit works but we want to continue even if we didnt find sumn
	}
	
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		// vehicle storage
		private SCR_VehicleInventoryStorageManagerComponent storageMgrComp = SCR_VehicleInventoryStorageManagerComponent.Cast(pOwnerEntity.FindComponent(SCR_VehicleInventoryStorageManagerComponent));
		private BaseInventoryStorageComponent storage = BaseInventoryStorageComponent.Cast(pOwnerEntity.FindComponent(BaseInventoryStorageComponent));
		
		if (!storage) 
		{
			Print("no storage componen");
			return;
		}
		
		// find arsenal
		GetGame().GetWorld().QueryEntitiesBySphere(
	        pOwnerEntity.GetOrigin(),
	       	20.0,
	        CheckArsenalAddToArray,
	        FilterItems,
	        EQueryEntitiesFlags.DYNAMIC
    	);
		
		if (availableArsenals.Count() < 1) 
		{
			Print("no arsenals");
			return;
		}
		
		// get needed arsenal components
		private IEntity targetArsenal = availableArsenals.Get(0);
		private TKY_InventoryStorageManagerComponent targetStorageMgr = TKY_InventoryStorageManagerComponent.Cast(targetArsenal.FindComponent(TKY_InventoryStorageManagerComponent));
		private BaseInventoryStorageComponent targetStorage = BaseInventoryStorageComponent.Cast(targetArsenal.FindComponent(BaseInventoryStorageComponent));
		
		// get all items from vic storage
		ref array<IEntity> outItems = new array<IEntity>;
		storage.GetAll(outItems);
		
		// move all items
		foreach (IEntity item : outItems)
		{
			targetStorageMgr.TryMoveItemToStorage(item, targetStorage);
		}
		
		availableArsenals.Clear();
	}
}
