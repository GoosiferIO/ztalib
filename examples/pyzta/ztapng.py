import os
from pyzta import ZtaF
from PIL import Image

if __name__ == "__main__":
    # --------- load ZTA file ---------
    zta = ZtaF()
    try:
        zta.load("SE", 0, "stgeend.zta")
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
    
