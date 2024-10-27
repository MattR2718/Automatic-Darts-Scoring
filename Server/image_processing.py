import cv2 as cv
import time
import numpy as np

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
        self.matrix = None

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
        cv.namedWindow("Callibrate corners")
        cv.setMouseCallback("Callibrate corners", self.get_mouse_coordinates)

        while self.counter < 4:
            ret, frame = self.stream.read()
            if not ret:
                break

            cv.imshow("Callibrate corners",frame)

            if cv.waitKey(1) & 0xFF == ord('q'):
                break
        
        # Loop until user clicks 4 times
        #while self.counter < 5:
        #    ret, frame = self.stream.read()
        #    if not ret:
        #        break
        #
        #    cv.imshow("Darts", frame)
        #
        #    if cv.waitKey(1) & 0xFF == ord('q'):
        #        break
        
        #stream.release()
        cv.destroyAllWindows()

        top_left = list(self.config_coord[0])
        top_right = list(self.config_coord[1])
        bottom_left = list(self.config_coord[2])
        bottom_right = list(self.config_coord[3])

        src_pts = np.float32([top_left,top_right,bottom_left,bottom_right])

        print(self.config_coord)

        width = 460
        height =460

        des_pts = np.float32([[0,0],[width,0],[0,height],[width,height]])
        self.matrix = cv.getPerspectiveTransform(src_pts,des_pts)
        #bull = self.config_coord[4]
        #if (self.config_coord[1][0]-bull[0])>(bull[0]-self.config_coord[3][0]):
        #    self.config_coord[3] = (bull[0]-(self.config_coord[1][0]-bull[0]),bull[1])
        #    self.config_coord[1] = (self.config_coord[1][0],bull[1])
        #else:
        #    self.config_coord[1] = (bull[0]+(self.config_coord[3][0]+bull[0]),bull[1])
        #    self.config_coord[3] = (self.config_coord[3][0],bull[1])
        #
        #if (self.config_coord[0][1]-bull[1])>(bull[1]-self.config_coord[2][1]):
        #    self.config_coord[2] = (bull[0],bull[1]+(self.config_coord[2][1]-bull[1]))
        #    self.config_coord[0] = (bull[0],self.config_coord[0][1])
        #else:
        #    self.config_coord[0] = (bull[0],bull[1]-(self.config_coord[0][1]-bull[1]))
        #    self.config_coord[2] = (bull[0],self.config_coord[2][1])
        #self.x_start = self.config_coord[3][0]
        #self.y_start = self.config_coord[0][1]
        #self.x_factor = 2/(self.config_coord[1][0]-self.config_coord[3][0])
        #self.y_factor = 2/(self.config_coord[2][1]-self.config_coord[0][1])
        print(self.config_coord)

        cv.namedWindow("Callibrate sides")
        cv.setMouseCallback("Callibrate sides", self.get_mouse_coordinates)
        self.counter = 0
        while self.counter < 5:
            ret, frame = self.stream.read()
            if not ret:
                break
            
            frame_p = cv.warpPerspective(frame,self.matrix,(700,500))
            for (x,y) in self.config_coord:
                cv.circle(frame,(x,y), radius = 5, color = (0,0,255),thickness=3)
            cv.imshow("Callibrate sides", frame)
            cv.imshow("Callibrate sides",frame_p)

            if cv.waitKey(1) & 0xFF == ord('q'):
                break
        
        top = self.config_coord[4]
        right = self.config_coord[5]
        bottom = self.config_coord[6]
        left = self.config_coord[7]
        bull = self.config_coord[8]

        if (right[0]-bull[0])>(bull[0]-left[0]):
            self.config_coord[7] = (bull[0]-(self.config_coord[5][0]-bull[0]),bull[1])
            self.config_coord[5] = (self.config_coord[5][0],bull[1])
        else:
            self.config_coord[5] = (bull[0]+(bull[0]-self.config_coord[7][0]),bull[1])
            self.config_coord[7] = (self.config_coord[7][0],bull[1])
        
        if (bull[1]-top[1])>(bottom[1]-bull[1]):
            self.config_coord[6] = (bull[0],bull[1]+(bull[1]-self.config_coord[4][1]))
            self.config_coord[4] = (bull[0],self.config_coord[4][1])
        else:
            self.config_coord[4] = (bull[0],bull[1]-(self.config_coord[6][1]-bull[1]))
            self.config_coord[6] = (bull[0],self.config_coord[6][1])
        self.x_start = self.config_coord[7][0]
        self.y_start = self.config_coord[4][1]
        self.x_factor = 2/(self.config_coord[5][0]-self.config_coord[7][0])
        self.y_factor = 2/(self.config_coord[6][1]-self.config_coord[4][1])

        while True:
            ret, frame = self.stream.read()
            if not ret:
                break
            
            frame_p = cv.warpPerspective(frame,self.matrix,(460,460))
            for (x,y) in self.config_coord[4:]:
                cv.circle(frame_p,(x,y), radius = 5, color = (0,0,255),thickness=3)
            cv.imshow("Callibrate sides",frame_p)

            if cv.waitKey(1) & 0xFF == ord('q'):
                break

    def DartLocation(self):
        #global x_factor
        #global y_factor
        #global x_start
        #global y_start

        #changes MOG2 to KNN, added dist2thershold
        sBackSub = cv.createBackgroundSubtractorKNN(dist2Threshold = 800)
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
            frame = cv.warpPerspective(frame,self.matrix,(460,460))
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
                    color = (0,0,255)
                    thickness = 1
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
                self.y = '{0:.3f}'.format(self.y)
                #print(self.x,self.y)
                text = f"X: +{self.x}+, Y: +{self.y}"
                org = (50, 50)   
                font = cv.FONT_HERSHEY_SIMPLEX  
                fontScale = 0.5 
                color = (0,0,255)   
                thickness = 1 
                
                cv.putText(lastFrame, text, org, font, fontScale, color, thickness, cv.LINE_AA)

                lastFrame = cv.warpPerspective(lastFrame,self.matrix,(460,460))
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
        
