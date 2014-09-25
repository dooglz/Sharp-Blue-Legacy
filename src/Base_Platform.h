#pragma once
namespace Engine{
	class Platform{
	protected:
		//! Output screen Width, in pixels
		static unsigned short _screenWidth;

		//! Output screen Height, in pixels
		static unsigned short _screenHeight;

		//! Output screen ratio, width/height
		static float _screenRatio;

		virtual void loadDefaultShaders() = 0;

	public:
		Platform();
		virtual ~Platform(){};
		virtual void InitDisplay(const unsigned short width, const unsigned short height) = 0;
		virtual void shutdown() = 0;

		static unsigned short getScreenWidth();
		static unsigned short getScreenHeight();
		static float getScreenRatio();

	};
}