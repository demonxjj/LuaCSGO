## LuaCSGO: 
	- A Lua API for CSGO hacking

**This is a proff of concept. I dont advise anyone to use public released hacks such as these.**

HOWTO:
	- The main script must be placed at "steamapps\common\Counter-Strike Global Offensive\LuaCSGO\LuaCSGO.lua". 
	- Additional scripts can be used but they must be included (require("LuaCSGO/CustomScriptName")) by the main script.

Checkout the example script at <link>

TODO:
	- Add error messages when something bad happens. As of right now if you call "class.function" instead of "class:function" the program just crashes without letting you know why.
	
Currently exported stuff in no particular order:

#CLASSES
```
Class:
	Interfaces
Functions:
```
	EngineInterface 		GetEngineInterface()
	EngineTraceInterface 	GetEngineTraceInterface()
	EntityListInterface 	GetEntityListInterface()
	PanelInterface 			GetPanelInterface()
	GlobalVars 				GetGlobalVars()
```

Class:
	DrawManager
Functions:
```
	unsigned long 			CreateNewFont( const char* fontFace, int size, bool bold, bool italic, bool outlined, bool dropShadow )
	void 					DrawLine( int x1, int y1, int x2, int y2 )
	void 					DrawFilledCircle( int x, int y, int r )
	void 					DrawFilledRect( int x, int y, int w, int h )
	void 					FillGradient( int x, int y, int w, int h, unsigned long to, bool vertical )
	void 					DrawOutlinedCircle( int x, int y, int r, unsigned long outlineColor )
	void 					DrawCircle( int x, int y, int r )
	void 					DrawOutlinedRect( int x, int y, int w, int h, unsigned long outlineColor )
	void 					DrawRect( int x, int y, int w, int h )
	void 					DrawText( unsigned long fontId, int x, int y, const char* text )
	Size 					MeasureText( unsigned long fontId, const char* text )
	unsigned long 			GetDrawColor()
	void 					SetDrawColor( unsigned long argb )
```

Class:
	Callbacks
Functions:
```
	bool 					RegisterCallback( CallbackType type, luabind::object f )
	void 					DisableAllExcept( CallbackType type )
	void 					EnableAllCallbacks()
```

Class:
	Utils
Functions:
```
	Vector 					GetEntityBone( BaseClientEntity pEntity, int iBoneNumber )
	Vector 					WorldToScreen( const Vector &origin )
	BaseClientEntity 		GetLocalPlayer()
```

Class:
	Keys
Functions:
```
	const char* 			LookupKeyString( int keycode )
```

Class:
	**Size**
Functions:
```
	bool 					IsValid()
```
Properties:
	int Width
	int Height
Operators:
	Size + Size
	Size - Size
	Size * Size
	Size / Size
	Size * float
	Size / float
	
Class:
	**Vector**
Functions:
```
	bool 					IsValid()
	float 					Dot()
	bool					IsZero()
	void 					Invalidate()
	float 					Length()
	Vector 					Normalized()
	void 					Zero()
```
Properties:
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
	
Class:
	**BaseClientEntity**
Functions:
```
	bool 					IsValid()
	Vector 					GetOrigin()
	int 					GetClassID()
	const char* 			GetClassName()
	int 					GetHealth()
	int 					GetFlags()
	Vector 					GetEyePos()
	bool 					IsDormant()
	bool 					IsAlive()
	int 					GetTeam()
	Vector 					GetAimPunch()
	Vector 					GetViewPunch()
	void 					SetAimPunch( Vector v )
	void 					SetViewPunch( Vector v )
```

Class:
	**EngineInterface**
Functions:
```
	bool 					IsInGame()
	bool 					IsConnected()
	bool 					IsPlayingDemo()
	bool 					IsRecordingDemo()
	bool 					IsTakingScreenshot()
	void 					ExecuteCommand( const char* szCommand )
	int 					GetLocalPlayer()
	int 					GetMaxClients()
	const char* 			GetClientName( int entNum )
	const char* 			GetLevelName()
	Size 					GetScreenSize()
	Vector 					GetViewAngles()
	void 					SetViewAngles( Vector v )
```

Class:
	**GameTrace**
Functions:
```
	bool 					DidHit()
	bool 					IsVisible()
	bool 					DidHitEntity( BaseClientEntity pEnt )
	int 					GetHitbox()
	Vector 					GetEndPos()
```

Class:
	**EngineTraceInterface**
Functions:
```
	GameTrace 				TraceRay( Vector start, Vector end, BaseClientEntity skipEnt, unsigned int mask)
```

Class:
	**EntityListInterface**
Functions:
```
	BaseClientEntity 		GetEntityFromIndex( int index )
	int 					GetHighestEntityIndex()
```

Class:
	**GlobalVars**
Functions:
```
	float 					GetAbsFrameTime()
	float 					GetCurrentTime()
    int 					GetFrameCount()
    float 					GetFrameTime()
    float 					GetInterpolationAmount()
    float 					GetIntervalPerTick
    int 					GetMaxClients()
    bool 					IsRemoteClient()
    float 					GetRealTime()
    float 					GetTickCount()
```

Class:
	**PanelInterface**
Functions:
```
	const char* 			GetName( unsigned int panel )
```

Class:
	**MouseEvent**
Properties:
	X
	Y
	Type
	Button
	
Class:
	**KeyboardEvent**
Properties:
	Type
	KeyCode

Class:
	**UserCMD**
Functions:
```
	void 					GetChecksum()
```
Properties:
	Buttons
	ViewAngles
	ForwardMove
	SideMove
	UpMove
	
Class:	
	**VerifiedCMD**
Functions:
```
	void 					Update(UserCMD userCmd)
```
ReloadScripts()