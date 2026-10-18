import cv2
import numpy as np
import requests

# ESP32-CAM stream URL
url = "http://192.168.29.225/"

print("Connecting to ESP32-CAM Stream...")

try:
    # Stream = True keeps the connection open for chunk reading
    response = requests.get(url, stream=True, timeout=10)
    if response.status_code != 200:
        print(f"Server returned status code: {response.status_code}")
        exit()
    print("Stream connected! Press 'q' on the video window to exit.")
except Exception as e:
    print(f"Failed to connect: {e}")
    print("Press the RST button on the ESP32-CAM and try again.")
    exit()

bytes_buffer = b""

for chunk in response.iter_content(chunk_size=4096):
    bytes_buffer += chunk
    
    # Locate JPEG frame start (\xff\xd8) and end (\xff\xd9)
    a = bytes_buffer.find(b'\xff\xd8')
    b = bytes_buffer.find(b'\xff\xd9')

    if a != -1 and b != -1:
        jpg = bytes_buffer[a:b+2]
        bytes_buffer = bytes_buffer[b+2:]

        # Decode JPEG bytes to OpenCV frame
        img = cv2.imdecode(np.frombuffer(jpg, dtype=np.uint8), cv2.IMREAD_COLOR)

        if img is not None:
            cv2.imshow("ESP32-CAM Live Stream", img)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cv2.destroyAllWindows()