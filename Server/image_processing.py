import cv2 as cv


#Change to camera
stream = cv.VideoCapture(0)


sBackSub = cv.createBackgroundSubtractorMOG2()

while True:
    ret, frame = stream.read()
    if frame is None:
        break


    #COULD CROP FRAME BEFORE APPLYING MASK
    # ===================================    

    fgMask = sBackSub.apply(frame)
    fgMask_th = cv.threshold(fgMask, 230,255, cv.THRESH_BINARY)[1]

    #original stream
    cv.imshow('Stream',frame)

    #stream with background substracted
    #cv.imshow('Mask',fgMask)

    #background substracted with lighter colours set to white
    cv.imshow("Threshold mask",fgMask_th)

    #find contours
    contours, hierarchy = cv.findContours(fgMask_th, cv.RETR_EXTERNAL,cv.CHAIN_APPROX_SIMPLE)

    for contour in contours:
        #calculates area of contours, and only outputs contours
        #greater than threshold to remove redundancies and noise
        area = cv.contourArea(contour)
        #AREA THRESHOLD PART OF CONFIG, DEPENDANT ON HOW FAR AWAY
        #DART IS FROM CAMERA

        if area > 1000:
            print(contour)
            x,y,w,h = cv.boundingRect(contour)
            cv.rectangle(fgMask_th,(x,y),(x+w,y+h),(0,255,0),2)
    #quit stream
    keyboard = cv.waitKey(30)
    if (keyboard == 'q'):
        break