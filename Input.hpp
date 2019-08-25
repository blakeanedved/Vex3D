#ifndef _VEX3D_INPUT_
#define _VEX3D_INPUT_
#include "Core.hpp"

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <cctype>
#include <boost/algorithm/string.hpp>

namespace Vex{
	namespace Input{
		const bool KEY_DOWN = true;
		const bool KEY_UP = false;
		GLFWwindow* window;	
		std::unordered_map<std::string, int>keycodes = {
			{"space",32},
			{"apostrophe",39},
			{"comma",44},
			{"minus",45},
			{"period",46},
			{"slash",47},
			{"semicolon",59},
			{"equal",61},
			{"left_bracket",91},
			{"backslash",92},
			{"right_bracket",93},
			{"grave_accent",96},
			{"world_1",161},
			{"world_2",162},
			{"escape",256},
			{"enter",257},
			{"tab",258},
			{"backspace",259},
			{"insert",260},
			{"delete",261},
			{"right",262},
			{"left",263},
			{"down",264},
			{"up",265},
			{"page_up",266},
			{"page_down",267},
			{"home",268},
			{"end",269},
			{"caps_lock",280},
			{"scroll_lock",281},
			{"num_lock",282},
			{"print_screen",283},
			{"pause",284},
			{"keypad_decimal",330},
			{"keypad_divide",331},
			{"keypad_multiply",332},
			{"keypad_subtract",333},
			{"keypad_add",334},
			{"keypad_enter",335},
			{"keypad_equal",336},
			{"left_shift",340},
			{"left_control",341},
			{"left_alt",342},
			{"left_super",343},
			{"right_shift",344},
			{"right_control",345},
			{"right_alt",346},
			{"right_super",347},
			{"menu",348},
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
			{"keypad_0",320},
			{"keypad_1",321},
			{"keypad_2",322},
			{"keypad_3",323},
			{"keypad_4",324},
			{"keypad_5",325},
			{"keypad_6",326},
			{"keypad_7",327},
			{"keypad_8",328},
			{"keypad_9",329},
			{"f1",290},
			{"f2",291},
			{"f3",292},
			{"f4",293},
			{"f5",294},
			{"f6",295},
			{"f7",296},
			{"f8",297},
			{"f9",298},
			{"f10",299},
			{"f11",300},
			{"f12",301},
			{"f13",302},
			{"f14",303},
			{"f15",304},
			{"f16",305},
			{"f17",306},
			{"f18",307},
			{"f19",308},
			{"f20",309},
			{"f21",310},
			{"f22",311},
			{"f23",312},
			{"f24",313},
			{"f25",314}
		};
		std::unordered_map<int,int> keyPresses = {}; //The keys that were pressed or released this frame. 0 = nothing, 1 = pressed, 2 = released.
		int mousePresses[3] = {0}; // same as above
		glm::vec2 currentMousePos = glm::vec2(0,0);
		glm::vec2 prevMousePos = glm::vec2(0,0);
		glm::vec2 scroll = glm::vec2(0,0);
		bool cursorDisabled = false;

		auto Poll() -> void;
		auto GetKeyCode(const std::string& keyname) -> int;
		auto KeyPressed(int code) -> bool;
		auto KeyReleased(int code) -> bool;
		auto KeyDown(int code) -> bool;
		auto KeyPressed(const std::string& code) -> bool;
		auto KeyReleased(const std::string& code) -> bool;
		auto KeyDown(const std::string& code) -> bool;
		auto key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) -> void;
		auto mouse_button_callback(GLFWwindow* window, int button, int action, int mods) -> void;
		auto cursor_position_callback(GLFWwindow* window, double xpos, double ypos) -> void;
		auto scroll_callback(GLFWwindow* window, double xoffset, double yoffset) -> void;
		auto GetMousePosition() -> glm::vec2;
		auto GetMouseDelta() -> glm::vec2;
		auto MouseButtonDown(int button) -> bool;
		auto MouseButtonPressed(int button) -> bool;
		auto MouseButtonReleased(int button) -> bool;
		auto GetScroll2D() -> glm::vec2;
		auto GetScrollVertical() -> float;
		auto GetScrollHorizontal() -> float;
		auto HideCursor(bool disable) -> void;
		auto ShowCursor() -> void;
		auto RawMouseMotion(bool enabled) -> void;
		auto Init(GLFWwindow* w) -> void;
	};		
}

auto Vex::Input::Poll() -> void {
	for (auto& c : Vex::Input::keyPresses){
		c.second = 0;
	}
	Vex::Input::mousePresses[0] = 0;
	Vex::Input::mousePresses[1] = 0;
	Vex::Input::mousePresses[2] = 0;
	Vex::Input::prevMousePos = Vex::Input::currentMousePos;
	Vex::Input::scroll = glm::vec2(0,0);
	glfwPollEvents();
}

auto Vex::Input::GetKeyCode(const std::string& keyname_input) -> int {
	std::string keyname = boost::algorithm::to_lower_copy(keyname_input);
	if (auto it = Vex::Input::keycodes.find(keyname); it != Vex::Input::keycodes.end()){
		return it->second;
	} else {
		std::cout << "Vex Input Warning: " << keyname << " could not be found." << std::endl;
		return -1;
	}
}

