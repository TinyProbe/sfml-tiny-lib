#pragma once
#ifndef COMMON_HPP
#define COMMON_HPP

#define WIDTH  1280
#define HEIGHT 720

#define MOVE_UNIT 5.f

// std library
#include <stdexcept>
#include <iostream>
#include <utility>
#include <cmath>

// third party
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

// dev
#include "Program.hpp"

// lib
#include "Animation.hpp"
#include "AutoSprite.hpp"
#include "FPSManager.hpp"
#include "KeyManager.hpp"
#include "SpriteAtlas.hpp"
#include "SpriteGenerator.hpp"
#include "WrapImage.hpp"
#include "WrapTexture.hpp"

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

// u64 getDigitLength(u64 const &n) {
//   return log10(n) + 1;
// }

#endif
