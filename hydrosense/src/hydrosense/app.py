"""
Teste para o projecto de fim de curso.
"""

import toga
from toga.style.pack import Pack
from toga.style.pack import COLUMN, ROW


class HydroSense(toga.App):
    def startup(self):

        main_box = toga.Box(


        )

        webview = toga.WebView(style= Pack(flex=1))

        main_box.add(webview)
        webview.url = "http://192.168.0.90:8080"






        self.main_window = toga.MainWindow(title=self.formal_name)

        self.main_window.content = main_box
        self.main_window.show()




def main():
    return HydroSense()
