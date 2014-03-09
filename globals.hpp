#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include "Graphics/RenderManager.hpp"
#include "ResourceManager.hpp"
#include "GUI/Console.hpp"

#define PI 3.14159265359 

extern Console *global_con;
extern InputManager *im;
extern ResourceManager resman;
extern RenderManager rendman;

extern int width;
extern int height;

#endif
