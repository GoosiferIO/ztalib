import os
from pyzta import ZtaF
from PIL import Image

def output_frames_as_pngs(zta, dir_name="out"):
    buffer = zta.get_frame_buffer()

    # create out dir
    os.makedirs(dir_name, exist_ok=True)

    # output frames as pngs
    for i, frame in enumerate(buffer):
        img = Image.frombytes("RGBA", (frame.width, frame.height), frame.pixels)
        img.save(f"{dir_name}/frame_{i}.png")

if __name__ == "__main__":
    # --------- load ZTA file ---------
    zta = ZtaF()
    try:
        zta.load("example/objects/gsatlftn/IDLE/SE")
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

    output_frames_as_pngs(zta)
    
    # -------- save back to zta ---------
    os.makedirs("out/zta", exist_ok=True)
    try:
        zta.save("example/objects/fdskajsd/NE/SE_copy.zta")
    except Exception as e:
        print(f"Error saving ZTA file: {e}")
        exit(1)

    # check if the file was saved correctly by loading it again
    zta_copy = ZtaF()
    try:
        zta_copy.load("example/objects/fdskajsd/NE/SE_copy.zta", 0, "stgeend.pal")
        print("ZTA file saved and loaded successfully!")
    except Exception as e:
        print(f"Error loading saved ZTA file: {e}")
        exit(1)
    
    # --------- assert that the original and loaded data are the same ---------
    assert zta.data().info.speed == zta_copy.data().info.speed, "Animation speed does not match"
    print(f"------ Original speed: {zta.data().info.speed}")
    print(f"------ Loaded speed: {zta_copy.data().info.speed}")
    assert zta.data().info.frame_count == zta_copy.data().info.frame_count, "Frame count does not match"
    print(f"------ Original frame count: {zta.data().info.frame_count}")
    print(f"------ Loaded frame count: {zta_copy.data().info.frame_count}")
    assert zta.data().has_background == zta_copy.data().has_background, "Background flag does not match"
    print(f"------ Original has background: {zta.data().has_background}")
    print(f"------ Loaded has background: {zta_copy.data().has_background}")
    assert zta.data().palette.location() == zta_copy.data().palette.location(), "Palette location does not match"
    print(f"------ Original palette location: {zta.data().palette.location()}")
    print(f"------ Loaded palette location: {zta_copy.data().palette.location()}")
    assert len(zta.data().palette.colors()) == len(zta_copy.data().palette.colors()), "Palette color count does not match"
    print(f"------ Original palette color count: {len(zta.data().palette.colors())}")
    print(f"------ Loaded palette color count: {len(zta_copy.data().palette.colors())}")

    print("All assertions passed! The original and loaded ZTA data are the same.")

    # ---------- output frames from the loaded copy to verify they are the same as the original pngs ---------
    output_frames_as_pngs(zta_copy, dir_name="out_copy")
