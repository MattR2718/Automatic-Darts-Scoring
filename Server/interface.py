from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("ADS - Server")
        self.setGeometry(100,100,1000,800)



 

if __name__ == "__main__":
    app = QApplication([])

    # Initialize main window and view
    view = MainWindow()

    view.show()

    app.exec_()