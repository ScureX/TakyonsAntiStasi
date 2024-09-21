class TKY_LocationHandler
{
	ref array<ref TKY_Location> locations = {};
	const int UPDATE_LIMITER = 60;
	
	
	void TKY_LocationHandler()
	{
		GetAllEntitiesWithMapDescriptor();
	}
	
	private int runCounter = 0;
	void Run()
	{
		if (runCounter % UPDATE_LIMITER == 0)
		{
			Print("LocCheck run");
			foreach (TKY_Location loc : locations)
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
		
		foreach (TKY_Location loc : locations)
		{
			Print(loc.iEntity.GetName());
		}
    } 
	
	protected bool CheckCityTownAddPopulation(IEntity entity)
	{
		MapDescriptorComponent mapdesc = MapDescriptorComponent.Cast(entity.FindComponent(MapDescriptorComponent));
		if (mapdesc){
			TKY_Location loc = new TKY_Location(entity, TKY_AntiStasiManagerComponent.enemyFaction);
			locations.Insert(loc);
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