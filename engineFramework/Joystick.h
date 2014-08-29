#pragma once

#include "Lib.h"


//lib namespace
namespace core
{

	class joystick
	{
	public:
		enum joy_id
		{
			joystick_01 = 0,
			joystick_02,
			joystick_03,
			joystick_04,
			joystick_05,
			joystick_06,
			joystick_07,
			joystick_08,
			joystick_09,
			joystick_10,
			joystick_11,
			joystick_12,
			joystick_13,
			joystick_14,
			joystick_15,
			joystick_16
		};

		enum axis
		{
			axis_01 = 0,
			axis_02,
			axis_03,
			axis_04,
			axis_05,
			axis_06,
			axis_07,
			axis_08,
			axis_09,
			axis_10,
			axis_11,
			axis_12,
			axis_13,
			axis_14,
			axis_15,
			axis_16
		};

		enum button
		{
			button_01 = 0,
			button_02,
			button_03,
			button_04,
			button_05,
			button_06,
			button_07,
			button_08,
			button_09,
			button_10,
			button_11,
			button_12,
			button_13,
			button_14,
			button_15,
			button_16
		};

		struct joystick_info
		{
			bool active;
			unsigned short num_axis;
			unsigned short num_buttons;
			float  axis_info[16];
			unsigned char button_info[16];

			joystick_info(void) : active(false), num_axis(0), num_buttons(0)
			{
				memset(axis_info, 0, sizeof(float)* 16);
				memset(button_info, 0, sizeof(bool)* 16);
			}

			void reset(void)
			{
				active = false;
				num_axis = 0;
				num_buttons = 0;

				memset(axis_info, 0, sizeof(float)* 16);
				memset(button_info, 0, sizeof(bool)* 16);
			}
		};

	private:
		joystick_info _status[16];

		typedef void(*user_joystick_callback_function)(joy_id id, joystick_info info);

		static user_joystick_callback_function _on_attach_function;
		static user_joystick_callback_function _on_deattach_function;

	public:
		joystick(void);
		~joystick(void);

		void poll_events(void);

		float get_axis(joy_id joystick_id, axis axis_id);
		bool get_button(joy_id joystick_id, button button_id);

		void set_on_attach_function(void(*function)(joy_id id, joystick_info info));
		void set_on_deattach_function(void(*function)(joy_id id, joystick_info info));
	};

}