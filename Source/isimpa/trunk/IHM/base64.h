#include "first_header_include.hpp"

//*********************************************************************
//* C_Base64 - a simple base64 encoder and decoder.
//*
//*     Copyright (c) 1999, Bob Withers - bwit@pobox.com
//*
//* This code may be freely used for any purpose, either personal
//* or commercial, provided the authors copyright notice remains
//* intact.
//*********************************************************************
//
// converted to wxWindows by Frank Buß
//

#ifndef BASE64_H
#define BASE64_H

#include <wx/wx.h>

/**
 * Helper class for encoding / decoding base64.
 */
class wxBase64
{
public:
	/**
	 * Converts a byte array to a base64 encoded string.
	 * \param pData The byte array.
	 * \param length The length of the byte array.
	 * \return The base64 representation of the byte array.
	 */
	static wxString Encode(const wxUint8* pData, size_t length);

	/**
	 * Decodes a base64 encoded string.
	 * \param data The base64 encoded string.
	 * \return The decoded binary string (may contain 0).
	 */
    static wxString Decode(const wxString& data);
};

#endif
