import pyzta 

def test_load_zta():
    zta = pyzta.ZtaF()
    zta.load("test_data/SE")
    assert zta.data().has_background == False

if __name__ == "__main__":
    test_load_zta()
    print("All tests passed!")