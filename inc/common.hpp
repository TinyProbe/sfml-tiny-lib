#pragma once
#ifndef COMMON_HPP
#define COMMON_HPP

#define WIDTH  1280
#define HEIGHT 720

#define MOVE_UNIT 5.f

#include <stdexcept>
#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

// #include "ScrManager.hpp"
#include "FPSManager.hpp"
#include "KeyManager.hpp"
#include "Animation.hpp"
// #include "ImageTexture.hpp"
// #include "SpriteTexture.hpp"

using i8 = char;
using i16 = short;
using i32 = int;
using i64 = long long;
using isize = long;
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;
using usize = unsigned long;
using f32 = float;
using f64 = double;

#endif
