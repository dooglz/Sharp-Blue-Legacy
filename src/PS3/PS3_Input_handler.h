#pragma once
#include "Utilities.h"		// asserts and commmon defines
#include <cell/pad.h>
#include <string>

namespace Engine{
	namespace PS3{
		//! port_no Controller number (0-6), this class currently only supports 1 controller
#define PORT_NO 0

		//! Screen dimensions in pixels for calculating pointer position
#define SCREENX 720
#define SCREENY 480

		//! How much the analogue sticks have to move befre they trigger anything.
#define DEADZONE 25.0f

		//! Speed multiplier of the pointer
#define SENSITIVITY 100.0f

			//! Enums of all the digital buttons on a ps3 controller
			enum PadButtons {
				INPUT_SELECT = 0,
				INPUT_L3 = 1,
				INPUT_R3 = 2,
				INPUT_START = 3,
				INPUT_UP = 4,
				INPUT_RIGHT = 5,
				INPUT_DOWN = 6,
				INPUT_LEFT = 7,
				INPUT_L2 = 8,
				INPUT_R2 = 9,
				INPUT_L1 = 10,
				INPUT_R1 = 11,
				INPUT_TRIANGLE = 12,
				INPUT_CIRCLE = 13,
				INPUT_CROSS = 14,
				INPUT_SQUARE = 15,
				PADBUTTONS_MAX
			};
			const std::string padbuttons_names[16] =
			{ "select", "l3", "r3", "start", "dpad_u", "dpad_r", "dpad_d", "dpad_l", "l2", "r2", "l1", "r1", "y", "b", "a", "x" };

			//The should be offest by +4 when looking up data[]
			enum analogueInput{
				ALOG_RIGHT_STICK_X = 0,
				ALOG_RIGHT_STICK_Y = 1,
				ALOG_LEFT_STICK_X = 2,
				ALOG_LEFT_STICK_Y = 3,
				ALOG_RIGHT = 4,
				ALOG_LEFT = 5,
				ALOG_UP = 6,
				ALOG_DOWN = 7,
				ALOG_TRIANGLE = 8,
				ALOG_CIRCLE = 9,
				ALOG_CROSS = 10,
				ALOG_SQUARE = 11,
				ALOG_L1 = 12,
				ALOG_R1 = 13,
				ALOG_L2 = 14,
				ALOG_R2 = 15,
				ALOG_SIXAXIS_X = 16,
				ALOG_SIXAXIS_Y = 17,
				ALOG_SIXAXIS_Z = 18,
				ALOG_SIXAXIS_ANG = 19
			};
			const std::string analogue_names[16] =
			{ "rs_x", "rs_y", "ls_x", "ls_y", "dpad_r_a", "dpad_l_a", "dpad_u_a", "dpad_d_a", "y_a", "b_a", "a_a",
			"x_a", "l1_a", "r1_a", "l2_a", "r2_a" };

			class PS3_Input
			{
			public:

				//! Call cellPadInit() and initialize arrays.
				static void Initialise();

				//! Release controll of controller.
				static void Destroy();

				//! Retreive and parse controller info, call funcs if needed.
				static void Update();

				//! Clear buffers and reset pointer position.
				static void Refresh();

			protected:

				static bool _pressureEnabled;

			};

	}
}