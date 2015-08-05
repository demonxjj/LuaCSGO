#pragma once
#include "CSGO-SDK/SDK.hpp"

#include <locale>
#include <codecvt>
#include <string>

namespace LuaCSGO
{
	class DrawManager {
	public:
		unsigned long CreateNewFont( const char* fontFace, int size, bool bold, bool italic, bool outlined, bool dropShadow ) {
			unsigned long font = SDK::Interfaces::g_pVGuiSurface->CreateFont();
			int flags = ( italic ? SDK::FontFlags::FONTFLAG_ITALIC : 0 ) | ( outlined ? SDK::FontFlags::FONTFLAG_OUTLINE : 0 ) | ( dropShadow ? SDK::FontFlags::FONTFLAG_DROPSHADOW : 0 );
			SDK::Interfaces::g_pVGuiSurface->SetFontGlyphSet( font, fontFace, size, bold ? FW_BOLD : 0, 0, 0, flags );
			return font;
		}

		void DrawLine( int x1, int y1, int x2, int y2 ) {
			SDK::Interfaces::g_pVGuiSurface->DrawLine( x1, y1, x2, y2 );
		}

		void DrawFilledCircle( int x, int y, int r ) {

			SDK::Interfaces::g_pVGuiSurface->DrawSetTexture( 1 );
			SDK::Interfaces::g_pVGuiSurface->DrawSetColor( m_CurrentColor );

			int n = 32;
			SDK::Vertex_t vertices[32];

			for(int i = 0; i < n; i++) {
				float angle = ( 2.0f * 3.1415926f * (float)i / (float)n );
				float _x = x + r * cos( angle );
				float _y = y + r * sin( angle );

				vertices[i] = SDK::Vector2D( _x, _y );
			}

			SDK::Interfaces::g_pVGuiSurface->DrawTexturedPolygon( n, vertices );
		}

		void DrawFilledRect( int x, int y, int w, int h ) {
			SDK::Interfaces::g_pVGuiSurface->DrawFilledRect( x, y, x + w, y + h );
		}

		void FillGradient( int x, int y, int w, int h, unsigned long to, bool vertical ) {
			SDK::Color color1 = m_CurrentColor;
			SDK::Color color2 = SDK::Color::FromARGB( to );

			if(vertical) {
				for(int i = 0; i < h; i++) {
					int r, g, b, a;
					a = color1.a() + ( i * ( color2.a() - color1.a() ) / h );
					r = color1.r() + ( i * ( color2.r() - color1.r() ) / h );
					g = color1.g() + ( i * ( color2.g() - color1.g() ) / h );
					b = color1.b() + ( i * ( color2.b() - color1.b() ) / h );
					SDK::Interfaces::g_pVGuiSurface->DrawSetColor( SDK::Color( r, g, b, a ) );
					SDK::Interfaces::g_pVGuiSurface->DrawLine( x, i + y, x + w, i + y );
				}
			} else {
				for(int i = 0; i < w; i++) {
					int r, g, b, a;
					a = color1.a() + ( i * ( color2.a() - color1.a() ) / w );
					r = color1.r() + ( i * ( color2.r() - color1.r() ) / w );
					g = color1.g() + ( i * ( color2.g() - color1.g() ) / w );
					b = color1.b() + ( i * ( color2.b() - color1.b() ) / w );
					SDK::Interfaces::g_pVGuiSurface->DrawSetColor( SDK::Color( r, g, b, a ) );
					SDK::Interfaces::g_pVGuiSurface->DrawLine( i + x, y, i + x, y + h );
				}
			}
			SDK::Interfaces::g_pVGuiSurface->DrawSetColor( color1 );
		}

		void DrawOutlinedCircle( int x, int y, int r, unsigned long outlineColor ) {
			SDK::Interfaces::g_pVGuiSurface->DrawOutlinedCircle( x, y, r, 32 );
			SDK::Interfaces::g_pVGuiSurface->DrawSetColor( SDK::Color::FromARGB( outlineColor ) );
			r -= 1;
			SDK::Interfaces::g_pVGuiSurface->DrawOutlinedCircle( x, y, r, 32 );
			r += 2;
			SDK::Interfaces::g_pVGuiSurface->DrawOutlinedCircle( x, y, r, 32 );
			SDK::Interfaces::g_pVGuiSurface->DrawSetColor( m_CurrentColor );
		}

		void DrawCircle( int x, int y, int r ) {
			SDK::Interfaces::g_pVGuiSurface->DrawOutlinedCircle( x, y, r, 32 );
		}

		void DrawOutlinedRect( int x, int y, int w, int h, unsigned long outlineColor ) {
			SDK::Interfaces::g_pVGuiSurface->DrawOutlinedRect( x, y, x + w, y + h );
			SDK::Interfaces::g_pVGuiSurface->DrawSetColor( SDK::Color::FromARGB( outlineColor ) );
			x -= 1; y -= 1;
			w += 2; h += 2;
			SDK::Interfaces::g_pVGuiSurface->DrawOutlinedRect( x, y, x + w, y + h );
			x += 2; y += 2;
			w -= 4; h -= 4;
			SDK::Interfaces::g_pVGuiSurface->DrawOutlinedRect( x, y, x + w, y + h );
			SDK::Interfaces::g_pVGuiSurface->DrawSetColor( m_CurrentColor );
		}

		void DrawRect( int x, int y, int w, int h ) {
			SDK::Interfaces::g_pVGuiSurface->DrawOutlinedRect( x, y, x + w, y + h );
		}

		void DrawText( unsigned long fontId, int x, int y, const char* text ) {
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::wstring wtext = converter.from_bytes( text );

			SDK::Interfaces::g_pVGuiSurface->DrawSetTextPos( x, y );

			SDK::Interfaces::g_pVGuiSurface->DrawSetTextFont( fontId );
			SDK::Interfaces::g_pVGuiSurface->DrawPrintText( wtext.c_str(), wtext.size() );
		}

		SDK::Vector2D MeasureText( unsigned long fontId, const char* text ) {
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::wstring wtext = converter.from_bytes( text );
			int w, h;
			SDK::Interfaces::g_pVGuiSurface->GetTextSize( fontId, wtext.c_str(), w, h );
			return SDK::Vector2D(w, h);
		}

		unsigned long GetDrawColor() {
			return m_CurrentColor.GetARGB();
		}

		void SetDrawColor( unsigned long argb ) {
			m_CurrentColor.SetARGB( argb );
			SDK::Interfaces::g_pVGuiSurface->DrawSetColor( m_CurrentColor );
			SDK::Interfaces::g_pVGuiSurface->DrawSetTextColor( m_CurrentColor );
		}

	private:
		SDK::Color m_CurrentColor;
	};

}