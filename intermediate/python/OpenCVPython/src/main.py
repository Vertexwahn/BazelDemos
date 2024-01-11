import cv2 as cv
import numpy as np
#import matplotlib.pyplot as plt

if __name__ == "__main__":
    img = cv.imread('data/52hz_shot-b_web-high.jpg', 0)
    print(img)
    cv.imshow('image', img)
    cv.waitKey(0)
    cv.destroyAllWindows()
