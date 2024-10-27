import cv2 as cv
import time

class Detection:
    def __init__(self,server):
        self.server = server
        self.stream = cv.VideoCapture(0)

        self.counter = 0
        self.config_coord = []
        self.x_factor = 1
        self.x_start = 0
        self.y_factor = 1
        self.y_start = 0
        self.x = 0
        self.y = 0 

        self.config()
        self.DartLocation()

    def get_mouse_coordinates(self,event, x, y, flags, param):
        #global counter
        #global config_coord
        if event == cv.EVENT_LBUTTONDOWN:
            self.config_coord.append((x,y))
            #print(f"X={x} Y={y}")
            self.counter = self.counter + 1

    def config(self):
        #global counter
        #global x_factor
        #global y_factor
        #global x_start
        #global y_start
        cv.namedWindow("Darts")
        cv.setMouseCallback("Darts", self.get_mouse_coordinates)

        # Loop until user clicks 4 times
        while self.counter < 5:
            ret, frame = self.stream.read()
            if not ret:
                break

            cv.imshow("Darts", frame)

            if cv.waitKey(1) & 0xFF == ord('q'):
                break
        
        #stream.release()
        cv.destroyAllWindows()
        print(self.config_coord)
        bull = self.config_coord[4]
        if (self.config_coord[1][0]-bull[0])>(bull[0]-self.config_coord[3][0]):
            self.config_coord[3] = (bull[0]-(self.config_coord[1][0]-bull[0]),bull[1])
            self.config_coord[1] = (self.config_coord[1][0],bull[1])
        else:
            self.config_coord[1] = (bull[0]+(self.config_coord[3][0]+bull[0]),bull[1])
            self.config_coord[3] = (self.config_coord[3][0],bull[1])
        
        if (self.config_coord[0][1]-bull[1])>(bull[1]-self.config_coord[2][1]):
            self.config_coord[2] = (bull[0],bull[1]-(self.config_coord[2][1]-bull[1]))
            self.config_coord[0] = (bull[0],self.config_coord[0][1])
        else:
            self.config_coord[0] = (bull[0],bull[1]+(self.config_coord[0][1]+bull[1]))
            self.config_coord[2] = (bull[0],self.config_coord[2][1])
        self.x_start = self.config_coord[1][0]
        self.y_start = self.config_coord[2][1]
        self.x_factor = 2/(self.config_coord[1][0]-self.config_coord[3][0])
        self.y_factor = 2/(self.config_coord[0][1]-self.config_coord[2][1])
        print(self.config_coord)

        while True:
            ret, frame = self.stream.read()
            if not ret:
                break
            
            for (x,y) in self.config_coord:
                cv.circle(frame,(x,y), radius = 5, color = (0,0,255),thickness=3)
            cv.imshow("Darts", frame)

            if cv.waitKey(1) & 0xFF == ord('q'):
                break

    def DartLocation(self):
        #global x_factor
        #global y_factor
        #global x_start
        #global y_start
        sBackSub = cv.createBackgroundSubtractorMOG2()
        temp,lastFrame = self.stream.read()
        lastFrame_contour = None
        temp,lastFrame_mask = self.stream.read()
        maxArea = 0
        self.x_bot = 0
        self.y_bot = 0
        

        while True:
            ret, frame = self.stream.read()
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


            #time.sleep(7) # Wait 7 seconds before throwing

            #find contours
            contours, hierarchy = cv.findContours(fgMask_th, cv.RETR_EXTERNAL,cv.CHAIN_APPROX_SIMPLE)

            
            for contour in contours:
                #calculates area of contours, and only outputs contours
                #greater than threshold to remove redundancies and noise
                area = cv.contourArea(contour)
                #AREA THRESHOLD PART OF CONFIG, DEPENDANT ON HOW FAR AWAY
                #DART IS FROM CAMERA
                #print(contour)
                if area > 100:
                    if (area > maxArea):
                        maxArea = area
                        lastFrame = frame.copy()
                        lastFrame_mask = fgMask.copy()
                        lastFrame_contour = contour
                        Bottom = tuple(lastFrame_contour[lastFrame_contour[:, :, 1].argmax()][0])
                        self.x_bot = int(Bottom[0])
                        self.y_bot = int(Bottom[1])
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
                if area > 100:
                    noMovement = False
            
            if noMovement:        
                #global x
                #global y
                self.x = '{0:.3f}'.format(((self.x_bot-self.x_start)*self.x_factor)-1)
                self.y = ((self.y_bot-self.y_start)*self.y_factor)-1
                self.y = self.y * (-1)
                self.y  ='{0:.3f}'.format(self.y)
                #print(self.x,self.y)
                text = f"X: +{self.x}+, Y: +{self.y}"
                org = (50, 50)   
                font = cv.FONT_HERSHEY_SIMPLEX  
                fontScale = 0.5 
                color = (255,255, 255)   
                thickness = 2  
                
                cv.putText(lastFrame, text, org, font, fontScale, color, thickness, cv.LINE_AA)

                cv.imshow("Last Detected Frame",lastFrame)
                cv.imshow("Last Detected Frame Mask",lastFrame_mask)
                maxArea = 0
                #self.server.send_coords(self.x,self.y)
                if self.server.is_connected():
                    self.server.send_doubles(self.x, self.y)
            #quit stream



            cv.imshow('Stream',frame)
            if (cv.waitKey(1)& 0xFF == ord('q')):
                cv.destroyAllWindows()
                self.server.stop()
                return
        

 

