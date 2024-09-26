class TKY_SpawnConstructionCommand : ScrServerCommand
{
	// Specify keyword of command
	//-----------------------------------------------------------------------------
	override string GetKeyword()
	{
		return "sethq";
	}
	
	// Run command server-side
	//-----------------------------------------------------------------------------
	override bool IsServerSide()
	{
		return true;
	}
	
	// Set requirement to admin permission via RCON
	//-----------------------------------------------------------------------------
	override int RequiredRCONPermission()
	{
		return ERCONPermissions.PERMISSIONS_ADMIN;
	}
	
	// Set requirement to be logged in administrator for chat command
	//-----------------------------------------------------------------------------
	override int RequiredChatPermission()
	{
		return EPlayerRole.ADMINISTRATOR;
	}
	
	// Shared handle for kicking player for RCON and Chat command
	//-----------------------------------------------------------------------------
	protected ScrServerCmdResult SpawnPrefab(array<string> argv, int playerId = 0)
	{
		// get player
		IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId);
			
		if (!player)
			return ScrServerCmdResult(string.Format("HQ Construction!"), EServerCmdResultType.OK);
		
		// get world
		World world = GetGame().GetWorld();
	    if (!world)
	    {
	        Print("World context not found.");
	        return ScrServerCmdResult(string.Format("HQ Construction!"), EServerCmdResultType.OK);
	    }
		
		// prefab
		const string prefab = "{4E1DFB032E3F2996}Prefabs/Props/AntiStasi/AntiStasi_BuildingService.et";
		
		
		// player location
		vector location = player.GetOrigin();
		
		// player vector but like reversed
		/*vector transform;
		player.GetTransform(transform);
		Print(transform);
		
		vector localTransform;
		player.GetLocalTransform(localTransform);
		Print(localTransform);*/
		
		// spawn prefab
		
		// Create the prefab spawn parameters (can be customized as needed)
	    EntitySpawnParams params = new EntitySpawnParams();
	    params.TransformMode = ETransformMode.WORLD;
	    params.Transform[3] = location; // Set the position to spawn the prefab
	    
		Resource res = Resource.Load(prefab);
		GetGame().SpawnEntityPrefab(res, world, params);
		
	    // Spawn the prefab at the random location
	    GenericEntity newEntity = GenericEntity.Cast(GetGame().SpawnEntityPrefab(res, world, params));
	    
	    // Check if the prefab was spawned successfully
	    if (!newEntity)
	    {
	        Print("Failed to spawn prefab!");
		return ScrServerCmdResult(string.Format("HQ Construction!"), EServerCmdResultType.OK);
	    }
	    
		FactionAffiliationComponent comp = FactionAffiliationComponent.Cast(player.FindComponent(FactionAffiliationComponent));
		
		SCR_FactionAffiliationComponent.SetFaction(newEntity, comp.GetAffiliatedFaction());
		
		
		return ScrServerCmdResult(string.Format("HQ Construction!"), EServerCmdResultType.OK);
	}
	
	// Handle Chat command on server
	//-----------------------------------------------------------------------------
	override ref ScrServerCmdResult OnChatServerExecution(array<string> argv, int playerId)
	{
		return SpawnPrefab(argv, playerId);
	}
	
	// Handle Chat command on client
	//-----------------------------------------------------------------------------
	override ref ScrServerCmdResult OnChatClientExecution(array<string> argv, int playerId)
	{
		return ScrServerCmdResult(string.Empty, EServerCmdResultType.OK);
	}
	
	// Handle RCON command on server
	//-----------------------------------------------------------------------------
	override ref ScrServerCmdResult OnRCONExecution(array<string> argv)
	{
		return ScrServerCmdResult(string.Empty, EServerCmdResultType.OK);
	}
	
	// Handle Pending command
	//-----------------------------------------------------------------------------
	override ref ScrServerCmdResult OnUpdate()
	{
		return ScrServerCmdResult(string.Empty, EServerCmdResultType.OK);
	}

}

class TKY_CommanderRelocate : ScriptedUserAction
{
	
}