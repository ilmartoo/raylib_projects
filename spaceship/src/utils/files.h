#pragma once
#ifndef FILES_H
#define FILES_H

// ----------------------------------------------------------------------------
// ---- Assets Folders --------------------------------------------------------
// ----------------------------------------------------------------------------

#define ASSETS_DIR "assets/"
#define AUDIO_DIR  ASSETS_DIR "audio/"
#define FONTS_DIR  ASSETS_DIR "fonts/"
#define IMAGES_DIR ASSETS_DIR "images/"

#define path_asset(asset_filename) (ASSETS_DIR asset_filename)
#define path_audio(audio_filename) (AUDIO_DIR audio_filename)
#define path_font(font_filename)   (FONTS_DIR font_filename)
#define path_image(image_filename) (IMAGES_DIR image_filename)

// ----------------------------------------------------------------------------
// ---- Saves Folders ---------------------------------------------------------
// ----------------------------------------------------------------------------

#define PROFILES_DIR  "profiles/"
#define UNIVERSES_DIR "universes/"

#endif // FILES_H