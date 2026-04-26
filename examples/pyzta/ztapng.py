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

def print_graphic_info(zta):
    print(f"Animation speed: {zta.data().info.speed}")
    print(f"Frame count: {zta.data().info.frame_count}")
    print(f"Has background: {zta.data().has_background}")
    print(f"Palette location: {zta.data().palette.location()}")
    print(f"Palette color count: {len(zta.data().palette.colors())}")

def print_buffer_info(zta, frame_range=None):
    # replace frames with range of frames if frame_range is provided
    if frame_range is not None:
        frames = zta.get_frame_buffer()[frame_range.start:frame_range.stop]
    for i, frame in enumerate(frames):
        print(f"Buffer Object {i}:")
        print(f"  Width: {frame.width}")
        print(f"  Height: {frame.height}")
        print(f"  Pixel data length: {len(frame.pixels)}")

def print_frame_info(zta, frame_range):
    for i in zta.data().frames[frame_range.start:frame_range.stop]:
        print(f"Frame {i}:")
        print(f"  frameSize: {i.frame_size}")
        print(f"  height: {i.height}")
        print(f"  width: {i.width}")
        print(f"  yOffset: {i.y_offset}")
        print(f"  xOffset: {i.x_offset}")
        print(f"  unk1: {i.unk1}")
        print(f"  unk2: {i.unk2}")
        print(f"  pixelSet count: {len(i.pixel_sets)}")

ATL_PATH = "example/objects/gsatlftn"
TEST_PATH = "F:\\Documents\\Github\\APE.Core\\examples\\pyzta\\example\\objects\\fdskajsd"
TEST_ROOT_PATH = "F:\\Documents\\Github\\APE.Core\\examples\\pyzta\\example"

# sanitize paths for Windows
ATL_PATH = ATL_PATH.replace("/", "\\")
TEST_PATH = TEST_PATH.replace("/", "\\")
TEST_ROOT_PATH = TEST_ROOT_PATH.replace("/", "\\")

if __name__ == "__main__":
    # --------- load ZTA file ---------
    zta = ZtaF()
    try:
        zta.load(f"{ATL_PATH}/IDLE/SE")
    except Exception as e:
        print(f"Error loading ZTA file: {e}")
        exit(1)

    # print some info about the animation
    print("################## ZTA INFO ##################")
    print_graphic_info(zta)
    print_buffer_info(zta, frame_range=range(0, 3)) # print info for first 3 frames
    print_frame_info(zta, frame_range=range(0, 3)) # print

    # --------- create pngs ---------

    output_frames_as_pngs(zta)
    
    # -------- save back to zta ---------
    os.makedirs("out/zta", exist_ok=True)
    try:
        zta.save(f"{TEST_PATH}/IDLE/SE", project_root=f"{TEST_ROOT_PATH}/", palette_path=f"{TEST_PATH}/IDLE/SE.pal")
    except Exception as e:
        print(f"Error saving ZTA file: {e}")
        exit(1)

    # check if the file was saved correctly by loading it again
    zta_copy = ZtaF()
    try:
        zta_copy.load(f"{TEST_PATH}/IDLE/SE")
        print("ZTA file saved and loaded successfully!")
    except Exception as e:
        print(f"Error loading saved ZTA file: {e}")
        exit(1)
    
    # --------- assert that the original and loaded data are the same ---------
    print("################## ZTA COPY INFO ##################")
    print_graphic_info(zta_copy)
    print_buffer_info(zta_copy, frame_range=range(0, 3)) # print info for first 3 frames
    print_frame_info(zta_copy, frame_range=range(0, 3)) # print info for first 3 frames

    # ---------- output frames from the loaded copy to verify they are the same as the original pngs ---------
    output_frames_as_pngs(zta_copy, dir_name="out_copy")
