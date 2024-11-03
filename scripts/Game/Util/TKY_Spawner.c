/*class TKY_AntiStasiFaction
{
	static Color GetColorForFaction(TKY_AntiStasiFactionEnum faction)
	{
		if (faction == TKY_AntiStasiFactionEnum.RHS_AFRF)
		{
			return Color.Red;
		}
		else if (faction == TKY_AntiStasiFactionEnum.RHS_USAF)
		{
			return Color.Blue;
		}
		else if (faction == TKY_AntiStasiFactionEnum.INDFOR)
		{
			return Color.Green;
		}
		return Color.White;
	}
		
	static array<string> GetPrefabsForFaction(TKY_AntiStasiFactionEnum faction)
	{
		if (faction == TKY_AntiStasiFactionEnum.RHS_AFRF)
		{
			return {
	            "{0ACD74AD27EEEE7D}Prefabs/Groups/OPFOR/RHS_AFRF/MSV/VKPO_Demiseason/Group_RHS_RF_MSV_VKPO_DS_FireGroup.et",
	            "{DFF74E2181355AFD}Prefabs/Groups/OPFOR/RHS_AFRF/MSV/VKPO_Demiseason/Group_RHS_RF_MSV_VKPO_DS_Team_GL.et",
	            "{F40CDA3D5A7B1CDF}Prefabs/Groups/OPFOR/RHS_AFRF/MSV/VKPO_Demiseason/Group_RHS_RF_MSV_VKPO_DS_LightFireTeam.et",
	            "{60E2D587BE5A9B43}Prefabs/Groups/OPFOR/RHS_AFRF/MSV/VKPO_Demiseason/Group_RHS_RF_MSV_VKPO_DS_MachineGunTeam.et"
	        };
		}
		else if (faction == TKY_AntiStasiFactionEnum.RHS_USAF)
		{
			return {
	            "{19843E954790DF28}Prefabs/Groups/BLUFOR/RHS_USAF/RHS_USAF_USMC_MEF/Group_USAF_USMC_MEF_FireTeam.et",
	            "{5B42785917B46C1C}Prefabs/Groups/BLUFOR/RHS_USAF/RHS_USAF_USMC_MEF/Group_USAF_USMC_MEF_Team_GL.et",
	            "{C848B065E20A890E}Prefabs/Groups/BLUFOR/RHS_USAF/RHS_USAF_USMC_MEF/Group_USAF_USMC_MEF_LightFireTeam.et",
	            "{559AAEF06C2AF113}Prefabs/Groups/BLUFOR/RHS_USAF/RHS_USAF_USMC_MEF/Group_USAF_USMC_MEF_MachineGunTeam.et"
	        };
		}
		else if (faction == TKY_AntiStasiFactionEnum.INDFOR)
		{
			return {
	            "{5BEA04939D148B1D}Prefabs/Groups/INDFOR/Group_FIA_FireTeam.et",
	            "{2CC26054775FBA2C}Prefabs/Groups/INDFOR/Group_FIA_Team_AT.et",
	            "{1BB20A4B3A53D0F5}Prefabs/Groups/INDFOR/Group_FIA_LightFireTeam.et",
	            "{22F33D3EC8F281AB}Prefabs/Groups/INDFOR/Group_FIA_MachineGunTeam.et"
	        };
		}
		return {};
	}
}*/

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
	
	
	static SCR_AIGroup SpawnPrefabAtLocation(string prefab, vector location)
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
	
	static SCR_AIGroup SpawnPrefabWithinRadius(string prefab, vector origin, int radius, int minDistance = 0)
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
	    SCR_AIGroup newEntity = SCR_AIGroup.Cast(TKY_Spawner.CreatePrefab(prefab, world, params));
	    
	    // Check if the prefab was spawned successfully
	    if (!newEntity)
	    {
	        Print("Failed to spawn prefab!");
			return null;
	    }
	    
	    return newEntity;
	}
	
	static IEntity CreatePrefab(ResourceName prefab, BaseWorld myWorld, EntitySpawnParams params)
	{
		Resource res = Resource.Load(prefab);
		return GetGame().SpawnEntityPrefab(res, myWorld, params);
	}
	
	static EntitySpawnParams GetEntitySpawnParams(IEntity entity)
	{
		EntitySpawnParams params();
		vector mat[4];
		entity.GetWorldTransform(mat);
		params.Transform = mat;
		return params;
	}
}