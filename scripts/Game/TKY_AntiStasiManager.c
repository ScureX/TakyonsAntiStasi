[ComponentEditorProps(category: "AntiStasi/GameScripted/GameMode", description: "")]
class TKY_AntiStasiManagerClass : GenericEntityClass
{
}

class TKY_AntiStasiManager : GenericEntity 
{
}


[ComponentEditorProps(category: "AntiStasi/GameScripted/GameMode", description: "Base for gamemode scripted component.")]
class TKY_AntiStasiManagerComponentClass : ScriptComponentClass
{
}

class TKY_AntiStasiManagerComponent: ScriptComponent
{
	static ref TKY_LocationHandler locationHandler;
	static ref TKY_Faction playerFaction;
	static ref TKY_Faction enemyFaction;
	
	private bool firstRun = true;
	
	protected override void OnPostInit(IEntity owner)
	{
		Print("TKY: OnPostInit");
		SetEventMask(owner, EntityEvent.FRAME);
	}
	
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		super.EOnFrame(owner, timeSlice);
		if (firstRun)
		{
			playerFaction = new TKY_Faction(
				TKY_FactionEnum.INDFOR,
				Color.Green,
				{},
				{},
				false
			);
			enemyFaction = new TKY_Faction(
				TKY_FactionEnum.RHS_AFRF,
				Color.Blue,
				{"{F40CDA3D5A7B1CDF}Prefabs/Groups/OPFOR/RHS_AFRF/MSV/VKPO_Demiseason/Group_RHS_RF_MSV_VKPO_DS_LightFireTeam.et"},
				{"{F40CDA3D5A7B1CDF}Prefabs/Groups/OPFOR/RHS_AFRF/MSV/VKPO_Demiseason/Group_RHS_RF_MSV_VKPO_DS_LightFireTeam.et"},
				true
			);
			
			locationHandler = new TKY_LocationHandler();
			firstRun = false;
			locationHandler.Run();
		}
	}
}