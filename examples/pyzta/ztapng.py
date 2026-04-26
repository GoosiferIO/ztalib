from pyzta import ZtaF
from PIL import Image

if __name__ == "__main__":
    zta = ZtaF()
    zta.load("SE", 0, "stgeend.pal")
    buffer = zta.get_frame_buffer()

    print(f"Frame buffer size: {len(buffer)}")

    # output frames as pngs
    for i, frame in enumerate(buffer):
        img = Image.frombytes("RGBA", (frame.width, frame.height), frame.pixels)
        img.save(f"frame_{i}.png")
    
