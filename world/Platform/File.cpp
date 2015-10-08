#include "File.h"

String GetFileName( const String &file ) {
	int index = file.FindBack( u'/' );
	if ( index < 0 ) {
		return String();
	}
	return String( file, index + 1, file.Length() - index - 1 );
}

String GetFileExt( const String &file ) {
	int dot = file.FindBack( u'.' );
	
	// tecka nenalezena soubor nema zadnou priponu
	if ( dot < 0 ) {
		return String();
	}
	// pokud je lomitko pred teckou, nema soubor zadnou priponu (tecka je pravdepodobne soucasti nazvu slozky)
	int slash = file.FindBack( u'/' );
	if ( slash > dot ) {
		return String();
	}
	return String( file, dot + 1, file.Length() - dot - 1 );
}

String GetFileBase( const String &file ) {
	int baseBegin = 0;
	int slash = file.FindBack( u'/' );
	if ( slash >= 0 ) {
		baseBegin = slash + 1;
	}
	int baseEnd = file.Length();
	int dot = file.FindBack( u'.' );
	if ( dot >= 0 ) {
		baseEnd = dot;
	}
	return String( file, baseBegin, baseEnd - baseBegin );
}

String GetFileDir( const String &file ) {
	int slash = file.FindBack( u'/' );
	return String( file, 0, slash + 1 );
}

// class File

IFile::IFile() { /* does nothing */ }

IFile::~IFile() { /* does nothing */ }

Byte IFile::ReadByte() {
	Byte byte = 0;
	Read( &byte, 1 );
	return byte;
}

Int32 IFile::ReadInt32() {
	Int32 value = 0;
	Read( &value, sizeof( Int32 ) );
	return value;
}

Uint32 IFile::ReadUint32() {
	Uint32 value = 0;
	Read( &value, sizeof( Uint32 ) );
	return value;
}

float IFile::ReadFloat() {
	float value = 0;
	Read( &value, sizeof( float ) );
	return value;
}

Float2 IFile::ReadFloat2() {
	Float2 value;
	Read( &value, sizeof( Float2 ) );
	return value;
}

Float3 IFile::ReadFloat3() {
	Float3 value;
	Read( &value, sizeof( Float3 ) );
	return value;
}

Float4 IFile::ReadFloat4() {
	Float4 value;
	Read( &value, sizeof( Float4 ) );
	return value;
}

void IFile::WriteByte( const Byte value ) {
	Write( &value, sizeof( Byte ) );
}

void IFile::WriteInt32( const Int32 value ) {
	Write( &value, sizeof( Int32 ) );
}

void IFile::WriteUint32( const Uint32 value ) {
	Write( &value, sizeof( Uint32 ) );
}

void IFile::WriteFloat( const float value ) {
	Write( &value, sizeof( Float2 ) );
}

void IFile::WriteFloat2( const Float2 &value ) {
	Write( &value, sizeof( float ) * 2 );
}

void IFile::WriteFloat3( const Float3 &value ) {
	Write( &value, sizeof( float ) * 3 );
}

void IFile::WriteFloat4( const Float4 &value ) {
	Write( &value, sizeof( float ) * 4 );
}

// namespace FileSystem

bool FileSystem::CreateDir( const String &path ) {
	return FileSystemPlatform::CreateDir( path );
}

bool FileSystem::RemoveDir( const String &path ) {
	return FileSystemPlatform::RemoveDir( path );
}

bool FileSystem::RemoveDirContent( const String &path ) {
	return FileSystemPlatform::RemoveDirContent( path );
}

bool FileSystem::RemoveFile( const String &fullname ) {
	return FileSystemPlatform::RemoveFile( fullname );
}

bool FileSystem::EnumFiles( const String &path, Array< String > &result ) {
	return FileSystemPlatform::EnumFiles( path, result );
}

bool FileSystem::EnumDirs( const String &path, Array< String > &result ) {
	return FileSystemPlatform::EnumDirs( path, result );
}