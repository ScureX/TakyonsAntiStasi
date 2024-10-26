class TKY_InventoryStorageManagerComponentClass: ScriptedInventoryStorageManagerComponentClass
{
};

class TKY_InventoryStorageManagerComponent : ScriptedInventoryStorageManagerComponent
{
	override protected void OnItemAdded(BaseInventoryStorageComponent storageOwner, IEntity item)
	{
		BaseInventoryStorageComponent arsenalStorage = storageOwner;
	    super.OnItemAdded(arsenalStorage, item);
		
		BaseInventoryStorageComponent storageComponent = BaseInventoryStorageComponent.Cast(item.FindComponent(BaseInventoryStorageComponent));
	    Print(item);
		
		if (storageComponent)
		{
				ProcessSubItems(storageComponent, arsenalStorage);
		}
		
		TryMoveItemToStorage(item, arsenalStorage);
	}
	
	protected void ProcessSubItems(BaseInventoryStorageComponent storageComponent, BaseInventoryStorageComponent arsenalStorage)
	{
		ref array<IEntity> containedItems = {};
		storageComponent.GetAll(containedItems);
		
		foreach (IEntity subItem : containedItems)
	    {
			BaseInventoryStorageComponent eqStorage = BaseInventoryStorageComponent.Cast(subItem.FindComponent(BaseInventoryStorageComponent));
			Print(subItem);
			
			if (!eqStorage)
			{
				Print("no storage, moving item");
				TryMoveItemToStorage(subItem, arsenalStorage);
			}
			else
			{
				Print("has storage, processing sub items");
				ProcessSubItems(eqStorage, arsenalStorage);
			}
	    }
	}
	
	
	/*
	override protected void OnItemAdded(BaseInventoryStorageComponent storageOwner, IEntity item)
	{
	    // Call the base class logic first
	    super.OnItemAdded(storageOwner, item);
	
	    // Try to cast the ScriptedBaseInventoryStorageComponent from the item
	    ScriptedBaseInventoryStorageComponent storageComponent = ScriptedBaseInventoryStorageComponent.Cast(item.FindComponent(ScriptedBaseInventoryStorageComponent));
	    //RHS_ClothNodeStorageComponent storageComponent = RHS_ClothNodeStorageComponent.Cast(item.FindComponent(RHS_ClothNodeStorageComponent));
		//SCR_UniversalInventoryStorageComponent storageComponent = SCR_UniversalInventoryStorageComponent.Cast(item.FindComponent(SCR_UniversalInventoryStorageComponent));
		Print(item);
	
	    // If the item does not have a ScriptedBaseInventoryStorageComponent, return
	    if (!storageComponent)
	        return;
	
	    // Process the item recursively
	    //ProcessSubItems(storageComponent, storageOwner);
		GetGame().GetCallqueue().Call(Batch, storageComponent, storageOwner);
		
		//ref array<IEntity> containedItems = {};
		//storageComponent.GetAll(containedItems);
		
		//foreach (IEntity subItem : containedItems)
	    //{
		//	TryMoveItemToStorage(subItem, storageOwner);
	    //}
	}
	
	protected void Batch(ScriptedBaseInventoryStorageComponent storageComponent, BaseInventoryStorageComponent storageOwner)
	{
		ref array<IEntity> containedItems = {};
		storageComponent.GetAll(containedItems);
		
		foreach (IEntity subItem : containedItems)
	    {
			//RHS_EquipmentStorageComponent eqStorage = RHS_EquipmentStorageComponent.Cast(subItem.FindComponent(RHS_EquipmentStorageComponent));
			SCR_UniversalInventoryStorageComponent eqStorage = SCR_UniversalInventoryStorageComponent.Cast(subItem.FindComponent(SCR_UniversalInventoryStorageComponent));
			Print(subItem);
			if (!eqStorage)
			{
				Print("no storage, moving item");
				TryMoveItemToStorage(subItem, storageOwner);
			}
			else
			{
				Print("has storage, processing sub items");
				GetGame().GetCallqueue().Call(Batch, eqStorage, storageOwner);
			}
	    }
	}
	
	protected void ProcessSubItems(ScriptedBaseInventoryStorageComponent storageComponent, BaseInventoryStorageComponent storageOwner)
	{
		ref array<IEntity> containedItems = {};
		storageComponent.GetAll(containedItems);
		
	    // Loop through each sub-item in the component
	    foreach (IEntity subItem : containedItems)
	    {
	        // Try to insert the sub-item into the target storage
			//TryMoveItemToStorage(IEntity item, BaseInventoryStorageComponent to, int slotID = -1, InventoryOperationCallback cb = null);
	        //bool inserted = TryInsertItemInStorage(subItem, storageOwner);
			
	
	        // If the item was successfully inserted, remove it from the current storage to avoid duplication
	        //if (inserted)
	        //{
	        //   TryRemoveItemFromStorage(subItem, storageComponent);
	        //}
	
	        // Recursively check if the sub-item also has a ScriptedBaseInventoryStorageComponent
	        //ScriptedBaseInventoryStorageComponent subStorageComponent = ScriptedBaseInventoryStorageComponent.Cast(subItem.FindComponent(ScriptedBaseInventoryStorageComponent));
	
	        //if (subStorageComponent)
	       // {
	            // Recursively process the sub-items
	        //    ProcessSubItems(subStorageComponent, storageOwner);
	        //}
			
			bool inserted = TryMoveItemToStorage(subItem, storageOwner);
	    }
	}*/

}