class TKY_QRFHandler
{
	private int qrForceTimer = 0;               // Tracks QR force attack time
	private int noPlayerTimer = 0;             	// Tracks the no-player timer
	int QR_FORCE_DURATION;  				// 15 minutes in seconds (15 * 60)
	int NO_PLAYER_DURATION; 				// 5 minutes in seconds (5 * 60)
	int WAVE_SPAWN_INTERVAL = 30;        // Interval in seconds for wave spawn (2 minutes)
	int ANNOUNCEMENT_DELAY;
	
	TKY_Location location;
	
	ref array<SCR_AIGroup> qrfForce = {};
	private bool qrfStarted = false;
	bool qrfEnded = false;
	
	
	void TKY_QRFHandler(TKY_Location _location, int _ANNOUNCEMENT_DELAY = 300, int _QR_FORCE_DURATION = 900, int _NO_PLAYER_DURATION = 300)
	{
		this.location = _location;
		this.QR_FORCE_DURATION = _QR_FORCE_DURATION;
		this.NO_PLAYER_DURATION = _NO_PLAYER_DURATION;
		this.ANNOUNCEMENT_DELAY = _ANNOUNCEMENT_DELAY;
		
		StartQRF();
		
		GetGame().GetCallqueue().CallLater(
			this.UpdateTimers,
			1000,  // 1-second intervals
			true   // Repeat every second
		);
	}
	
	void ~TKY_QRFHandler()
	{
		foreach (SCR_AIGroup g : qrfForce)
			delete g;
	}
	
	protected void StartQRF()
	{
		// Announce the QRF raid at the location with a map marker or notification
		AnnounceQRF();
		
		// After the announcement delay, start the QRF
		GetGame().GetCallqueue().CallLater(
			this.Run,
			this.ANNOUNCEMENT_DELAY * 1000,  // Convert to milliseconds
			false
		);
	}
	
	protected void AnnounceQRF()
	{
		Print("QRF will begin at " + location.locationEntity.GetName() + " in " + ANNOUNCEMENT_DELAY / 60 + " minutes.");
		// You could create map markers or trigger notifications to the player
	}
	
	protected void Run()
	{
		qrfStarted = true;
		
		// Start the wave spawning process
		SpawnWave();

		// Continue spawning waves every 2 minutes (120 seconds)
		GetGame().GetCallqueue().CallLater(
			this.SpawnWave,
			WAVE_SPAWN_INTERVAL * 1000,
			true
		);
	}
	
	protected void SpawnWave()
	{
		// Spawning logic here
		Print("Spawning a new wave at location: " + location.locationEntity.GetName());
		ref array<ref string> prefabs = location.faction.infantryPrefabs;
		
		// no prefabs in this faction
		if (prefabs.Count() < 1)
			return;
		
		
        SCR_AIGroup defender = TKY_Spawner.SpawnPrefabWithinRadius(
			prefabs[Math.RandomInt(0, prefabs.Count())],
            location.locationEntity.GetOrigin(), 
            location.LOCATION_RADIUS
        );

        if (defender)
            qrfForce.Insert(defender);
	}
	
	protected void UpdateTimers()
	{
		// TODO: timer extend if neither is in the location?
		if (qrfStarted)
		{
			// Increment the QR force timer
			qrForceTimer++;

			// If the QR force duration is over, stop the waves and handle the QRF end
			if (qrForceTimer >= QR_FORCE_DURATION)
			{
				// players won
				EndQRF(TKY_AntiStasiManagerComponent.playerFaction);
				return;
			}
		}
		
		// Increment the no-player timer if there are no players in the location
		if (!TKY_Util.IsAPlayerInsidaRadius(this.location.locationEntity, this.location.LOCATION_RADIUS))
		{
			noPlayerTimer++;

			// If no players for the no-player duration, change faction to enemy
			if (noPlayerTimer >= NO_PLAYER_DURATION)
			{
				// players lost
				EndQRF(location.faction); // location.faction should still be enemy faction i think
			}
		}
		else
		{
			// Reset the no-player timer if a player is in the location
			noPlayerTimer = 0;
		}
		
		Print(qrForceTimer);
		Print(noPlayerTimer);
	} 
	
	protected void EndQRF(TKY_Faction faction)
	{
		// announce task complete
		Print("QRF has ended at location: " + location.locationEntity.GetName());
		
		location.UpdateFaction(faction);
		// Stop further wave spawns
		GetGame().GetCallqueue().Remove(this.SpawnWave);
		GetGame().GetCallqueue().Remove(this.UpdateTimers);
		
		qrfEnded = true;
	}
}
