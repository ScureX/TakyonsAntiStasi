class TKY_Location
{
	IEntity iEntity;
	TKY_AntiStasiFactionEnum faction;
	
	TKY_AntiStasiFactionEnum capturingFaction = null;
	const int MIN_HOLDING_TIME = 100;
	int holdingTime = 0;
	bool allowCapture = false;
	
	ref array<SCR_AIGroup> defendingForce = {};
	protected bool defendingForceSpawned = false;
	ref array<SCR_AIGroup> qrForce = {};
	protected bool qrForceSpawned = false;
	
	const int PLAYER_MIN_DISTANCE_TO_SPAWN = 1000;
	bool isPlayerInSpawnDistance = false;
	const int LOCATION_RADIUS = 100; // radius thats actually the location (spawn enemies, capture) // TODO scale with obj size 
	bool isPlayerInLocationDistance = false;
	
	void TKY_Location(IEntity entity, TKY_AntiStasiFactionEnum faction)
	{
		this.iEntity = entity;
		this.faction = faction;
	}
	
	void RunChecks()
	{
		CheckPlayerNear();
		CheckPlayerInLocation();
		RemoveNullsFromArrays();
		SpawnDefenseForce();
		SpawnQRForce();
		UpdateFaction();
		UpdateMap();
	}
	
	protected void RemoveNullsFromArrays()
	{
		for (int i = 0; i < defendingForce.Count(); i++)
		{
			if (defendingForce[i] == null)
				defendingForce.Remove(i);
		}
		
		for (int i = 0; i < qrForce.Count(); i++)
		{
			if (qrForce[i] == null)
				qrForce.Remove(i);
		}
	}
	
	// check if a player is near enough to warrant spawning enemies
	protected void CheckPlayerNear()
	{
		private ref array<IEntity> nearbyCharacters = new array<IEntity>();
		private array<int> players = new array<int>(); 
		GetGame().GetPlayerManager().GetPlayers(players);
		
		foreach (int playerid : players)
		{
			IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerid);
			
			if (!player)
				continue;

			if (vector.DistanceSqXZ(player.GetOrigin(), iEntity.GetOrigin()) <= (PLAYER_MIN_DISTANCE_TO_SPAWN * PLAYER_MIN_DISTANCE_TO_SPAWN))
			{
				//Print("Player near " + iEntity.GetName());
				isPlayerInSpawnDistance = true;
				return; // one is enough
			}
		}
		
		isPlayerInSpawnDistance = false;
		
		// nobody near, clear defenders
		if (defendingForce.Count() > 0)
		{
			foreach (SCR_AIGroup defender : defendingForce)
			{
				delete defender;
			}
			defendingForce.Clear();
			defendingForceSpawned = false;
		}
		
		// nobody near, clear qrf
		if (qrForce.Count() > 0)
		{
			foreach (SCR_AIGroup defender : qrForce)
			{
				delete defender;
			}
			qrForce.Clear();
			qrForceSpawned = false;
		}
	}
	
	
	// check if a player is near enough to warrant spawning enemies
	protected void CheckPlayerInLocation()
	{
		private ref array<IEntity> nearbyCharacters = new array<IEntity>();
		private array<int> players = new array<int>(); 
		GetGame().GetPlayerManager().GetPlayers(players);
		
		foreach (int playerid : players)
		{
			IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerid);
			
			if (!player)
				continue;

			if (vector.DistanceSqXZ(player.GetOrigin(), iEntity.GetOrigin()) <= (LOCATION_RADIUS * LOCATION_RADIUS))
			{
				//Print("Player inside " + iEntity.GetName());
				isPlayerInLocationDistance = true;
				capturingFaction = TKY_AntiStasiManagerComponent.playerFaction;
				return; // one is enough
			}
		}
		
		isPlayerInLocationDistance = false;
	}
	
	
	protected void SpawnDefenseForce()
	{
		if (!isPlayerInSpawnDistance)
			return;
		
		if (defendingForce.Count() > 0)
			return;
		
		if (defendingForceSpawned)
			return;
		
		// if faction is player faction? mayb as long as you cant spawn ur own force there
		if (this.faction == TKY_AntiStasiManagerComponent.playerFaction)
			return;
		
		TKY_Spawner spawner = TKY_Spawner();
		
		// Define how many groups to spawn based on the radius
	    // For example, spawn 1 group for every 50 units of radius (this can be adjusted)
	    int numberOfGroups = Math.Ceil(LOCATION_RADIUS / 50.0);
	
	    // Loop to spawn the groups
	    for (int i = 0; i < numberOfGroups; i++)
	    {
			ref array<string> prefabs = TKY_AntiStasiFaction.GetPrefabsForFaction(this.faction);
			
			// no prefabs in this faction
			if (prefabs.Count() < 1)
				break;
			
	        // Call the spawner to spawn a prefab within the radius
	        SCR_AIGroup defender = spawner.SpawnPrefabWithinRadius(
				prefabs[Math.RandomInt(0, prefabs.Count())],
	            iEntity.GetOrigin(), 
	            LOCATION_RADIUS
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
	
	protected void SpawnQRForce()
	{
		if (!isPlayerInSpawnDistance)
			return;
		
		if (!isPlayerInLocationDistance)
			return;
		
		if (this.faction == TKY_AntiStasiManagerComponent.playerFaction)
			return;
		
		if (qrForceSpawned)
			return;
		
		if (defendingForce.Count() < 1)
			return;
		
		TKY_Spawner spawner = TKY_Spawner();
	
		// Define how many groups to spawn based on the radius
	    // For example, spawn 1 group for every 50 units of radius (this can be adjusted)
	    int numberOfGroups = Math.Ceil(LOCATION_RADIUS / 20.0);
	
	    // Loop to spawn the groups
	    for (int i = 0; i < numberOfGroups; i++)
	    {
			ref array<string> prefabs = TKY_AntiStasiFaction.GetPrefabsForFaction(this.faction);
			
			// no prefabs in this faction
			if (prefabs.Count() < 1)
				break;
			
	        // Call the spawner to spawn a prefab within the radius
	        SCR_AIGroup defender = spawner.SpawnPrefabWithinRadius(
				prefabs[Math.RandomInt(0, prefabs.Count())],
	            iEntity.GetOrigin(), 
	            LOCATION_RADIUS + 200,
				LOCATION_RADIUS + 100
	        );
	
	        // If the group was successfully spawned, add it to the defendingForce array
	        if (defender)
	        {
	            //Print("Group " + i + " spawned successfully.");
	            qrForce.Insert(defender);
	        }
	        else
	        {
	            //Print("Failed to spawn group " + i);
	        }
	    }
		
		qrForceSpawned = true;
		allowCapture = true;
	}
	
	protected void UpdateFaction()
	{
		if (!isPlayerInLocationDistance)
			return;
		
		if (capturingFaction == faction)
			return;
		
		if (!qrForceSpawned)
			return;
		
		if (!allowCapture)
			return;
		
		if (defendingForce.Count() > 0)
			return;
		
		if (qrForce.Count() > 0)
			return;
		
		if (holdingTime < MIN_HOLDING_TIME)
		{
			Print("Holding time " + holdingTime + " smaller than minimum " + MIN_HOLDING_TIME);
			holdingTime++;
			return;
		}
		
		this.faction = capturingFaction;
		Print("faction of " + iEntity.GetName() + " changed to " + this.faction);
	}
	
	protected void UpdateMap()
	{
		SCR_MapDescriptorComponent descr = SCR_MapDescriptorComponent.Cast(iEntity.FindComponent(SCR_MapDescriptorComponent));
		if (descr)
		{
			MapItem m_MapItem = descr.Item();
			MapDescriptorProps props = m_MapItem.GetProps();
			props.SetTextColor(TKY_AntiStasiFaction.GetColorForFaction(this.faction));
			props.Activate(true);
		}
	}
}