auto Vex::Input::KeyPressed(int code) -> bool { 
	if(Vex::Input::keyPresses.find(code) == Vex::Input::keyPresses.end()){
		std::cout << "Vex Input Error: " << code << " is not a valid key code." << std::endl;
		return false;
	}
	return Vex::Input::keyPresses[code] == 1;
}
auto Vex::Input::KeyReleased(int code) -> bool {
	if(Vex::Input::keyPresses.find(code) == Vex::Input::keyPresses.end()){
		std::cout << "Vex Input Error: " << code << " is not a valid key code." << std::endl;
		return false;
	}
	return Vex::Input::keyPresses[code] == 2;
}
auto Vex::Input::KeyDown(int code) -> bool {
	return glfwGetKey(Vex::Input::window, code) == GLFW_PRESS;
}
auto Vex::Input::KeyPressed(const std::string& code) -> bool {
	int kcode = Vex::Input::GetKeyCode(code);
	if(Vex::Input::keyPresses.find(kcode) == Vex::Input::keyPresses.end()){
		std::cout << "Vex Input Error: " << code << " is not a valid key." << std::endl;
		return false;
	}
	return Vex::Input::keyPresses[kcode] == 1;
}
auto Vex::Input::KeyReleased(const std::string& code) -> bool {
	int kcode = Vex::Input::GetKeyCode(code);
	if(Vex::Input::keyPresses.find(kcode) == Vex::Input::keyPresses.end()){
		std::cout << "Vex Input Error: " << code << " is not a valid key." << std::endl;
		return false;	
	}
	return Vex::Input::keyPresses[kcode] == 2;
}
auto Vex::Input::KeyDown(const std::string& code) -> bool {
	return glfwGetKey(window, Vex::Input::GetKeyCode(code)) == GLFW_PRESS;
}

auto Vex::Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) -> void {
	if(action == GLFW_PRESS){
		Vex::Input::keyPresses[key] = 1;
	}else if(action == GLFW_RELEASE){
		Vex::Input::keyPresses[key] = 2;
	}
}
auto Vex::Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) -> void {
	if(action == GLFW_PRESS){
		Vex::Input::mousePresses[button] = 1;
	}else if(action == GLFW_RELEASE){
		Vex::Input::mousePresses[button] = 2;
	}
}
auto Vex::Input::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) -> void {
	Vex::Input::prevMousePos = Vex::Input::currentMousePos;
	Vex::Input::currentMousePos = glm::vec2(xpos, ypos);
}
auto Vex::Input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) -> void {
	Vex::Input::scroll = glm::vec2(xoffset, yoffset);
}

auto Vex::Input::GetMousePosition() -> glm::vec2 {
	if(Vex::Input::cursorDisabled == true){
		std::cout << "Vex Input Warning: Cursor is disabled, use GetMouseDelta() instead of GetMousePosition()." << std::endl;
		return glm::vec2(0,0);
	}else{
		return Vex::Input::currentMousePos;
	}	
}
auto Vex::Input::GetMouseDelta() -> glm::vec2 {
	return Vex::Input::currentMousePos - Vex::Input::prevMousePos;
}

auto Vex::Input::MouseButtonDown(int button) -> bool {
	if(button > -1 && button < 3){
		return glfwGetMouseButton(Vex::Input::window, button) == GLFW_PRESS;
	}else{
		std::cout << "Vex Input Error: There is no mouse button " << button << ". Valid mouse buttons are 0, 1, and 2." << std::endl;
		return false;
	}
}
auto Vex::Input::MouseButtonPressed(int button) -> bool {
	if(button > -1 && button < 3){
		return Vex::Input::mousePresses[button] == 1;
	}else{
		std::cout << "Vex Input Error: There is no mouse button " << button << ". Valid mouse buttons are 0, 1, and 2." << std::endl;
		return false;
	}
}
auto Vex::Input::MouseButtonReleased(int button) -> bool {
	if(button > -1 && button < 3){
		return Vex::Input::mousePresses[button] == 2;
	}else{
		std::cout << "Vex Input Error: There is no mouse button " << button << ". Valid mouse buttons are 0, 1, and 2." << std::endl;
		return false;
	}
}
auto Vex::Input::GetScroll2D() -> glm::vec2 {
	return Vex::Input::scroll;
}
auto Vex::Input::GetScrollVertical() -> float {
	return Vex::Input::scroll.y;
}
auto Vex::Input::GetScrollHorizontal() -> float {
	return Vex::Input::scroll.x;
}
auto Vex::Input::HideCursor(bool disable) -> void {
	if(disable){
		glfwSetInputMode(Vex::Input::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}else{
		glfwSetInputMode(Vex::Input::window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	Vex::Input::cursorDisabled = disable;
}
auto Vex::Input::ShowCursor() -> void {
	glfwSetInputMode(Vex::Input::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	Vex::Input::cursorDisabled = false;
}
auto Vex::Input::RawMouseMotion(bool enabled) -> void {
	if (glfwRawMouseMotionSupported()){
		glfwSetInputMode(Vex::Input::window, GLFW_RAW_MOUSE_MOTION, enabled);
	}else if(enabled){
		std::cout << "Vex Input Warning: Raw mouse motion is not supported on this device" << std::endl;
	}
}
auto Vex::Input::Init(GLFWwindow* w) -> void {
	Vex::Input::window = w;
	glfwSetKeyCallback(Vex::Input::window, Vex::Input::key_callback);
	glfwSetMouseButtonCallback(Vex::Input::window, Vex::Input::mouse_button_callback);
	glfwSetCursorPosCallback(Vex::Input::window, Vex::Input::cursor_position_callback);
	glfwSetScrollCallback(Vex::Input::window, Vex::Input::scroll_callback);
	for (auto& c : Vex::Input::keycodes){
		Vex::Input::keyPresses.insert(std::make_pair(c.second, 0));
	}
}

#endif
