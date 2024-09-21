class TKY_Spawner
{
	/*SCR_AIGroup SpawnWithDelay(func fn, int delay,bool repeat = false, void param1 = NULL, void param2 = NULL, void param3 = NULL)
	{
		//proto void CallLater(func fn, int delay = 0, bool repeat = false, void param1 = NULL,..);	
		GetGame().GetCallqueue().CallLater(
			fn,
			delay,
			repeat,
			param1,
			param2,
			param3
		);
	}*/
	
	
	SCR_AIGroup SpawnPrefabAtLocation(string prefab, vector location)
	{
	    World world = GetGame().GetWorld();
	    if (!world)
	    {
	        Print("World context not found.");
	        return null;
	    }

		// Create the prefab spawn parameters (can be customized as needed)
	    EntitySpawnParams params = new EntitySpawnParams();
	    params.TransformMode = ETransformMode.WORLD;
	    params.Transform[3] = location; // Set the position to spawn the prefab
	    
	    // Spawn the prefab at the random location
	    SCR_AIGroup newEntity = SCR_AIGroup.Cast(CreatePrefab(prefab, world, params));
	    
	    // Check if the prefab was spawned successfully
	    if (!newEntity)
	    {
	        Print("Failed to spawn prefab!");
			return null;
	    }
	    
	    return newEntity;
	}
	
	SCR_AIGroup SpawnPrefabWithinRadius(string prefab, vector origin, int radius, int minDistance = 0)
	{
	    World world = GetGame().GetWorld();
	    if (!world)
	    {
	        Print("World context not found.");
	        return null;
	    }
		
		vector randomPosition = TKY_Util.GetRandomPositionWithinRadiusWithExclusion(origin, minDistance, radius);
		
		// Create the prefab spawn parameters (can be customized as needed)
	    EntitySpawnParams params = new EntitySpawnParams();
	    params.TransformMode = ETransformMode.WORLD;
	    params.Transform[3] = randomPosition; // Set the position to spawn the prefab
	    
	    // Spawn the prefab at the random location
	    SCR_AIGroup newEntity = SCR_AIGroup.Cast(CreatePrefab(prefab, world, params));
	    
	    // Check if the prefab was spawned successfully
	    if (!newEntity)
	    {
	        Print("Failed to spawn prefab!");
			return null;
	    }
	    
	    return newEntity;
	}
	
	protected IEntity CreatePrefab(ResourceName prefab, BaseWorld myWorld, EntitySpawnParams params)
	{
		Resource res = Resource.Load(prefab);
		return GetGame().SpawnEntityPrefab(res, myWorld, params);
	}
}