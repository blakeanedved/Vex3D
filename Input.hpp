#ifndef _VEX3D_INPUT_
#define _VEX3D_INPUT_
#include "Core.hpp"

#include <iostream>
#include <unordered_map>
#include <unordered_set>

namespace Vex{
	class Input{
		private:
			GLFWwindow *window;
			std::unordered_map<std::string, int> keycodes;
			static std::unordered_set<int> keysDown; //The keys that were pressed this frame
			static std::unordered_set<int> keysUp; //The keys that were released this frame
			auto static key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) -> void;
		public:
			Input(GLFWwindow *window);
			auto PollInput() -> void;
			auto GetKeyDown(int keycode) -> bool;
			auto GetKeyUp(int keycode) -> bool;
			auto GetKey(int keycode) -> bool;
			auto GetKeyCode(std::string keyname) -> int;
	};
}

Vex::Input::Input(GLFWwindow *window){
	this->window = window;
	glfwSetKeyCallback(window, this->key_callback);
	keycodes = {
		{"Space",32},
		{"Apostrophe",39},
		{"Comma",44},
		{"Minus",45},
		{"Period",46},
		{"Slash",47},
		{"Semicolon",59},
		{"Equal",61},
		{"Left_Bracket",91},
		{"Backslash",92},
		{"Right_Bracket",93},
		{"Grave_Accent",96},
		{"World_1",161},
		{"World_2",162},
		{"Escape",256},
		{"Enter",257},
		{"Tab",258},
		{"Backspace",259},
		{"Insert",260},
		{"Delete",261},
		{"Right",262},
		{"Left",263},
		{"Down",264},
		{"Up",265},
		{"Page_Up",266},
		{"Page_Down",267},
		{"Home",268},
		{"End",269},
		{"Caps_Lock",280},
		{"Scroll_Lock",281},
		{"Num_Lock",282},
		{"Print_Screen",283},
		{"Pause",284},
		{"Keypad_Decimal",330},
		{"Keypad_Divide",331},
		{"Keypad_Multiply",332},
		{"Keypad_Subtract",333},
		{"Keypad_Add",334},
		{"Keypad_Enter",335},
		{"Keypad_Equal",336},
		{"Left_Shift",340},
		{"Left_Control",341},
		{"Left_Alt",342},
		{"Left_Super",343},
		{"Right_Shift",344},
		{"Right_Control",345},
		{"Right_Alt",346},
		{"Right_Super",347},
		{"Menu",348},
	};
	keysDown = {};
	keysUp = {};
};

auto Vex::Input::PollInput() -> void{
	keysDown.clear();
	keysUp.clear();
	glfwPollEvents();
}

auto Vex::Input::GetKeyCode(std::string keyname) -> int{
	int code = -1;
	int length = keyname.length();
	if(length == 1){
		code = (int)keyname[0];
		if(code > 96 && code < 123){
			code -= 32;
		}
	} else if (keyname[0] == 'F'){
		if(length == 3){
			code = ((keyname[1] - '0')*10) + (keyname[2] - '0') + 289;
		}else if(length == 2){
			code = (keyname[1] - '0') + 289;
		}
	} else if (keyname.substr(0,6) == "Keypad" && length == 8){
		code = (keyname[7] - '0') + 320;
	} else {
		auto it = keycodes.find(keyname);
		code = it->second;
	}
	return code;
}

auto Vex::Input::GetKeyDown(int code) -> bool{
	return keysDown.find(code) != keysDown.end();
}
auto Vex::Input::GetKeyUp(int code) -> bool{
	return keysUp.find(code) != keysUp.end();
}
auto Vex::Input::GetKey(int code) -> bool{
	return glfwGetKey(window, code) == GLFW_PRESS;
}
auto Vex::Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) -> void{
	if(action == GLFW_PRESS){
		keysDown.insert(key);	
	}else if(action == GLFW_RELEASE){
		keysUp.insert(key);
	}
}

#endif
