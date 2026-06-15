#include "efsw/String.hpp"
#include "test_util.hpp"
#include "utest.h"
#include <cstdint>

using namespace efsw_test;

UTEST( Encoding, Utf32String ) {
	const auto emojiString = efsw::String( U"🐈🏳️‍🌈🇺🇸0️⃣✈️™️❤️" );

	// Test utf32 -> utf32
	const size_t expectedUtf32ByteCount = 16;
	const char32_t expectedUtf32Bytes[expectedUtf32ByteCount] = {
		0x0001f408, 0x0001f3f3, 0x0000fe0f, 0x0000200d, 0x0001f308, 0x0001f1fa,
		0x0001f1f8, 0x00000030, 0x0000fe0f, 0x000020e3, 0x00002708, 0x0000fe0f,
		0x00002122, 0x0000fe0f, 0x00002764, 0x0000fe0f };
	const auto expectedUtf32String = std::u32string( expectedUtf32Bytes, expectedUtf32ByteCount );
	EXPECT_EQ( expectedUtf32String, emojiString );

	// Test utf32 -> utf8
	const auto utf8 = emojiString.toUtf8();

	const size_t expectedUtf8ByteCount = 51;
	unsigned char expectedUtf8Bytes[expectedUtf8ByteCount] = {
		0xf0, 0x9f, 0x90, 0x88, 0xf0, 0x9f, 0x8f, 0xb3, 0xef, 0xb8, 0x8f, 0xe2, 0x80,
		0x8d, 0xf0, 0x9f, 0x8c, 0x88, 0xf0, 0x9f, 0x87, 0xba, 0xf0, 0x9f, 0x87, 0xb8,
		0x30, 0xef, 0xb8, 0x8f, 0xe2, 0x83, 0xa3, 0xe2, 0x9c, 0x88, 0xef, 0xb8, 0x8f,
		0xe2, 0x84, 0xa2, 0xef, 0xb8, 0x8f, 0xe2, 0x9d, 0xa4, 0xef, 0xb8, 0x8f };
	const auto expectedUtf8String =
		std::string( reinterpret_cast<const char*>( expectedUtf8Bytes ), expectedUtf8ByteCount );

	EXPECT_EQ( expectedUtf8String, utf8 );
}

UTEST( Encoding, WideString ) {

	const auto emojiString = efsw::String( L"🐈🏳️‍🌈🇺🇸0️⃣✈️™️❤️" );

	// Test wide -> utf32
	const size_t expectedWideByteCount = 21;
	const wchar_t expectedWideBytes[expectedWideByteCount] = {
		0xd83d, 0xdc08, 0xd83c, 0xdff3, 0xfe0f, 0x200d, 0xd83c, 0xdf08, 0xd83c, 0xddfa, 0xd83c,
		0xddf8, 0x0030, 0xfe0f, 0x20e3, 0x2708, 0xfe0f, 0x2122, 0xfe0f, 0x2764, 0xfe0f };
	const auto expectedWideString = std::wstring( expectedWideBytes, expectedWideByteCount );

	const auto wide = emojiString.toWideString();
	EXPECT_EQ( expectedWideString, wide );

	// Test utf32 -> utf8
	const auto utf8 = emojiString.toUtf8();

	const size_t expectedUtf8ByteCount = 51;
	unsigned char expectedUtf8Bytes[expectedUtf8ByteCount] = {
		0xf0, 0x9f, 0x90, 0x88, 0xf0, 0x9f, 0x8f, 0xb3, 0xef, 0xb8, 0x8f, 0xe2, 0x80,
		0x8d, 0xf0, 0x9f, 0x8c, 0x88, 0xf0, 0x9f, 0x87, 0xba, 0xf0, 0x9f, 0x87, 0xb8,
		0x30, 0xef, 0xb8, 0x8f, 0xe2, 0x83, 0xa3, 0xe2, 0x9c, 0x88, 0xef, 0xb8, 0x8f,
		0xe2, 0x84, 0xa2, 0xef, 0xb8, 0x8f, 0xe2, 0x9d, 0xa4, 0xef, 0xb8, 0x8f };
	const auto expectedUtf8String =
		std::string( reinterpret_cast<const char*>( expectedUtf8Bytes ), expectedUtf8ByteCount );

	EXPECT_EQ( expectedUtf8String, utf8 );

	// High surrogate 0xD83D followed by 'A'instead of a low surrogate should be rejected
	const wchar_t replacement = L'\uFFFD';
	auto invalidPair = efsw::String( L"\xD83D\x0041" );
	EXPECT_EQ( invalidPair.data()[0], replacement );
	EXPECT_EQ( invalidPair.data()[1], L'\u0041' );

	// Test that single-character surrugate pairs are rejected
	const auto rejectedString = efsw::String( static_cast<wchar_t>( 0xDA00u ) );

	EXPECT_EQ( rejectedString.data()[0], replacement );
}