class TKY_Location
{
	/*IEntity iEntity;
	TKY_AntiStasiFactionEnum faction;
	
	TKY_AntiStasiFactionEnum capturingFaction = null;
	const int MIN_HOLDING_TIME = 10;
	int holdingTime = 0;
	bool allowCapture = false;
	
	ref array<SCR_AIGroup> defendingForce = {};
	protected bool defendingForceSpawned = false;
	ref array<SCR_AIGroup> qrForc = {};
	protected bool qrForceSpawned = false;
	
	const int PLAYER_MIN_DISTANCE_TO_SPAWN = 1000;
	bool isPlayerInSpawnDistance = false;
	const int LOCATION_RADIUS = 100; // radius thats actually the location (spawn enemies, capture) // TODO scale with obj size 
	bool isPlayerInLocationDistance = false;*/
	
	
	//
	
	IEntity locationEntity;
	ref TKY_Faction faction;
	
	ref TKY_LocationAttackHandler attackHandler = null;
	ref array<SCR_AIGroup> civvies = {};
	
	const int PLAYER_MIN_DISTANCE_TO_SPAWN = 1000;
	const int LOCATION_RADIUS = 50; // radius thats actually the location (spawn enemies, capture) // TODO scale with obj size
	int reputation = 0;
	
	//bool isPlayerInSpawnDistance = false;
	bool isUnderAttack = false; // set to true if attacked to not unload attack just bcs player is too far
	
	void TKY_Location(IEntity _entity, TKY_Faction _faction)
	{
		this.locationEntity = _entity;
		this.faction = _faction;
	}
	
	// runs every second
	void RunChecks()
	{
		CheckPlayerNear();
		UpdateMap();
	}
	
	// check if a player is near enough to warrant spawning enemies
	protected void CheckPlayerNear()
	{
		if (isUnderAttack)
			return; // we dont care
		
		private ref array<IEntity> nearbyCharacters = new array<IEntity>();
		private array<int> players = new array<int>(); 
		GetGame().GetPlayerManager().GetPlayers(players);
		
		foreach (int playerid : players)
		{
			IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerid);
			
			if (!player)
				continue;

			if (vector.DistanceSqXZ(player.GetOrigin(), locationEntity.GetOrigin()) <= (PLAYER_MIN_DISTANCE_TO_SPAWN * PLAYER_MIN_DISTANCE_TO_SPAWN))
			{
				//Print("Player near " + iEntity.GetName());
				//isPlayerInSpawnDistance = true;
				GetGame().GetCallqueue().CallLater(
					this.PlayerNearTrigger,
					1000,
					true
				);
				return; // one is enough
			}
		}
		
		GetGame().GetCallqueue().Remove(this.PlayerNearTrigger);
		UnloadLocation();
	}
	
	// runs every second if player is near
	protected void PlayerNearTrigger()
	{
		// TODO spawn civvies // keep amount topped up
		for (int i = civvies.Count(); i < LOCATION_RADIUS / 10; i++)
		{
			SCR_AIGroup civ = TKY_Spawner.SpawnPrefabWithinRadius(
				"{464DE4EEF0F155CE}Prefabs/Groups/CIV/Group_Civ.et",
				locationEntity.GetOrigin(),
				LOCATION_RADIUS
			);
			
			SCR_DefendWaypoint newWP = SCR_DefendWaypoint.Cast(
				TKY_Spawner.CreatePrefab(
					"{D9C14ECEC9772CC6}PrefabsEditable/Auto/AI/Waypoints/E_AIWaypoint_Defend.et", 
					GetGame().GetWorld(), 
					TKY_Spawner.GetEntitySpawnParams(locationEntity)
				)
			);
		
			newWP.SetOrigin(locationEntity.GetOrigin());
			newWP.SetCompletionRadius(LOCATION_RADIUS * 2);
			civ.AddWaypoint(newWP);
			
			civvies.Insert(civ);
		}
		
		// if its a non player city we assume attack
		if (this.faction != TKY_AntiStasiManagerComponent.playerFaction && attackHandler == null)
		{
			attackHandler = new TKY_LocationAttackHandler(this);
			GetGame().GetCallqueue().CallLater(
				this.AttackHandlerLifeCycle,
				1000,
				true
			);
		}
			
	}
	
	protected void AttackHandlerLifeCycle()
	{
		if (attackHandler == null)
			return;
		
		if (attackHandler.qrfHandler == null)
			return;
		
		if (attackHandler.qrfHandler.qrfEnded)
		{
			// player won the attack
			if (this.faction == TKY_AntiStasiManagerComponent.playerFaction)
			{
				GetGame().GetCallqueue().Remove(this.AttackHandlerLifeCycle);
				return;
			}
			
			// player lost the attack, reload new handler
			Print("reloading attack handler");
			if (attackHandler.qrfHandler != null)
			{
				// TODO: shit dont work getting derefd
				ref array<SCR_AIGroup> defendingForce = {};
				defendingForce.InsertAll(attackHandler.qrfHandler.qrfForce);
				
				attackHandler.Unload();
				attackHandler = new TKY_LocationAttackHandler(this, defendingForce);
			}
			else
			{
				attackHandler.Unload();
				attackHandler = new TKY_LocationAttackHandler(this);
			}
		}
	}
	
	void UpdateFaction(TKY_Faction newFaction)
	{
		this.faction = newFaction; // Change the faction to the capturing enemy
		Print("Faction of " + this.locationEntity.GetName() + " changed to " + this.faction.factionKey);
	}
	
	protected void UpdateMap()
	{
		SCR_MapDescriptorComponent descr = SCR_MapDescriptorComponent.Cast(locationEntity.FindComponent(SCR_MapDescriptorComponent));
		if (descr)
		{
			MapItem m_MapItem = descr.Item();
			MapDescriptorProps props = m_MapItem.GetProps();
 			props.SetTextColor(this.faction.color);
			props.SetOutlineColor(Color.Black);
			props.SetFont("{3E7733BAC8C831F6}UI/Fonts/RobotoCondensed/RobotoCondensed_Regular.fnt");
			//float scaledValue = ((LOCATION_RADIUS - 50) / (150 - 50)) * (45 - 25) + 25;
			float scaledValue = 20;
			int val = Math.Round(scaledValue);
			props.SetTextSize(val, val, val);
			props.SetVisible(true);
			props.Activate(true);
		}
	}
	
	protected void UnloadLocation()
	{
		if (attackHandler != null)
		{
			attackHandler.Unload();
			delete attackHandler;
			attackHandler = null;
		}
		
		GetGame().GetCallqueue().Remove(this.AttackHandlerLifeCycle);
		
		foreach (SCR_AIGroup g : civvies)
			delete g;
		
		civvies.Clear();
	}
	
	
	
	
	
	
	////////////////////////////////////////////////////////////
	
	/*
	
	
	int r = 1;
	void RunChecks()
	{
		Print("run: "+r);
		r++;
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
			props.SetOutlineColor(Color.Black);
			float scaledValue = ((LOCATION_RADIUS - 50) / (150 - 50)) * (45 - 25) + 25;
			int val = Math.Round(scaledValue);
			props.SetTextSize(val, val, val);
			props.SetVisible(true);
			props.Activate(true);
		}
	}*/
}