import cv2
import numpy as np


class steganography_lsb:

    def encode(image: cv2.Mat, message: str) -> cv2.Mat:
    
        def bit_at(val: int, shift: int) -> int:
            return (val >> shift) & 1
        
        h, w, _ = image.shape
        
        assert len(message) + 64 <= h * w * 3, "message is to big to hide in the image"

        mask = np.zeros((h, w, 3), np.uint8)
        mask[:] = (254, 254, 254)
        
        res = cv2.bitwise_and(image, mask)
        
        i = j = k = 0
        # hiding the message length
        n = len(message)

        for shift in range(64):
            res[i][j][k] |= bit_at(n, shift)
            i, j, k = steganography_lsb.__update_indices(i, j, k, h, w)

        # hiding the message        
        for ch in message:
            ascii_code = ord(ch)

            for shift in range(8):
                res[i][j][k] |= bit_at(ascii_code, shift)
                i, j, k = steganography_lsb.__update_indices(i, j, k, h, w)
                
        return res

    def decode(image: cv2.Mat) -> str:
        
        def current_lsb(shift: int) -> int:
            return (image[i][j][k] & 1) << shift

        h, w, _ = image.shape
        i = j = k = 0        

        n = 0
        
        for shift in range(64):
            n |= current_lsb(shift)
            i, j, k = steganography_lsb.__update_indices(i, j, k, h, w)

        res = ''
        
        for _ in range(n):
            ascii_code = 0
            for shift in range(8):
                ascii_code |= current_lsb(shift)
                i, j, k = steganography_lsb.__update_indices(i, j, k, h, w)
            res += chr(ascii_code)
            
        return res
    
    @staticmethod
    def __update_indices(i, j, k, h, w) -> None:
        i += 1
        if i == h:
            i = 0
            j += 1
        if j == w:
            j = 0
            k += 1
        return (i, j, k)

if __name__ == '__main__':

    image = cv2.imread("container.jpg")
    message = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."

    steg_image = steganography_lsb.encode(image, message)
    hidden_message = steganography_lsb.decode(steg_image)

    assert hidden_message == message, "the steganography operation didnt work"
    
    while True:
        cv2.imshow("image without hidden text", image)
        cv2.imshow("image with hidden text", steg_image)
        cv2.waitKey(1)