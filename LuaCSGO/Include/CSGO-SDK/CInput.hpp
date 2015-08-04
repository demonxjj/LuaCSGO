#pragma once

#include "QAngle.hpp"
#include "CUserCMD.hpp"

namespace SDK {
	class bf_write;

	class CVerifiedUserCmd {
	public:
		CUserCmd	m_cmd;
		CRC32_t		m_crc;
	};

	class CKeyboardKey {
	public:
		// Name for key
		char				name[32];
		// Pointer to the underlying structure
		void			*pkey;
		// Next key in key list.
		CKeyboardKey		*next;
	};

	class CInput {
	public:
		CInput( void );
		virtual		void		Init_All( void );
		virtual		void		Shutdown_All( void );
		virtual		int			GetButtonBits( bool bResetState );
		virtual		void		CreateMove( int sequence_number, float input_sample_frametime, bool active );
		virtual		void		ExtraMouseSample( float frametime, bool active );
		virtual		bool		WriteUsercmdDeltaToBuffer( int nSlot, bf_write *buf, int from, int to, bool isnewcommand );
		virtual		void		EncodeUserCmdToBuffer( int nSlot, bf_write& buf, int slot );
		virtual		void		DecodeUserCmdFromBuffer( int nSlot, bf_read& buf, int slot );
		virtual		CUserCmd*	GetUserCmd( int nSlot, int sequence_number );

		typedef struct {
			unsigned int AxisFlags;
			unsigned int AxisMap;
			unsigned int ControlMap;
		} joy_axis_t;

		enum {
			GAME_AXIS_NONE = 0,
			GAME_AXIS_FORWARD,
			GAME_AXIS_PITCH,
			GAME_AXIS_SIDE,
			GAME_AXIS_YAW,
			MAX_GAME_AXES
		};

		enum {
			CAM_COMMAND_NONE = 0,
			CAM_COMMAND_TOTHIRDPERSON = 1,
			CAM_COMMAND_TOFIRSTPERSON = 2
		};

		enum {
			MOUSE_ACCEL_THRESHHOLD1 = 0,	// if mouse moves > this many mickey's double it
			MOUSE_ACCEL_THRESHHOLD2,		// if mouse moves > this many mickey's double it a second time
			MOUSE_SPEED_FACTOR,				// 1 - 20 (default 10) scale factor to accelerated mouse setting
			NUM_MOUSE_PARAMS,
		};

		bool m_fTrackIRAvailable;			// 0x04
		bool m_fMouseInitialized;			// 0x05
		bool m_fMouseActive;				// 0x06
		bool m_fJoystickAdvancedInit;		// 0x07
		char pad_0x08[0x2C];				// 0x08
		CKeyboardKey *m_pKeys;				// 0x34
		char pad_0x38[0x64];				// 0x38
		bool m_fCameraInterceptingMouse;	// 0x9C
		bool m_fCameraInThirdPerson;		// 0x9D
		bool m_fCameraMovingWithMouse;		// 0x9E
		Vector m_vecCameraOffset;			// 0xA0
		bool m_fCameraDistanceMove;			// 0xAC
		int m_nCameraOldX;					// 0xB0
		int m_nCameraOldY;					// 0xB4
		int m_nCameraX;						// 0xB8
		int m_nCameraY;						// 0xBC
		bool m_CameraIsOrthographic;		// 0xC0
		QAngle m_angPreviousViewAngles;		// 0xC4
		QAngle m_angPreviousViewAnglesTilt;	// 0xD0
		float m_flLastForwardMove;			// 0xDC
		int m_nClearInputState;				// 0xE0
		char pad_0xE4[0x8];					// 0xE4
		CUserCmd* m_pCommands;				// 0xEC
		CVerifiedUserCmd* m_pVerifiedCommands; // 0xF0
	};
}