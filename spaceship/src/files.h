#pragma once
#ifndef __SOURCE_FILES_H_
#define __SOURCE_FILES_H_

#define ASSETS_DIR "assets/"
#define AUDIO_DIR  ASSETS_DIR "audio/"
#define FONTS_DIR  ASSETS_DIR "fonts/"
#define IMAGES_DIR ASSETS_DIR "images/"

#define GET_ASSET_PATH(asset_filename) (ASSETS_DIR asset_filename)
#define GET_AUDIO_PATH(audio_filename) (AUDIO_DIR audio_filename)
#define GET_FONT_PATH(font_filename)   (FONTS_DIR font_filename)
#define GET_IMAGE_PATH(image_filename) (IMAGES_DIR image_filename)

#endif // __SOURCE_FILES_H_