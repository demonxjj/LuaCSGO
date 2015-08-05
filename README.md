## LuaCSGO: 
	- A Lua API for CSGO hacking

**This is a prof of concept. I dont advise anyone to use public released hacks such as these.**

HOWTO:
	- The main script must be placed at "steamapps\common\Counter-Strike Global Offensive\LuaCSGO\LuaCSGO.lua". 
	- Additional scripts can be used but they must be included (require("LuaCSGO/CustomScriptName")) by the main script.

Checkout the example script at https://github.com/MarkHC/LuaCSGO/tree/master/Example

TODO:
	- Add error messages when something bad happens. As of right now if you call "class.function" instead of "function" the program just crashes without letting you know why.
	
Currently exported stuff in no particular order:

##CLASSES


**Interfaces**
```
	EngineInterface 		GetEngineInterface()
	EngineTraceInterface 	GetEngineTraceInterface()
	EntityListInterface 	GetEntityListInterface()
	PanelInterface 			GetPanelInterface()
	GlobalVars 				GetGlobalVars()
```

**DrawManager**
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

**Callbacks**
```
	bool 					RegisterCallback( CallbackType type, luabind::object f )
	void 					DisableAllExcept( CallbackType type )
	void 					EnableAllCallbacks()
```

**Utils**
	
Functions:
```
	Vector 					GetEntityBone( BaseClientEntity pEntity, int iBoneNumber )
	Vector 					WorldToScreen( const Vector &origin )
	BaseClientEntity 		GetLocalPlayer()
```

**Keys**
	
Functions:
```
	const char* 			LookupKeyString( int keycode )
```

**Size** 
```
	bool 					IsValid()
 
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
```
	bool 					IsValid()
	float 					Dot()
	bool					IsZero()
	void 					Invalidate()
	float 					Length()
	Vector 					Normalized()
	void 					Zero() 
	
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

**EngineInterface** 
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

**GameTrace** 
```
	bool 					DidHit()
	bool 					IsVisible()
	bool 					DidHitEntity( BaseClientEntity pEnt )
	int 					GetHitbox()
	Vector 					GetEndPos()
```

**EngineTraceInterface** 
```
	GameTrace 				TraceRay( Vector start, Vector end, BaseClientEntity skipEnt, unsigned int mask)
```

**EntityListInterface** 
```
	BaseClientEntity 		GetEntityFromIndex( int index )
	int 					GetHighestEntityIndex()
```

**GlobalVars** 
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

**PanelInterface**
```
	const char* 			GetName( unsigned int panel )
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
```
	void 					GetChecksum() 
	
	Buttons
	ViewAngles
	ForwardMove
	SideMove
	UpMove
```

**VerifiedCMD**
```
	void 					Update(UserCMD userCmd)
```