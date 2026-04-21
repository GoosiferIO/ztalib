// APE.CORE .NET Bindings
// by Eric Galvan (Goosifer.IO)
// https://github.com/openztcc/APE.Core
// Licensed under MIT (see LICENSE)
//
// Python bindings for APE.CORE
// Version 0.4.0

#include "../include/ZtaCore.h"

#define VERSION "0.4.0"

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT __attribute__((visibility("default")))
#endif

extern "C" {
    EXPORT ZtaCore* create_ape_instance() {
        return new ZtaCore();
    }

    EXPORT void destroy_ape_instance(ZtaCore* ape) {
        if (ape) {
            // Ensure OutputBuffer pointers are cleared before deletion
            for (int i = 0; i < ape->getFrameCount(); i++) {
                delete ape->apeBuffer()[i];  // Free each frame buffer
            }
            delete ape;
        }
    }

    EXPORT int load_image(ZtaCore* ape, const char* fileName, int colorProfile, const char* palName) {
        if (!ape) return 0;
        return ape->load(std::string(fileName), colorProfile, std::string(palName)) ? 1 : 0;
    }

    EXPORT int get_frame_count(ZtaCore* ape) {
        return (ape) ? ape->getFrameCount() : 0;
    }

    EXPORT OutputBuffer** get_frame_buffer(ZtaCore* ape) {
        if (!ape || ape->getFrameCount() == 0) return nullptr;
        return ape->apeBuffer();
    }

    EXPORT void free_frame_buffer(OutputBuffer** buffer) {
        if (buffer) {
            delete[] buffer;
        }
    }

    EXPORT Color* get_colors(ZtaCore* ape) {
        if (!ape || ape->getColors().empty()) 
            return nullptr;
        return ape->getColors().data();
    }

    EXPORT Frame* get_frames(ZtaCore* ape) {
        if (!ape || ape->getFrames().empty()) 
            return nullptr;
        return ape->getFrames().data();
    }

    EXPORT int frame_to_png(ZtaCore* ape, const char* fileName, int index) {
        if (!ape || index < 0 || index >= ape->getFrameCount()) return 0;
        return ape->exportToPNG(std::string(fileName), *ape->apeBuffer()[index]) ? 1 : 0;
    }

    EXPORT int validate_graphic_file(const char* fileName) {
        return ZtaCore::validateGraphicFile(std::string(fileName));
    }

    EXPORT int validate_palette_file(const char* fileName) {
        return ZtaCore::validatePaletteFile(std::string(fileName));
    }

    EXPORT int has_background_frame(const char* fileName) {
        return ZtaCore::hasBackgroundFrame(std::string(fileName));
    }

    EXPORT Header get_header(const char* fileName) {
        return ZtaCore::getHeader(std::string(fileName));
    }
}
