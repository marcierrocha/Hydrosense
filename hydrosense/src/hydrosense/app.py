"""
Teste para o projecto de fim de curso.
"""

import toga
from toga.style.pack import Pack
from toga.style.pack import COLUMN, ROW


class HydroSense(toga.App):
    def startup(self):

        main_box = toga.Box(

                style=Pack(
                background_color="#FF0000",
                direction=COLUMN,
            )
        )



        name_label = toga.Label(
            "Your name: ",
            margin=(0, 5),
            direction=ROW
        )
        self.name_input = toga.TextInput(flex=1)

        name_box = toga.Box(direction=ROW, margin=5, )
        name_box.add(name_label)
        name_box.add(self.name_input)

        button = toga.Button(
            "Say Hello!",
            on_press=self.say_hello,
            margin=5,
            background_color='blue',
        )

        # CORRIGIDO: estas linhas agora estão bem indentadas
        main_box.add(name_box)
        main_box.add(button)

        self.main_window = toga.MainWindow(title=self.formal_name)
        self.main_window.content = main_box
        self.main_window.show()

    def say_hello(self, widget):
        print(f"Hello, {self.name_input.value}")


def main():
    return HydroSense()
