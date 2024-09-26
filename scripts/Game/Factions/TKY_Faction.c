enum TKY_FactionEnum {
	RHS_AFRF = 0,
	RHS_USAF = 1,
	INDFOR = 2
}

class TKY_Faction
{
	TKY_FactionEnum factionKey;
	//ref Faction faction;
	ref Color color;
	ref array<ref string> infantryPrefabs;
	ref array<ref string> vehiclePrefabs;
	bool isAiFaction;
	
	void TKY_Faction(
		TKY_FactionEnum _factionKey,
		//Faction _faction,
		Color _color,
		array<ref string> _infantryPrefabs,
		array<ref string> _vehiclePrefabs,
		bool _isAiFaction
	)
	{
		this.factionKey = _factionKey;
		//this.faction = _faction;
		this.color = _color;
		this.infantryPrefabs = _infantryPrefabs;
		this.vehiclePrefabs = _vehiclePrefabs;
		this.isAiFaction = _isAiFaction;
		
		if (this.isAiFaction)
		{
			GetGame().GetCallqueue().CallLater(
				this.RunAiFactionUpdate,
				1000,
				true
			);
		}
	}
	
	protected void RunAiFactionUpdate()
	{
	
	}
}