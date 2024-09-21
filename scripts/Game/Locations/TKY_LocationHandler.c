class Location
{
	IEntity iEntity;
	Faction faction;
	ref array<SCR_AIGroup> defendingForce = {};
	ref array<SCR_AIGroup> qrfForce = {};
	
	const int PLAYER_MIN_DISTANCE_TO_SPAWN = 1000;
	bool isPlayerInSpawnDistance = false;
	const int LOCATION_RADIUS = 250; // radius thats actually the location (spawn enemies, capture) // TODO scale with obj size 
	
	void Location(IEntity entity)
	{
		this.iEntity = entity;
	}
	
	void RunChecks()
	{
		CheckPlayerNear();
		SpawnDefenseForce();
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
				Print("Player near " + iEntity.GetName());
				isPlayerInSpawnDistance = true;
				return; // one is enough
			}
		}
		
		// ran thru w/out returning
		isPlayerInSpawnDistance = false;
		
		// nobody near, clear defenders
		if (defendingForce.Count() > 0)
		{
			foreach (SCR_AIGroup defender : defendingForce)
			{
				delete defender;
			}
			defendingForce.Clear();
		}
	}
	
	protected void SpawnDefenseForce()
	{
		if (!isPlayerInSpawnDistance)
			return;
		
		if (defendingForce.Count() > 0)
			return;
		
		// if faction is player faction? mayb as long as you cant spawn ur own force there
		
		TKY_Spawner spawner = TKY_Spawner();
		SCR_AIGroup defender = spawner.SpawnPrefabWithinRadius("{0ACD74AD27EEEE7D}Prefabs/Groups/OPFOR/RHS_AFRF/MSV/VKPO_Demiseason/Group_RHS_RF_MSV_VKPO_DS_FireGroup.et", iEntity.GetOrigin(), LOCATION_RADIUS);
		
		if (defender)
			defendingForce.Insert(defender);
	}
	
	
	
	
}


class TKY_LocationHandler
{
	ref array<ref Location> locations = {};
	const int HELP = 60;
	
	
	void TKY_LocationHandler()
	{
		GetAllEntitiesWithMapDescriptor();
	}
	
	private int runCounter = 0;
	void Run()
	{
		if (runCounter % HELP == 0)
		{
			Print("LocCheck run");
			foreach (Location loc : locations)
			{
				loc.RunChecks();
			}
		}
		runCounter++;
	}
	
	// Method to retrieve all entities that have the SCR_MapDescriptorComponent
    void GetAllEntitiesWithMapDescriptor()
    {
        World world = GetGame().GetWorld();
        if (!world)
        {
            Print("World context not found.");
        }
        
        //world.GetActiveEntities(allEntities);
		Print("tky: searching");
		GetGame().GetWorld().QueryEntitiesBySphere("0 0 0", 99999999, CheckCityTownAddPopulation, FilterCityTownEntities, EQueryEntitiesFlags.STATIC);
		
		Print("tky: done");
		
		foreach (Location loc : locations)
		{
			Print(loc.iEntity.GetName());
		}
    } 
	
	protected bool CheckCityTownAddPopulation(IEntity entity)
	{
		MapDescriptorComponent mapdesc = MapDescriptorComponent.Cast(entity.FindComponent(MapDescriptorComponent));
		if (mapdesc){
			locations.Insert(new Location(entity));
		}
		return true;
	}
	
	protected bool FilterCityTownEntities(IEntity entity) 
	{		
		MapDescriptorComponent mapdesc = MapDescriptorComponent.Cast(entity.FindComponent(MapDescriptorComponent));
		if (mapdesc){	
			int type = mapdesc.GetBaseType();
			if(type == EMapDescriptorType.MDT_NAME_CITY) return true;
			if(type == EMapDescriptorType.MDT_NAME_VILLAGE) return true;
			if(type == EMapDescriptorType.MDT_NAME_TOWN) return true;
			if(type == EMapDescriptorType.MDT_NAME_GENERIC) return true;
		}
				
		return false;		
	}
}