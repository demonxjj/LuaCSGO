#pragma once

#include "Definitions.hpp"

#include "CGlobalVarsBase.hpp"
#include "ClientClass.hpp"

namespace SDK {

	//-----------------------------------------------------------------------------
	// Purpose: The engine reports to the client DLL what stage it's entering so the DLL can latch events
	//  and make sure that certain operations only happen during the right stages.
	// The value for each stage goes up as you move through the frame so you can check ranges of values
	//  and if new stages get added in-between, the range is still valid.
	//-----------------------------------------------------------------------------
	enum ClientFrameStage_t {
		FRAME_UNDEFINED = -1,			// (haven't run any frames yet)
		FRAME_START,

		// A network packet is being recieved
		FRAME_NET_UPDATE_START,
		// Data has been received and we're going to start calling PostDataUpdate
		FRAME_NET_UPDATE_POSTDATAUPDATE_START,
		// Data has been received and we've called PostDataUpdate on all data recipients
		FRAME_NET_UPDATE_POSTDATAUPDATE_END,
		// We've received all packets, we can now do interpolation, prediction, etc..
		FRAME_NET_UPDATE_END,

		// We're about to start rendering the scene
		FRAME_RENDER_START,
		// We've finished rendering the scene.
		FRAME_RENDER_END
	};

	class IBaseClientDLL {
	public:
		// Connect appsystem components, get global interfaces, don't run any other init code
		virtual int				Connect( CreateInterfaceFn appSystemFactory, CGlobalVarsBase *pGlobals ) = 0;
		virtual int				Disconnect( void ) = 0;
		// run other init code here
		virtual int				Init( CreateInterfaceFn appSystemFactory, CGlobalVarsBase *pGlobals ) = 0;
		virtual void			PostInit() = 0;
		// Called once when the client DLL is being unloaded
		virtual void			Shutdown( void ) = 0;
		// Called at the start of each level change
		virtual void			LevelInitPreEntity( char const* pMapName ) = 0;
		// Called at the start of a new level, after the entities have been received and created
		virtual void			LevelInitPostEntity() = 0;
		// Called at the end of a level
		virtual void			LevelShutdown( void ) = 0;
		// Request a pointer to the list of client datatable classes
		virtual ClientClass		*GetAllClasses( void ) = 0;
		// Called once per level to re-initialize any hud element drawing stuff
		virtual int				HudVidInit( void ) = 0;
		// Called by the engine when gathering user input
		virtual void			HudProcessInput( bool bActive ) = 0;
		// Called oncer per frame to allow the hud elements to think
		virtual void			HudUpdate( bool bActive ) = 0;
		// Reset the hud elements to their initial states
		virtual void			HudReset( void ) = 0;
		// Display a hud text message
		virtual void			HudText( const char * message ) = 0;
		virtual void			ShouldDrawDropdownConsole( void ) = 0;
		// Mouse Input Interfaces
		// Activate the mouse (hides the cursor and locks it to the center of the screen)
		virtual void			IN_ActivateMouse( void ) = 0;
		// Deactivates the mouse (shows the cursor and unlocks it)
		virtual void			IN_DeactivateMouse( void ) = 0;
		// This is only called during extra sound updates and just accumulates mouse x, y offets and recenters the mouse.
		//  This call is used to try to prevent the mouse from appearing out of the side of a windowed version of the engine if 
		//  rendering or other processing is taking too long
		virtual void			IN_Accumulate( void ) = 0;
		// Reset all key and mouse states to their initial, unpressed state
		virtual void			IN_ClearStates( void ) = 0;
		// If key is found by name, returns whether it's being held down in isdown, otherwise function returns false
		virtual bool			IN_IsKeyDown( const char *name, bool& isdown ) = 0;
		// Raw keyboard signal, if the client .dll returns 1, the engine processes the key as usual, otherwise,
		//  if the client .dll returns 0, the key is swallowed.
		virtual int				IN_KeyEvent( int eventcode, int /*ButtonCode_t*/ keynum, const char *pszCurrentBinding ) = 0;
		// This function is called once per tick to create the player CUserCmd (used for prediction/physics simulation of the player)
		// Because the mouse can be sampled at greater than the tick interval, there is a separate input_sample_frametime, which
		//  specifies how much additional mouse / keyboard simulation to perform.
		virtual void			CreateMove(
			int sequence_number,			// sequence_number of this cmd
			float input_sample_frametime,	// Frametime for mouse input sampling
			bool active ) = 0;				// True if the player is active (not paused)
	};
}