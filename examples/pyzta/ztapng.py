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

def print_frame_info(zta):
    print(f"Animation speed: {zta.data().info.speed}")
    print(f"Frame count: {zta.data().info.frame_count}")
    print(f"Has background: {zta.data().has_background}")
    print(f"Palette location: {zta.data().palette.location()}")
    print(f"Palette color count: {len(zta.data().palette.colors())}")

ATL_PATH = "example/objects/gsatlftn"
TEST_PATH = "example/objects/fdskajsd"

if __name__ == "__main__":
    # --------- load ZTA file ---------
    zta = ZtaF()
    try:
        zta.load(f"{ATL_PATH}/IDLE/SE")
    except Exception as e:
        print(f"Error loading ZTA file: {e}")
        exit(1)

    # print some info about the animation
    print_frame_info(zta)

    # --------- create pngs ---------

    output_frames_as_pngs(zta)
    
    # -------- save back to zta ---------
    os.makedirs("out/zta", exist_ok=True)
    try:
        zta.save(f"{TEST_PATH}/NE/SE_copy.zta", project_root="example/", palette_path=f"{TEST_PATH}/NE/stgeend.pal")
    except Exception as e:
        print(f"Error saving ZTA file: {e}")
        exit(1)

    # check if the file was saved correctly by loading it again
    zta_copy = ZtaF()
    try:
        zta_copy.load(f"{TEST_PATH}/NE/SE_copy.zta", 0, f"{TEST_PATH}/NE/stgeend.pal")
        print("ZTA file saved and loaded successfully!")
    except Exception as e:
        print(f"Error loading saved ZTA file: {e}")
        exit(1)
    
    # --------- assert that the original and loaded data are the same ---------
    print_frame_info(zta_copy)

    # ---------- output frames from the loaded copy to verify they are the same as the original pngs ---------
    output_frames_as_pngs(zta_copy, dir_name="out_copy")
