import sys
import PySide2.QtWidgets as widg
#import PySide2.QtCore as core
from main_ui import MainUI
import threading
import socket


class MainView(widg.QMainWindow, MainUI):
    def __init__(self):
        super(MainView, self).__init__()
        self.server = 'chat.freenode.net'
        self.port = 6666
        self.nickname = 'client_test3141'  # Must be unique to server network
        self.connection = None
        self.response_thread = None
        self.channel_message_list = []
        self.private_message_list = []

        self.connect_to_IRC_server()
        self.start_response_handler_thread()
        self.setupUi(self)
        self.model = self.chat_listview.model()
        self.link_ui()

    def connect_to_IRC_server(self):
        self.connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.connection.connect((self.server, self.port))
        self.send_command('NICK', self.nickname)
        self.send_command('USER', '{} * * :{}'.format(self.nickname, self.nickname))

    def start_response_handler_thread(self):
        self.response_thread = threading.Thread(target=self.handle_responses)
        self.response_thread.daemon = True
        self.response_thread.start()

    def handle_responses(self):
        while True:
            # Decoding some server messages can throw errors
            try:
                # Blocks running process while listening to socket
                server_response = self.connection.recv(512).decode('utf-8')

                if server_response:
                    ping_test = server_response.strip().split()

                    if ping_test[0] == "PING":
                        self.send_command("PONG", ping_test[1])

                    messages = server_response.strip().split('\r\n')
                    self.chat_listview.addItems(messages)

            except Exception:
                pass

    def send_command(self, command, message):
        command = '{} {}\r\n'.format(command, message).encode('utf-8')
        self.connection.send(command)

    def link_ui(self):
        self.submit_button.clicked.connect(self.submit_pushed)
        self.message_textedit.returnPressed.connect(self.submit_pushed)
        self.model.rowsInserted.connect(self.chat_listview.scrollToBottom)

    def submit_pushed(self):
        self.message = self.message_textedit.text()
        self.message_textedit.clear()
        self.message_textedit.update()
        self.send_command('PRIVMSG {}'.format(self.nickname), ':' + self.message)


app = widg.QApplication([])
client = MainView()
client.show()
sys.exit(app.exec_())
