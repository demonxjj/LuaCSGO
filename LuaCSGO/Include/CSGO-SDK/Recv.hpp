#pragma once

#include <stdio.h>

#include "Definitions.hpp"


namespace SDK {

	

	typedef enum {
		DPT_Int = 0,
		DPT_Float,
		DPT_Vector,
		DPT_VectorXY, // Only encodes the XY of a vector, ignores Z
		DPT_String,
		DPT_Array,	// An array of the base types (can't be of datatables).
		DPT_DataTable,
#if 0 // We can't ship this since it changes the size of DTVariant to be 20 bytes instead of 16 and that breaks MODs!!!
		DPT_Quaternion,
#endif
		DPT_Int64,
		DPT_NUMSendPropTypes
	} SendPropType;

	class DVariant {
	public:
		DVariant() { m_Type = DPT_Float; }
		DVariant( float val ) { m_Type = DPT_Float; m_Float = val; }

		int Q_snprintf( char *pDest, int maxLen, char const *pFormat, ... ) {
			va_list marker;

			va_start( marker, pFormat );
			int len = _vsnprintf( pDest, maxLen, pFormat, marker );
			va_end( marker );

			// Len < 0 represents an overflow
			if( len < 0 ) {
				len = maxLen;
				pDest[maxLen - 1] = 0;
			}

			return len;
		}

		const char *ToString() {
			static char text[128];

			switch( m_Type ) {
				case DPT_Int:
					Q_snprintf( text, sizeof( text ), "%i", (int)m_Int );
					break;
				case DPT_Float:
					Q_snprintf( text, sizeof( text ), "%.3f", m_Float );
					break;
				case DPT_Vector:
					Q_snprintf( text, sizeof( text ), "(%.3f,%.3f,%.3f)",
						m_Vector[0], m_Vector[1], m_Vector[2] );
					break;
				case DPT_VectorXY:
					Q_snprintf( text, sizeof( text ), "(%.3f,%.3f)",
						m_Vector[0], m_Vector[1] );
					break;
#if 0 // We can't ship this since it changes the size of DTVariant to be 20 bytes instead of 16 and that breaks MODs!!!
				case DPT_Quaternion:
					Q_snprintf( text, sizeof( text ), "(%.3f,%.3f,%.3f %.3f)",
						m_Vector[0], m_Vector[1], m_Vector[2], m_Vector[3] );
					break;
#endif
				case DPT_String:
					if( m_pString )
						return m_pString;
					else
						return "NULL";
					break;
				case DPT_Array:
					Q_snprintf( text, sizeof( text ), "Array" );
					break;
				case DPT_DataTable:
					Q_snprintf( text, sizeof( text ), "DataTable" );
					break;
				case DPT_Int64:
					Q_snprintf( text, sizeof( text ), "%lld", m_Int64 );
					break;
				default:
					Q_snprintf( text, sizeof( text ), "DVariant type %i unknown", m_Type );
					break;
			}

			return text;
		}

		union {
			float	m_Float;
			long	m_Int;
			char	*m_pString;
			void	*m_pData;	// For DataTables.
#if 0 // We can't ship this since it changes the size of DTVariant to be 20 bytes instead of 16 and that breaks MODs!!!
			float	m_Vector[4];
#else
			float	m_Vector[3];
#endif
			LONGLONG m_Int64;
		};
		SendPropType	m_Type;
	};

#define ADDRESSPROXY_NONE	-1


	class RecvTable;
	class RecvProp;


	// This is passed into RecvProxy functions.
	class CRecvProxyData {
	public:
		const RecvProp	*m_pRecvProp;		// The property it's receiving.

		DVariant		m_Value;			// The value given to you to store.

		int				m_iElement;			// Which array element you're getting.

		int				m_ObjectID;			// The object being referred to.
	};


	//-----------------------------------------------------------------------------
	// pStruct = the base structure of the datatable this variable is in (like C_BaseEntity)
	// pOut    = the variable that this this proxy represents (like C_BaseEntity::m_SomeValue).
	//
	// Convert the network-standard-type value in m_Value into your own format in pStruct/pOut.
	//-----------------------------------------------------------------------------
	typedef void( *RecvVarProxyFn )(const CRecvProxyData *pData, void *pStruct, void *pOut);

	// ------------------------------------------------------------------------ //
	// ArrayLengthRecvProxies are optionally used to get the length of the 
	// incoming array when it changes.
	// ------------------------------------------------------------------------ //
	typedef void( *ArrayLengthRecvProxyFn )(void *pStruct, int objectID, int currentArrayLength);


	// NOTE: DataTable receive proxies work differently than the other proxies.
	// pData points at the object + the recv table's offset.
	// pOut should be set to the location of the object to unpack the data table into.
	// If the parent object just contains the child object, the default proxy just does *pOut = pData.
	// If the parent object points at the child object, you need to dereference the pointer here.
	// NOTE: don't ever return null from a DataTable receive proxy function. Bad things will happen.
	typedef void( *DataTableRecvVarProxyFn )(const RecvProp *pProp, void **pOut, void *pData, int objectID);


