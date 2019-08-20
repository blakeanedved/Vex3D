#ifndef _VEX3D_INPUT_
#define _VEX3D_INPUT_
#include "Core.hpp"

#include <iostream>
#include <unordered_map>
#include <unordered_set>

namespace Vex{
	namespace Input{
		GLFWwindow* window;	
		std::unordered_map<std::string, int>keycodes = {
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
			{"A",65},
			{"B",66},
			{"C",67},
			{"E",69},
			{"D",68},
			{"F",70},
			{"G",71},
			{"H",72},
			{"I",73},
			{"J",74},
			{"K",75},
			{"L",76},
			{"M",77},
			{"N",78},
			{"O",79},
			{"P",80},
			{"Q",81},
			{"R",82},
			{"S",83},
			{"T",84},
			{"U",85},
			{"V",86},
			{"W",87},
			{"X",88},
			{"Y",89},
			{"Z",90},
			{"a",65},
			{"b",66},
			{"c",67},
			{"d",68},
			{"e",69},
			{"f",70},
			{"g",71},
			{"h",72},
			{"i",73},
			{"j",74},
			{"k",75},
			{"l",76},
			{"m",77},
			{"n",78},
			{"o",79},
			{"p",80},
			{"q",81},
			{"r",82},
			{"s",83},
			{"t",84},
			{"u",85},
			{"v",86},
			{"w",87},
			{"x",88},
			{"y",89},
			{"z",90},
			{"0",48},
			{"1",49},
			{"2",50},
			{"3",51},
			{"4",52},
			{"5",53},
			{"6",54},
			{"7",55},
			{"8",56},
			{"9",57},
			{"Keypad_0",320},
			{"Keypad_1",321},
			{"Keypad_2",322},
			{"Keypad_3",323},
			{"Keypad_4",324},
			{"Keypad_5",325},
			{"Keypad_6",326},
			{"Keypad_7",327},
			{"Keypad_8",328},
			{"Keypad_9",329},
			{"F1",290},
			{"F2",291},
			{"F3",292},
			{"F4",293},
			{"F5",294},
			{"F6",295},
			{"F7",296},
			{"F8",297},
			{"F9",298},
			{"F10",299},
			{"F11",300},
			{"F12",301},
			{"F13",302},
			{"F14",303},
			{"F15",304},
			{"F16",305},
			{"F17",306},
			{"F18",307},
			{"F19",308},
			{"F20",309},
			{"F21",310},
			{"F22",311},
			{"F23",312},
			{"F24",313},
			{"F25",314}
		};
		std::unordered_map<int,int> keyPresses = {}; //The keys that were pressed or released this frame. 0 = nothing, 1 = pressed, 2 = released.
		int mousePresses[3] = {0}; // same as above
		glm::vec2 currentMousePos = glm::vec2(0,0);
		glm::vec2 prevMousePos = glm::vec2(0,0);

		auto PollInput() -> void{
			for (auto& c : keyPresses){
				c.second = 0;
			}
			mousePresses[0] = 0;
			mousePresses[1] = 0;
			mousePresses[2] = 0;
			prevMousePos = currentMousePos;
			glfwPollEvents();
		}

		auto GetKeyCode(const std::string& keyname) -> int{
			if (auto it = keycodes.find(keyname); it != keycodes.end()){
			    return it->second;
			} else {
				std::cout << "Vex Input Warning: " << keyname << " could not be found." << std::endl;
			    return -1;
			}
		}

		auto GetKeyDown(int code) -> bool{
			if(keyPresses.find(code) == keyPresses.end()){
				std::cout << "Vex Input Error: " << code << " is not a valid key code." << std::endl;
				return false;
			}
			return keyPresses[code] == 1;
		}
		auto GetKeyUp(int code) -> bool{
			if(keyPresses.find(code) == keyPresses.end()){
				std::cout << "Vex Input Error: " << code << " is not a valid key code." << std::endl;
				return false;
			}
			return keyPresses[code] == 2;
		}
		auto GetKey(int code) -> bool{
			return glfwGetKey(window, code) == GLFW_PRESS;
		}
		auto GetKeyDown(const std::string& code) -> bool{
			int kcode = GetKeyCode(code);
			if(keyPresses.find(kcode) == keyPresses.end()){
				std::cout << "Vex Input Error: " << code << " is not a valid key." << std::endl;
				return false;
			}
			return keyPresses[kcode] == 1;
		}
		auto GetKeyUp(const std::string& code) -> bool{
			int kcode = GetKeyCode(code);
			if(keyPresses.find(kcode) == keyPresses.end()){
				std::cout << "Vex Input Error: " << code << " is not a valid key." << std::endl;
				return false;	
			}
			return keyPresses[kcode] == 2;
		}
		auto GetKey(const std::string& code) -> bool{
			return glfwGetKey(window, GetKeyCode(code)) == GLFW_PRESS;
		}

		auto key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) -> void{
			if(action == GLFW_PRESS){
				keyPresses[key] = 1;
			}else if(action == GLFW_RELEASE){
				keyPresses[key] = 2;
			}
		}
		auto mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
			if(action == GLFW_PRESS){
				mousePresses[button] = 1;
			}else if(action == GLFW_RELEASE){
				mousePresses[button] = 2;
			}
		}
		auto cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
			prevMousePos = currentMousePos;
			currentMousePos = glm::vec2(xpos, ypos);
		}

		auto GetMousePosition() -> glm::vec2{
			return currentMousePos;	
		}
		auto GetMouseDelta() -> glm::vec2{
			return currentMousePos - prevMousePos;
		}

		auto GetMouseButton(int button) -> bool{
			if(button > -1 && button < 3){
				return glfwGetMouseButton(window, button) == GLFW_PRESS;
			}else{
				std::cout << "Vex Input Error: There is no mouse button " << button << ". Valid mouse buttons are 0, 1, and 2." << std::endl;
				return false;
			}
		}
		auto GetMouseButtonDown(int button) -> bool{
			if(button > -1 && button < 3){
				return mousePresses[button] == 1;
			}else{
				std::cout << "Vex Input Error: There is no mouse button " << button << ". Valid mouse buttons are 0, 1, and 2." << std::endl;
				return false;
			}
		}
		auto GetMouseButtonUp(int button) -> bool{
			if(button > -1 && button < 3){
				return mousePresses[button] == 2;
			}else{
				std::cout << "Vex Input Error: There is no mouse button " << button << ". Valid mouse buttons are 0, 1, and 2." << std::endl;
				return false;
			}
		}
		auto InputInit(GLFWwindow* w) -> void{
			window = w;
			glfwSetKeyCallback(window, key_callback);
			glfwSetMouseButtonCallback(window, mouse_button_callback);
			glfwSetCursorPosCallback(window, cursor_position_callback);
			for (auto& c : keycodes){
				keyPresses.insert(std::make_pair(c.second, 0));
			}
		};
	};		
}

#endif
