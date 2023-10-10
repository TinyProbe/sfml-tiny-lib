#ifndef SFML_COMMON_H_
#define SFML_COMMON_H_

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
#include <dev/Program.h>
#include <dev/object/Object.h>
#include <dev/object/Visible.h>
#include <dev/object/Hearable.h>
#include <dev/object/Collidable.h>

// lib
#include <lib/Animation.h>
#include <lib/FPSManager.h>
#include <lib/KeyManager.h>
#include <lib/MouseManager.h>
#include <lib/SpriteGenerator.h>
#include <lib/WrapImage.h>
#include <lib/WrapTexture.h>
#include <lib/WrapSoundBuffer.h>

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

#endif // SFML_COMMON_H_
