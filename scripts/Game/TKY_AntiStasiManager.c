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
	ref TKY_LocationHandler locationHandler;
	bool firstRun = true;
	
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
			locationHandler = new TKY_LocationHandler();
			firstRun = false;
		}
		locationHandler.Run();
	}
}