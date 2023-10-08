#ifndef COMMON_HPP_
#define COMMON_HPP_

// std library
#include <stdexcept>
#include <iostream>
#include <utility>
#include <cmath>

// third party
#include <SFML/Graphics.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

// dev
#include "dev/Program.hpp"
#include "dev/object/Background.hpp"

// lib
#include "lib/Animation.hpp"
#include "lib/FPSManager.hpp"
#include "lib/KeyManager.hpp"
#include "lib/MouseManager.hpp"
#include "lib/SpriteGenerator.hpp"
#include "lib/WrapImage.hpp"
#include "lib/WrapTexture.hpp"
#include "lib/WrapSoundBuffer.hpp"

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

static constexpr usize kWidth = 1280;
static constexpr usize kHeight = 720;
static constexpr f32 kMoveUnit = 5.0f;

// u64 getDigitLength(u64 const &n) {
//   return log10(n) + 1;
// }

#endif
