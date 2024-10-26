import cv2 as cv


#Change to camera
stream = cv.VideoCapture(0)


sBackSub = cv.createBackgroundSubtractorKNN()
temp,lastFrame = stream.read()
lastFrameG = cv.cvtColor(lastFrame,cv.COLOR_BGR2GRAY)
lastFrame_contour = None
temp,lastFrame_mask = stream.read()

 
maxArea = 0
maxContours = []
maxAreas = []
maxFrames = []
maxFramesM = []
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

        if area > 900:
              
            maxContours.append(contour)
            maxAreas.append(area)
                
            maxFrames.append(frame)
            maxFramesM.append(fgMask)
            
            org = (50,50)
            font = cv.FONT_HERSHEY_SIMPLEX
            fontScale = 1
            color = (0,0,0)
            thickness = 2
            #lastFrame_mask = cv.putText(lastFrame_mask, Bottom,org, font, fontScale, color, thickness, cv.LINE_AA)
        
    
    noMovement = True
    for contour in contours:
        area = cv.contourArea(contour)
        if area > 900:
            noMovement = False
    
    if noMovement:
         
        cv.circle(lastFrame, Bottom, 8, (0,0, 255), -1)
        cv.drawContours(frame,closestContour,-1,(0,255,0),2)
        cv.drawContours(lastFrame,closestContour,-1,(0,255,0),2)

         
        cv.imshow("Last Detected Frame",lastFrame)
        cv.imshow("Last Detected Frame Mask",lastFrame_mask)
        maxArea = 0
        maxContours = []
        maxAreas = []
        maxFramesM = []
        maxFrames = []
    #quit stream

    cv.imshow('Stream',frame)
    if (cv.waitKey(1)& 0xFF == ord('q')):
        break




stream.release()
cv.destroyAllWindows()
print("STREAM ENDED")


 