	// This is used to fork over the standard proxy functions to the engine so it can
	// make some optimizations.
	class CStandardRecvProxies {
	public:
		CStandardRecvProxies();

		RecvVarProxyFn m_Int32ToInt8;
		RecvVarProxyFn m_Int32ToInt16;
		RecvVarProxyFn m_Int32ToInt32;
		RecvVarProxyFn m_Int64ToInt64;
		RecvVarProxyFn m_FloatToFloat;
		RecvVarProxyFn m_VectorToVector;
	};
	extern CStandardRecvProxies g_StandardRecvProxies;


	class CRecvDecoder;


	class RecvProp {
		// This info comes from the receive data table.
	public:
		RecvProp();

		void					InitArray( int nElements, int elementStride );

		int						GetNumElements() const;
		void					SetNumElements( int nElements );

		int						GetElementStride() const;
		void					SetElementStride( int stride );

		int						GetFlags() const;

		const char*				GetName() const;
		SendPropType			GetType() const;

		RecvTable*				GetDataTable() const;
		void					SetDataTable( RecvTable *pTable );

		RecvVarProxyFn			GetProxyFn() const;
		void					SetProxyFn( RecvVarProxyFn fn );

		DataTableRecvVarProxyFn	GetDataTableProxyFn() const;
		void					SetDataTableProxyFn( DataTableRecvVarProxyFn fn );

		int						GetOffset() const;
		void					SetOffset( int o );

		// Arrays only.
		RecvProp*				GetArrayProp() const;
		void					SetArrayProp( RecvProp *pProp );

		// Arrays only.
		void					SetArrayLengthProxy( ArrayLengthRecvProxyFn proxy );
		ArrayLengthRecvProxyFn	GetArrayLengthProxy() const;

		bool					IsInsideArray() const;
		void					SetInsideArray();

		// Some property types bind more data to the prop in here.
		const void*			GetExtraData() const;
		void				SetExtraData( const void *pData );

		// If it's one of the numbered "000", "001", etc properties in an array, then
		// these can be used to get its array property name for debugging.
		const char*			GetParentArrayPropName();
		void				SetParentArrayPropName( const char *pArrayPropName );

	public:

		char					*m_pVarName;
		SendPropType			m_RecvType;
		int						m_Flags;
		int						m_StringBufferSize;


	private:

		bool					m_bInsideArray;		// Set to true by the engine if this property sits inside an array.

		// Extra data that certain special property types bind to the property here.
		const void *m_pExtraData;

		// If this is an array (DPT_Array).
		RecvProp				*m_pArrayProp;
		ArrayLengthRecvProxyFn	m_ArrayLengthProxy;

		RecvVarProxyFn			m_ProxyFn;
		DataTableRecvVarProxyFn	m_DataTableProxyFn;	// For RDT_DataTable.

		RecvTable				*m_pDataTable;		// For RDT_DataTable.
		int						m_Offset;

		int						m_ElementStride;
		int						m_nElements;

		// If it's one of the numbered "000", "001", etc properties in an array, then
		// these can be used to get its array property name for debugging.
		const char				*m_pParentArrayPropName;
	};


	class RecvTable {
	public:

		typedef RecvProp	PropType;

		RecvTable();
		RecvTable( RecvProp *pProps, int nProps, char *pNetTableName );
		~RecvTable();

		void		Construct( RecvProp *pProps, int nProps, char *pNetTableName );

		int			GetNumProps();
		RecvProp*	GetProp( int i );

		const char*	GetName();

		// Used by the engine while initializing array props.
		void		SetInitialized( bool bInitialized );
		bool		IsInitialized() const;

		// Used by the engine.
		void		SetInMainList( bool bInList );
		bool		IsInMainList() const;


	public:

		// Properties described in a table.
		RecvProp		*m_pProps;
		int				m_nProps;

		// The decoder. NOTE: this covers each RecvTable AND all its children (ie: its children
		// will have their own decoders that include props for all their children).
		CRecvDecoder	*m_pDecoder;

		char			*m_pNetTableName;	// The name matched between client and server.


	private:

		bool			m_bInitialized;
		bool			m_bInMainList;
	};


	inline int RecvTable::GetNumProps() {
		return m_nProps;
	}

	inline RecvProp* RecvTable::GetProp( int i ) {
		return &m_pProps[i];
	}

	inline const char* RecvTable::GetName() {
		return m_pNetTableName;
	}

	inline void RecvTable::SetInitialized( bool bInitialized ) {
		m_bInitialized = bInitialized;
	}

	inline bool RecvTable::IsInitialized() const {
		return m_bInitialized;
	}

	inline void RecvTable::SetInMainList( bool bInList ) {
		m_bInMainList = bInList;
	}

	inline bool RecvTable::IsInMainList() const {
		return m_bInMainList;
	}
}