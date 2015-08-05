## LuaCSGO: 
* A Lua API for CSGO hacking
	
####This is a prof of concept. I dont advise anyone to use public released hacks such as these.

HOWTO:
* The main script must be placed at "steamapps\common\Counter-Strike Global Offensive\LuaCSGO\LuaCSGO.lua". 
* Additional scripts can be used but they must be included (require("LuaCSGO/CustomScriptName") by the main script.

Checkout the example script at https://github.com/MarkHC/LuaCSGO/tree/master/Example


TODO:
* Add error messages when something bad happens. As of right now if you call "class.function" instead of "function" the program just crashes without letting you know why.
	
	
Currently exported stuff in no particular order:

##CLASSES


**Interfaces**
```C++
	EngineInterface 		GetEngineInterface();
	EngineTraceInterface 	GetEngineTraceInterface();
	EntityListInterface 	GetEntityListInterface();
	PanelInterface 			GetPanelInterface();
	GlobalVars 				GetGlobalVars();
```

**DrawManager**
```C++
	unsigned long 			CreateNewFont( const char* fontFace, int size, bool bold, bool italic, bool outlined, bool dropShadow );
	void 					DrawLine( int x1, int y1, int x2, int y2 );
	void 					DrawFilledCircle( int x, int y, int r );
	void 					DrawFilledRect( int x, int y, int w, int h );
	void 					FillGradient( int x, int y, int w, int h, unsigned long to, bool vertical );
	void 					DrawOutlinedCircle( int x, int y, int r, unsigned long outlineColor );
	void 					DrawCircle( int x, int y, int r );
	void 					DrawOutlinedRect( int x, int y, int w, int h, unsigned long outlineColor );
	void 					DrawRect( int x, int y, int w, int h );;
	void 					DrawText( unsigned long fontId, int x, int y, const char* text );
	Size 					MeasureText( unsigned long fontId, const char* text );
	unsigned long 			GetDrawColor();
	void 					SetDrawColor( unsigned long argb );
```

**Callbacks**
```C++
	bool 					RegisterCallback( CallbackType type, luabind::object f );
	void 					DisableAllExcept( CallbackType type );
	void 					EnableAllCallbacks();
```

**Utils**
```C++
	Vector 					GetEntityBone( BaseClientEntity pEntity, int iBoneNumber );
	Vector 					WorldToScreen( const Vector &origin );
	BaseClientEntity 		GetLocalPlayer();
```

**Keys**
```C++
	const char* 			LookupKeyString( int keycode );
```

**Size** 
```C++
	bool 					IsValid();
 
	int Width
	int Height 
	
Operators: 
	Size + Size
	Size - Size
	Size * Size
	Size / Size
	Size * float
	Size / float
```

**Vector** 
```C++
	bool 					IsValid();
	float 					Dot();
	bool					IsZero();
	void 					Invalidate();
	float 					Length();
	Vector 					Normalized();
	void 					Zero(); 
	
	float X
	float Y
	float Z
	
Operators:
	Vector + Vector
	Vector - Vector
	Vector * Vector
	Vector / Vector
	Vector * float
	Vector / float
```

**BaseClientEntity** 
```C++
	bool 					IsValid();
	Vector 					GetOrigin();
	int 					GetClassID();
	const char* 			GetClassName();
	int 					GetHealth();
	int 					GetFlags();
	Vector 					GetEyePos();
	bool 					IsDormant();
	bool 					IsAlive();
	int 					GetTeam();
	Vector 					GetAimPunch();
	Vector 					GetViewPunch();
	void 					SetAimPunch( Vector v );
	void 					SetViewPunch( Vector v );
```

**EngineInterface** 
```C++
	bool 					IsInGame();
	bool 					IsConnected();
	bool 					IsPlayingDemo();
	bool 					IsRecordingDemo();
	bool 					IsTakingScreenshot();
	void 					ExecuteCommand( const char* szCommand );
	int 					GetLocalPlayer();
	int 					GetMaxClients();
	const char* 			GetClientName( int entNum );
	const char* 			GetLevelName();
	Size 					GetScreenSize();
	Vector 					GetViewAngles();
	void 					SetViewAngles( Vector v );
```

**GameTrace** 
```C++
	bool 					DidHit();
	bool 					IsVisible();
	bool 					DidHitEntity( BaseClientEntity pEnt );
	int 					GetHitbox();
	Vector 					GetEndPos();
```

**EngineTraceInterface** 
```C++
	GameTrace 				TraceRay( Vector start, Vector end, BaseClientEntity skipEnt, unsigned int mask);
```

**EntityListInterface** 
```C++
	BaseClientEntity 		GetEntityFromIndex( int index );
	int 					GetHighestEntityIndex();
```

**GlobalVars** 
```C++
	float 					GetAbsFrameTime();
	float 					GetCurrentTime();
    int 					GetFrameCount();
    float 					GetFrameTime();
    float 					GetInterpolationAmount();
    float 					GetIntervalPerTick
    int 					GetMaxClients();
    bool 					IsRemoteClient();
    float 					GetRealTime();
    float 					GetTickCount();
```

**PanelInterface**
```C++
	const char* 			GetName( unsigned int panel );
```

**MouseEvent**
```
	X
	Y
	Type
	Button
```

**KeyboardEvent**
```
	Type
	KeyCode
```


**UserCMD**
```C++
	void 					GetChecksum(); 
	
	Buttons
	ViewAngles
	ForwardMove
	SideMove
	UpMove
```

**VerifiedCMD**
```C++
	void 					Update(UserCMD userCmd);
```

#CONSTANTS

**Callbacks**
```
CALLBACK_SCRIPTLOAD
CALLBACK_SCRIPTUNLOAD
CALLBACK_MOUSE
CALLBACK_KEYBOARD
CALLBACK_PAINTTRAVERSE
CALLBACK_CREATEMOVE
CALLBACK_FRAMESTAGENOTIFY_BEGIN
CALLBACK_FRAMESTAGENOTIFY_END
```

**FrameStageNotify stages**
```
FRAME_UNDEFINED
FRAME_START
FRAME_NET_UPDATE_START
FRAME_NET_UPDATE_POSTDATAUPDATE_START
FRAME_NET_UPDATE_POSTDATAUPDATE_END
FRAME_NET_UPDATE_END
FRAME_RENDER_START
FRAME_RENDER_END
```

**Events**
```
EVENT_UNKNOWN
EVENT_KEYDOWN
EVENT_KEYUP
EVENT_MOUSEDOWN
EVENT_MOUSEUP
EVENT_MOUSEMOVE
```

**Masks for TraceRay**
```
MASK_ALL
MASK_SOLID
MASK_SHOT
MASK_VISIBLE
MASK_SHOT_PORTAL
MASK_SHOT_BRUSHONLY
```

**Keys**
```
KEY_NONE		
BUTTON_LEFT		
BUTTON_RIGHT	
BUTTON_MIDDLE	
BUTTON_MOUSE4	
BUTTON_MOUSE5	
KEY_BACK		
KEY_TAB			
KEY_CLEAR		
KEY_RETURN		
KEY_SHIFT		
KEY_CONTROL		
KEY_ALT			
KEY_PAUSE		
KEY_CAPITAL		
KEY_ESCAPE		
KEY_SPACE		
KEY_PRIOR		
KEY_NEXT		
KEY_END			
KEY_HOME		
KEY_LEFT		
KEY_UP			
KEY_RIGHT		
KEY_DOWN		
KEY_PRINT		
KEY_INSERT		
KEY_DELETE		
KEY_0			
KEY_1			
KEY_2			
KEY_3			
KEY_4			
KEY_5			
KEY_6			
KEY_7			
KEY_8			
KEY_9			
KEY_A			
KEY_B			
KEY_C			
KEY_D			
KEY_E			
KEY_F			
KEY_G			
KEY_H			
KEY_I			
KEY_J			
KEY_K			
KEY_L			
KEY_M			
KEY_N			
KEY_O			
KEY_P			
KEY_Q			
KEY_R			
KEY_S			
KEY_T			
KEY_U			
KEY_V			
KEY_W			
KEY_X			
KEY_Y			
KEY_Z			
KEY_LWIN		
KEY_RWIN		
KEY_APPS		
KEY_SLEEP		
KEY_NUMPAD0		
KEY_NUMPAD1		
KEY_NUMPAD2		
KEY_NUMPAD3		
KEY_NUMPAD4		
KEY_NUMPAD5		
KEY_NUMPAD6		
KEY_NUMPAD7		
KEY_NUMPAD8		
KEY_NUMPAD9		
KEY_MULTIPLY	
KEY_ADD			
KEY_SEPARATOR	
KEY_SUBTRACT	
KEY_DECIMAL		
KEY_DIVIDE		
KEY_F1			
KEY_F2			
KEY_F3			
KEY_F4			
KEY_F5			
KEY_F6			
KEY_F7			
KEY_F8			
KEY_F9			
KEY_F10			
KEY_F11			
KEY_F12			
KEY_LMENU		
KEY_RMENU		
KEY_OEM_1		
KEY_OEM_PLUS	
KEY_OEM_COMMA	
KEY_OEM_MINUS	
KEY_OEM_PERIOD	
KEY_OEM_2		
KEY_OEM_3		
KEY_OEM_4		
KEY_OEM_5		
KEY_OEM_6		
KEY_OEM_7		
KEY_OEM_8
```