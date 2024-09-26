class TKY_LocationAttackHandler
{
	ref TKY_Location location;
	
	ref array<SCR_AIGroup> defendingForce = {};
	protected bool defendingForceSpawned = false;
	
	ref TKY_QRFHandler qrfHandler = null;
	
	
	void TKY_LocationAttackHandler(TKY_Location _location, array<SCR_AIGroup> _defendingForce = null)
	{
		this.location = _location;
		if (_defendingForce != null)
			this.defendingForce = _defendingForce;
		
		Print("new location attack handler at " + location.locationEntity.GetName() + " with additional spawn count: " + this.defendingForce.Count());
		SpawnDefenseForce();
	}
	
	protected void WatcherDefenseForce()
	{
		for (int i = 0; i < defendingForce.Count(); i++)
		{
			if (defendingForce[i] == null)
				defendingForce.Remove(i);
		}
		
		if (defendingForce.IsEmpty())
		{
			Print("Defense Force Dead");
			GetGame().GetCallqueue().Remove(WatcherDefenseForce);
			SpawnQRForce();
		}
	}
	
	protected void SpawnDefenseForce()
	{
		Print("Defense Force Init");
		GetGame().GetCallqueue().CallLater(
			this.WatcherDefenseForce,
			1000,
			true
		);
		
		TKY_Spawner spawner = TKY_Spawner();
		
		// Define how many groups to spawn based on the radius
	    // For example, spawn 1 group for every 50 units of radius (this can be adjusted)
	    int numberOfGroups = Math.Ceil(location.LOCATION_RADIUS / 50.0);
	
	    // Loop to spawn the groups
	    for (int i = 0; i < numberOfGroups; i++)
	    {
			ref array<ref string> prefabs = location.faction.infantryPrefabs;
			
			// no prefabs in this faction
			if (prefabs.IsEmpty())
				break;
			
	        // Call the spawner to spawn a prefab within the radius
	        SCR_AIGroup defender = spawner.SpawnPrefabWithinRadius(
				prefabs[Math.RandomInt(0, prefabs.Count())],
	            location.locationEntity.GetOrigin(), 
	            location.LOCATION_RADIUS
	        );
	
	        // If the group was successfully spawned, add it to the defendingForce array
	        if (defender)
	        {
	            //Print("Group " + i + " spawned successfully.");
	            defendingForce.Insert(defender);
	        }
	        else
	        {
	            //Print("Failed to spawn group " + i);
	        }
	    }
		
		defendingForceSpawned = true;
	}
	
	void SpawnQRForce()
	{	
		Print("QR Force Init");
		// void TKY_QRFHandler(TKY_Location _location, int _ANNOUNCEMENT_DELAY = 300, int _QR_FORCE_DURATION = 900, int _NO_PLAYER_DURATION = 300)
		qrfHandler = new TKY_QRFHandler(
			location,
			0, // start immediatly
			15,
			5
		);
		
	}
	
	void Unload()
	{
		if (qrfHandler != null)
			delete qrfHandler;
		
		foreach (SCR_AIGroup g : defendingForce)
			delete g;
	}
}