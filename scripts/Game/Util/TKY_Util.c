class TKY_Util
{
	static bool IsAPlayerInsidaRadius(IEntity iEntity, int radius)
	{
		private ref array<IEntity> nearbyCharacters = new array<IEntity>();
		private array<int> players = new array<int>(); 
		GetGame().GetPlayerManager().GetPlayers(players);
		
		foreach (int playerid : players)
		{
			IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerid);
			
			if (!player)
				continue;

			if (vector.DistanceSqXZ(player.GetOrigin(), iEntity.GetOrigin()) <= (radius * radius))
				return true;
		}
		
		return false;
	}
	
	
	static vector GetRandomPositionWithinRadius(vector center, float radius)
	{
	    // Generate random offsets within the radius
	    float randomAngle = Math.RandomFloatInclusive(0, Math.PI2);  // Random angle between 0 and 2π
	    float randomDistance = Math.RandomFloatInclusive(0, radius); // Random distance within the radius
	
	    // Calculate the random position
	    float offsetX = Math.Cos(randomAngle) * randomDistance;
	    float offsetZ = Math.Sin(randomAngle) * randomDistance;
	    
	    // Return the new position with the random offset
	    return Vector(center[0] + offsetX, center[1], center[2] + offsetZ);
	}
	
	// Static function to generate a random position within a radius but excluding a minimum radius
	static vector GetRandomPositionWithinRadiusWithExclusion(vector center, float minRadius, float maxRadius)
	{
	    // Ensure minRadius is less than maxRadius
	    if (minRadius >= maxRadius)
	    {
	        Print("Error: minRadius must be smaller than maxRadius.");
	        return center;
	    }
	
	    // Generate a random angle between 0 and 2π (full circle)
	    float randomAngle = Math.RandomFloatInclusive(0, Math.PI2);
	
	    // Generate a random distance between the minRadius and maxRadius
	    float randomDistance = Math.RandomFloatInclusive(minRadius, maxRadius);
	
	    // Calculate the random offset based on the angle and distance
	    float offsetX = Math.Cos(randomAngle) * randomDistance;
	    float offsetZ = Math.Sin(randomAngle) * randomDistance;
	
	    // Create the random position by applying the offset to the center position
	    return Vector(center[0] + offsetX, center[1], center[2] + offsetZ);
	}

}