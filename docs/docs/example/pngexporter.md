# Example - ZTA File to PNG Exporter

ztalib is only designed to parse Zoo Tycoon 1 animation graphics and palette files in-memory. However, you can use the parsed data to export the frames as PNG images. By taking advantage of the buffer data extracted from the ZTA files, you can use popular libraries like `stb_image_write` (C++) or `pillow` (Python) to save the frames as PNG files. Below is a working example of how to achieve this.

- Download `stb_image_write.h` from https://github.com/nothings/stb  
- See the code in the repository: [examples](https://github.com/GoosiferIO/ztalib/tree/main/examples)

=== "C++"
    ```c++
    #define STB_IMAGE_WRITE_IMPLEMENTATION
    #include "ztalib/ZtaF.h"
    #include "include/stb_image_write.h"
    #include <string>
    #include <filesystem>

    int main()
    {
        // ---------------------- LOAD ZTA FILE
        std::cout << "ztalib test - version " << ZTA_VERSION << std::endl;
        ZtaF zta;
        std::shared_ptr<ZtaData> data = std::make_shared<ZtaData>();

        std::cout << "Loading ZTA file..." << std::endl;

        try {
            data = zta.load("SE", 0);
            if(!data) {
                std::cerr << "Failed to load ZTA file." << std::endl;
                return 1;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error loading ZTA file: " << e.what() << std::endl;
            return 1;
        }

        // ---------------------- INFO

        std::cout << "Speed: " << data->info.speed << std::endl;
        std::cout << "Frame Count: " << data->info.frameCount << std::endl;
        std::cout << "Has Background Frame: " << (data->hasBackground ? "Yes" : "No") << std::endl;
        std::cout << "Palette Location: " << data->palette->location() << std::endl;
        std::cout << "Palette Color Count: " << data->palette->numColors() << std::endl;

        // ---------------------- FRAMES TO PNG

        std::vector<ZtaFrameBufferObject> buffer = zta.getFrameBuffer();
        std::cout << "Number of frames in buffer: " << buffer.size() << std::endl;

        if (!buffer.empty() && !buffer[0].pixels.data()) {
            std::cerr << "No pixels to write" << std::endl;
            return 1;
        }

        if (!std::filesystem::exists("./out")) {
            std::filesystem::create_directory("./out");
        }

        for (size_t i = 0; i < buffer.size(); ++i) {
            std::cout << "Frame " << i << ": " 
                    << "Width: " << buffer[i].width << ", "
                    << "Height: " << buffer[i].height << ", "
                    << "Channels: " << buffer[i].channels << std::endl;

            std::string fileName = "frame_" + std::to_string(i) + ".png";
            std::string filePath = "./out/" + fileName;

            if (!stbi_write_png(filePath.c_str(), buffer[i].width, buffer[i].height, buffer[i].channels, buffer[i].pixels.data(), 0)) {
                std::cerr << "Failed to write image" << std::endl;
                return 1;
            } else {
                std::cout << "Wrote image" << std::endl;
            }

        }
        return 0;
    }
    ```
=== "Python"
    ```Python   
    import os
    from pyzta import ZtaF
    from PIL import Image

    if __name__ == "__main__":
    # --------- load ZTA file ---------
    zta = ZtaF()
    try:
        zta.load("SE", 0, "stgeend.pal")
    except Exception as e:
        print(f"Error loading ZTA file: {e}")
        exit(1)

    # print some info about the animation
    print(f"Animation speed: {zta.data().info.speed}")
    print(f"Frame count: {zta.data().info.frame_count}")
    print(f"Has background: {zta.data().has_background}")
    print(f"Palette location: {zta.data().palette.location()}")
    print(f"Palette color count: {len(zta.data().palette.colors())}")

    # --------- create pngs ---------

    buffer = zta.get_frame_buffer()

    # create out dir
    os.makedirs("out", exist_ok=True)

    # output frames as pngs
    for i, frame in enumerate(buffer):
        img = Image.frombytes("RGBA", (frame.width, frame.height), frame.pixels)
        img.save(f"out/frame_{i}.png")
    ```