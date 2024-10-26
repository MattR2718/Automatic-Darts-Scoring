import cv2 as cv


#Change to camera
stream = cv.VideoCapture(0)


sBackSub = cv.createBackgroundSubtractorMOG2()

while True:
    ret, frame = stream.read()
    if frame is None:
        break

    fgMask = sBackSub.apply(frame)
    fgMask_th = cv.threshold(fgMask, 200,255, cv.THRESH_BINARY)[1]

    #original stream
    cv.imshow('Stream',frame)

    #stream with background substracted
    cv.imshow('Mask',fgMask)

    #background substracted with lighter colours set to white
    cv.imshow("Threshold mask",fgMask_th)


    #quit stream
    keyboard = cv.waitKey(30)
    if (keyboard == 'q'):
        break