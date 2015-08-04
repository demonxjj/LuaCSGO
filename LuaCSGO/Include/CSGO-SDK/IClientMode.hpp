#pragma once

#include "Definitions.hpp"
#include "VMatrix.hpp"

namespace SDK {
	class IPanel;
	class CBaseEntity;

	enum ClearFlags_t {
		VIEW_CLEAR_COLOR = 0x1,
		VIEW_CLEAR_DEPTH = 0x2,
		VIEW_CLEAR_FULL_TARGET = 0x4,
		VIEW_NO_DRAW = 0x8,
		VIEW_CLEAR_OBEY_STENCIL = 0x10, // Draws a quad allowing stencil test to clear through portals
		VIEW_CLEAR_STENCIL = 0x20,
	};


	enum MotionBlurMode_t {
		MOTION_BLUR_DISABLE = 1,
		MOTION_BLUR_GAME = 2,			// Game uses real-time inter-frame data
		MOTION_BLUR_SFM = 3				// Use SFM data passed in CViewSetup structure
	};

	class CViewSetup {
	public:
		CViewSetup() {
			m_flAspectRatio = 0.0f;
			m_flNearBlurDepth = 20.0;
			m_flNearFocusDepth = 100.0;
			m_flFarFocusDepth = 250.0;
			m_flFarBlurDepth = 1000.0;
			m_flNearBlurRadius = 10.0;
			m_flFarBlurRadius = 5.0;
			m_nDoFQuality = 0;
			m_bCustomViewMatrix = false;
		}

		int			x, x_old;
		int			y, y_old;
		int			width, width_old;
		int			height, height_old;
		bool		m_bOrtho;
		float		m_OrthoLeft;
		float		m_OrthoTop;
		float		m_OrthoRight;
		float		m_OrthoBottom;
		bool		m_bCustomViewMatrix; // 0x34
		matrix3x4_t	m_matCustomViewMatrix; // 0x38
		char		pad_0x68[0x48]; // 0x68
		float		fov; //	0xB0
		float		fovViewmodel;
		Vector		origin;
		QAngle		angles;
		float		zNear;
		float		zFar;
		float		zNearViewmodel;
		float		zFarViewmodel;
		float		m_flAspectRatio;
		float		m_flNearBlurDepth;
		float		m_flNearFocusDepth;
		float		m_flFarFocusDepth;
		float		m_flFarBlurDepth;
		float		m_flNearBlurRadius;
		float		m_flFarBlurRadius;
		int			m_nDoFQuality;
		MotionBlurMode_t	m_nMotionBlurMode;
		float		m_flShutterTime;				// In seconds
		Vector		m_vShutterOpenPosition;			// Start of frame or "shutter open"
		QAngle		m_shutterOpenAngles;			//
		Vector		m_vShutterClosePosition;		// End of frame or "shutter close"
		QAngle		m_shutterCloseAngles;			// 
		float		m_flOffCenterTop;
		float		m_flOffCenterBottom;
		float		m_flOffCenterLeft;
		float		m_flOffCenterRight;
		int			m_EdgeBlur; // 0x148

	}; // size = 0x14C = 332


	class IClientMode {
		// Misc.
	public:

		virtual			~IClientMode() {}
		virtual	int		ClientModeCSNormal( void * ) = 0;

		// Called before the HUD is initialized.
		virtual void	InitViewport() = 0;

		// One time init when .dll is first loaded.
		virtual void	Init() = 0;

		// Called when vgui is shutting down.
		virtual void	VGui_Shutdown() = 0;

		// One time call when dll is shutting down
		virtual void	Shutdown() = 0;

		// Called when switching from one IClientMode to another.
		// This can re-layout the view and such.
		// Note that Enable and Disable are called when the DLL initializes and shuts down.
		virtual void	Enable() = 0;
		virtual void	EnableWithRootPanel( unsigned int pRoot ) = 0;

		// Called when it's about to go into another client mode.
		virtual void	Disable() = 0;

		// Called when initializing or when the view changes.
		// This should move the viewport into the correct position.
		virtual void	Layout( bool bForce = false ) = 0;
		// Gets at the viewport, if there is one...
		virtual IPanel *GetViewport() = 0;
		// Gets a panel hierarchically below the viewport by name like so "ASWHudInventoryMode/SuitAbilityPanel/ItemPanel1"...
		virtual IPanel *GetPanelFromViewport( const char *pchNamePath ) = 0;
		// Gets at the viewports vgui panel animation controller, if there is one...
		virtual void*  GetViewportAnimationController() = 0;
		// called every time shared client dll/engine data gets changed,
		// and gives the cdll a chance to modify the data.
		virtual void	ProcessInput( bool bActive ) = 0;
		// The mode can choose to draw/not draw entities.
		virtual bool	ShouldDrawDetailObjects() = 0;
		virtual bool	ShouldDrawEntity( CBaseEntity *pEnt ) = 0;
		virtual bool	ShouldDrawLocalPlayer( CBaseEntity *pPlayer ) = 0;
		virtual bool	ShouldDrawParticles() = 0;
		// The mode can choose to not draw fog
		virtual bool	ShouldDrawFog( void ) = 0;
		virtual void	OverrideView( CViewSetup *pSetup ) = 0; // 18
		virtual void	OverrideAudioState( void* pAudioState ) = 0; // 19
		virtual int		KeyInput( int down, int keynum, const char *pszCurrentBinding ) = 0; // 20
		virtual void	StartMessageMode( int iMessageModeType ) = 0;
		virtual IPanel *GetMessagePanel() = 0;
		virtual void	OverrideMouseInput( float *x, float *y ) = 0;
		virtual bool	CreateMove( float flInputSampleTime, void* usercmd ) = 0; // 24
		virtual void	LevelInit( const char *newmap ) = 0;
		virtual void	LevelShutdown( void ) = 0;
	};

}