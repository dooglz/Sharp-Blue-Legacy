#include "PS3_Input_handler.h"
#include "Input.h"

namespace Engine{
	namespace PS3{
		bool  PS3_Input::_pressureEnabled;

		// Call cellPadInit() and initialize arrays.
		void PS3_Input::Initialise()
		{
			int err = cellPadInit(1);
			DBG_ASSERT_MSG(err == CELL_PAD_OK, "cellPadInit failed !");
			//Refresh();

			Input::addAxis("gp_1_dpad_l");
			Input::addAxis("gp_1_dpad_r");
			Input::addAxis("gp_1_dpad_u");
			Input::addAxis("gp_1_dpad_d");
			Input::addAxis("gp_1_a");
			Input::addAxis("gp_1_b");
			Input::addAxis("gp_1_x");
			Input::addAxis("gp_1_y");
			Input::addAxis("gp_1_start");
			Input::addAxis("gp_1_select");
		}

		// Release controll of controller.
		void PS3_Input::Destroy()
		{
			cellPadEnd();
		}

		// Clear buffers and reset pointer position.
		void PS3_Input::Refresh()
		{
			int err = cellPadClearBuf(PORT_NO);
			DBG_ASSERT_MSG(err == CELL_PAD_OK, "cellPadClearBuf failed !");
		}

		// Retreive and parse controller info, call funcs if needed.
		void PS3_Input::Update()
		{
			CellPadData data;
			char ctrler[5];
			cellPadGetData(PORT_NO, &data);
			sprintf(ctrler, "gp_%i_", (PORT_NO + 1));

			if (data.len > 0)  // Has Pad status changed ?
			{
				//data.button[2] : Digital button statuses 
				for (int i = 0; i < PADBUTTONS_MAX / 2; ++i)
				{
					Input::setValue(ctrler + padbuttons_names[i], 255 * ((bool)(data.button[2] & (1 << i))));
				}
				// data.button [3] : More Digital button statuses 
				for (int i = 0; i < PADBUTTONS_MAX / 2; ++i)
				{
					Input::setValue(ctrler + padbuttons_names[i + 8], 255 * ((bool)(data.button[3] & (1 << i))));
				}

				//---- Analogue Sticks-----
				// data is between 0x0 and 0x00FF (255)

				//-- Right stick
				Input::setValue(ctrler + analogue_names[ALOG_RIGHT_STICK_X], (unsigned char)(data.button[ALOG_RIGHT_STICK_X + 4]));
				Input::setValue(ctrler + analogue_names[ALOG_RIGHT_STICK_Y], (unsigned char)(data.button[ALOG_RIGHT_STICK_Y + 4]));

				//-- Left stick
				Input::setValue(ctrler + analogue_names[ALOG_LEFT_STICK_X], (unsigned char)(data.button[ALOG_LEFT_STICK_X + 4]));
				Input::setValue(ctrler + analogue_names[ALOG_LEFT_STICK_Y], (unsigned char)(data.button[ALOG_LEFT_STICK_Y + 4]));

				//---- Button pressure -----
				// data is between 0x0 and 0x00FF (255)
				// data.button[8->19] : pressure info (requires cellPadSetPortSetting())
				if (_pressureEnabled)
				{
					for (int i = 4; i < 20; ++i)
					{
						if (data.button[i + 4] != 0)
						{

						}
					}
				}

				//---- Sixaxis -----
				// data is between 0x0 and 0x03FF (1023)
				// data.button[20->23] : Sixaxis info (requires cellPadSetPortSetting())
			}
		}
	}
}