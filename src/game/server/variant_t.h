﻿//====== Copyright � 1996-2005, Valve Corporation, All rights reserved. =======//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef VARIANT_T_H
#define VARIANT_T_H
#ifdef _WIN32
#pragma once
#endif

#include "tier1/string_t.h"
#include "mathlib/vmatrix.h"
#include "mathlib/color.h"
#include "game/shared/ehandle.h"
#include "datamap.h"

// Microsoft headers define these for '_variant_t'
// undefine them to prevent clashes with our struct.
#if defined variant_t
#undef variant_t
#endif variant_t

class CBaseEntity;

//-----------------------------------------------------------------------------
// A variant class for passing data in entity input/output connections.
//-----------------------------------------------------------------------------
class variant_t
{
public:
	// constructor
	variant_t() : fieldType(FIELD_VOID), iVal(0) {}

	inline bool Bool( void ) const						{ return( fieldType == FIELD_BOOLEAN ) ? bVal : false; }
	inline const char *String( void ) const				{ return( fieldType == FIELD_STRING ) ? STRING(iszVal) : ToString(); }
	inline string_t StringID( void ) const				{ return( fieldType == FIELD_STRING ) ? iszVal : NULL_STRING; }
	inline int Int( void ) const						{ return( fieldType == FIELD_INTEGER ) ? iVal : 0; }
	inline float Float( void ) const					{ return( fieldType == FIELD_FLOAT ) ? flVal : 0; }
	inline const CHandle<CBaseEntity> &Entity( void ) const;
	inline color32 Color32( void ) const				{ return rgbaVal; }
	inline void Vector3D( ::Vector3D &vec ) const;

	fieldtype_t FieldType( void ) { return fieldType; }

	void SetBool( bool b ) { bVal = b; fieldType = FIELD_BOOLEAN; }
	void SetString( string_t str ) { iszVal = str, fieldType = FIELD_STRING; }
	void SetInt( int val ) { iVal = val, fieldType = FIELD_INTEGER; }
	void SetFloat( float val ) { flVal = val, fieldType = FIELD_FLOAT; }
	void SetEntity( CBaseEntity *val );
	void SetVector3D( const ::Vector3D &val ) { vecVal[0] = val[ 0 ]; vecVal[ 1 ] = val[ 1 ]; vecVal[ 2 ] = val[ 2 ]; fieldType = FIELD_VECTOR; }
	void SetPositionVector3D( const ::Vector3D &val ) { vecVal[ 0 ] = val[ 0 ]; vecVal[ 1 ] = val[ 1 ]; vecVal[ 2 ] = val[ 2 ]; fieldType = FIELD_POSITION_VECTOR; }
	void SetColor32( color32 val ) { rgbaVal = val; fieldType = FIELD_COLOR32; }
	void SetColor32( int r, int g, int b, int a ) { rgbaVal.r = (byte)r; rgbaVal.g = (byte)g; rgbaVal.b = (byte)b; rgbaVal.a = (byte)a; fieldType = FIELD_COLOR32; }
	void Set( fieldtype_t ftype, void *data );
	void SetOther( void *data );
	bool Convert( fieldtype_t newType );

	// !TODO[AMOS]: If found, make this a ptrand link it to the one in the game engine!
	//static typedescription_t m_SaveBool[];
	//static typedescription_t m_SaveInt[];
	//static typedescription_t m_SaveFloat[];
	//static typedescription_t m_SaveEHandle[];
	//static typedescription_t m_SaveString[];
	//static typedescription_t m_SaveColor[];
	//static typedescription_t m_SaveVector[];
	//static typedescription_t m_SavePositionVector[];
	//static typedescription_t m_SaveVMatrix[];
	//static typedescription_t m_SaveVMatrixWorldspace[];
	//static typedescription_t m_SaveMatrix3x4Worldspace[];

protected:
	//-------------------------------------------------------------------------
	// Returns a string representation of the value without modifying the variant.
	//-------------------------------------------------------------------------
	const char *ToString( void ) const;

	friend class CVariantSaveDataOps;

private:
	union
	{
		bool bVal;
		string_t iszVal;
		int iVal;
		float flVal;
		float vecVal[3];
		color32 rgbaVal;
	};
	CHandle<CBaseEntity> eVal; // this can't be in the union because it has a constructor.

	fieldtype_t fieldType;
};


//-----------------------------------------------------------------------------
// Purpose: Returns this variant as a vector.
//-----------------------------------------------------------------------------
inline void variant_t::Vector3D( ::Vector3D &vec ) const
{
	if (( fieldType == FIELD_VECTOR ) || ( fieldType == FIELD_POSITION_VECTOR ))
	{
		vec[ 0 ] =  vecVal[ 0 ];
		vec[ 1 ] =  vecVal[ 1 ];
		vec[ 2 ] =  vecVal[ 2 ];
	}
	else
	{
		vec = vec3_origin;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Returns this variant as an EHANDLE.
//-----------------------------------------------------------------------------
inline const CHandle<CBaseEntity> &variant_t::Entity( void ) const
{
	if ( fieldType == FIELD_EHANDLE )
		return eVal;

	static const CHandle<CBaseEntity> hNull( INVALID_EHANDLE );
	return( hNull );
}

#endif // VARIANT_T_H
