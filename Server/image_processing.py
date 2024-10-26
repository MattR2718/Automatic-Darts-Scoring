import cv2 as cv


#Change to camera
stream = cv.VideoCapture(0)

 

counter = 0
config_coord = []
x_factor = 1
y_factor = 1

def get_mouse_coordinates(event, x, y, flags, param):
    global counter
    global config_coord
    if event == cv.EVENT_LBUTTONDOWN:
        config_coord.append((x,y))
        #print(f"X={x} Y={y}")
        counter = counter + 1

def config():
    global counter
    global x_factor
    global y_factor
    cv.namedWindow("Darts")
    cv.setMouseCallback("Darts", get_mouse_coordinates)

    # Loop until user clicks 4 times
    while counter < 5:
        ret, frame = stream.read()
        if not ret:
            break

        cv.imshow("Darts", frame)

        if cv.waitKey(1) & 0xFF == ord('q'):
            break
    
    #stream.release()
    cv.destroyAllWindows()
    print(config_coord)

    x_factor = 2/(config_coord[0][0]-config_coord[1][0])
    y_factor = 2/(config_coord[3][1]-config_coord[2][1])


 





    


def DartLocation():
    global x_factor
    global y_factor
    sBackSub = cv.createBackgroundSubtractorMOG2()
    temp,lastFrame = stream.read()
    lastFrame_contour = None
    temp,lastFrame_mask = stream.read()
    maxArea = 0
    x_bot = 0
    y_bot = 0
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
            #print(contour)
            if area > 900:
                if (area > maxArea):
                    maxArea = area
                    lastFrame = frame.copy()
                    lastFrame_mask = fgMask.copy()
                    lastFrame_contour = contour
                    Bottom = tuple(lastFrame_contour[lastFrame_contour[:, :, 1].argmax()][0])
                    x_bot = int(Bottom[0])
                    y_bot = int(Bottom[1])
                    cv.circle(lastFrame, Bottom, 8, (0,0, 255), -1)
                    cv.drawContours(frame,contour,-1,(0,255,0),2)
                    #cv.rectangle(frame,(x,y),(x+w,y+h),(0,255,0),5)
                    cv.drawContours(lastFrame,contour,-1,(0,255,0),2)
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
             
            text = f"X: +{'{0:.3f}'.format((x_bot*x_factor)-1)}+, Y: +{'{0:.3f}'.format((y_bot*y_factor)-1)}"
            org = (50, 50)   
            font = cv.FONT_HERSHEY_SIMPLEX  
            fontScale = 0.5 
            color = (255,255, 255)   
            thickness = 2  


             
            cv.putText(lastFrame, text, org, font, fontScale, color, thickness, cv.LINE_AA)


            cv.imshow("Last Detected Frame",lastFrame)
            cv.imshow("Last Detected Frame Mask",lastFrame_mask)
            maxArea = 0
        #quit stream

        cv.imshow('Stream',frame)
        if (cv.waitKey(1)& 0xFF == ord('q')):
            break


config()
DartLocation()