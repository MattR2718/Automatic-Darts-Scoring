import cv2 as cv


stream = cv.VideoCapture(0)

sBackSub = cv.createBackgroundSubtractorMOG2()

while True:
    ret, frame = stream.read()
    if frame is None:
        break

    fgMask = sBackSub.apply(frame)

    cv.imshow('Stream',frame)
    cv.imshow('Mask',fgMask)


    #quit stream
    keyboard = cv.waitKey(30)
    if (keyboard == 'q'):
        break