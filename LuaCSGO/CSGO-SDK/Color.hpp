#pragma once

#include "Definitions.hpp"

//warning C4333: '>>' : right shift by too large amount, data loss
#pragma warning(disable:4333)
namespace SDK {
	class Color {
	public:
		// constructors
		Color() {
			*((int *)this) = 0;
		}
		Color( int r, int g, int b ) {
			SetColor( r, g, b, 255 );
		}
		Color( int r, int g, int b, int a ) {
			SetColor( r, g, b, a );
		}
		void SetColor( int r, int g, int b, int a = 0 ) {
			_color[0] = (unsigned char)r;
			_color[1] = (unsigned char)g;
			_color[2] = (unsigned char)b;
			_color[3] = (unsigned char)a;
		}

		void GetColor( int &r, int &g, int &b, int &a ) const {
			r = _color[0];
			g = _color[1];
			b = _color[2];
			a = _color[3];
		}

		void SetRawColor( int color32 ) {
			*((int *)this) = color32;
		}

		static Color FromARGB( unsigned long color ) {
			int a = (color & 0xFF000000) >> 24;
			int r = (color & 0x00FF0000) >> 16;
			int g = (color & 0x0000FF00) >> 8;
			int b = (color & 0x000000FF);

			return Color( r, g, b, a );
		}

		Color SetARGB( unsigned long color ) {
			int a = (color & 0xFF000000) >> 24;
			int r = (color & 0x00FF0000) >> 16;
			int g = (color & 0x0000FF00) >> 8;
			int b = (color & 0x000000FF);

			SetColor( r, g, b, a );
		}

		int GetRawColor() const {
			return *((int *)this);
		}

		inline int r() const { return _color[0]; }
		inline int g() const { return _color[1]; }
		inline int b() const { return _color[2]; }
		inline int a() const { return _color[3]; }

		unsigned char &operator[]( int index ) {
			return _color[index];
		}

		const unsigned char &operator[]( int index ) const {
			return _color[index];
		}

		bool operator == (const Color &rhs) const {
			return (*((int *)this) == *((int *)&rhs));
		}

		bool operator != (const Color &rhs) const {
			return !(operator==(rhs));
		}

		Color &operator=(const Color &rhs) {
			SetRawColor( rhs.GetRawColor() );
			return *this;
		}

		operator const unsigned char*() {
			return (unsigned char*)(&_color[0]);
		}

		operator int() {
			return ((int)((((_color[3]) & 0xff) << 24) | (((_color[0]) & 0xff) << 16) | (((_color[1]) & 0xff) << 8) | ((_color[2]) & 0xff)));
		}
		operator unsigned int() {
			return ((unsigned int)((((_color[3]) & 0xff) << 24) | (((_color[0]) & 0xff) << 16) | (((_color[1]) & 0xff) << 8) | ((_color[2]) & 0xff)));
		}
		operator unsigned long() {
			return ((unsigned long)((((_color[3]) & 0xff) << 24) | (((_color[0]) & 0xff) << 16) | (((_color[1]) & 0xff) << 8) | ((_color[2]) & 0xff)));
		}
	private:
		unsigned char _color[4];
	};
}