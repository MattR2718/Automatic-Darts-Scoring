import cv2 as cv


#Change to camera
stream = cv.VideoCapture(0)


sBackSub = cv.createBackgroundSubtractorMOG2()
temp,lastFrame = stream.read()
lastFrame_contour = None
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

        if area > 750:
            lastFrame = frame.copy()
            lastFrame_mask = fgMask.copy()
            lastFrame_contour = contour
            print("contour",contour)
            x,y,w,h = cv.boundingRect(contour)
            cv.rectangle(frame,(x,y),(x+w,y+h),(0,255,0),2)
            cv.rectangle(lastFrame,(x,y),(x+w,y+h),(0,255,0),2)
        
    
    noMovement = True
    for contour in contours:
        area = cv.contourArea(contour)
        if area > 750:
            noMovement = False
    
    if noMovement:
        cv.imshow("Last Detected Frame",lastFrame)
    #quit stream

    cv.imshow('Stream',frame)
    if (cv.waitKey(1)& 0xFF == ord('q')):
        break

stream.release()
cv.destroyAllWindows()
print("STREAM ENDED")


 