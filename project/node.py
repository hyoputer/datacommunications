import random
BACKOFF_MAX = 10

class Node:
    def __init__(self, rate):
        self.rate = rate
        self.bocnt = 0
        self.retrans = 0
        self.readytosend = False
        self.delay = 0

    def start(self):
        if self.bocnt > 0:
            self.readytosend = False
        else:
            if self.retrans > 0:
                self.readytosend = True
            elif random.random() < self.rate:
                self.readytosend = True
    
    def collision(self):
        self.retrans+=1
        if self.retrans < 10:
            self.bocnt = random.randrange(1, 2 ** self.retrans + 1)
        else:
            self.bocnt = random.randrange(1, 2 ** BACKOFF_MAX + 1)

    def success(self):
        self.readytosend = False
        self.bocnt = 0
        self.retrans = 0
        tmp = self.delay
        self.delay = 0
        return tmp

    def wait(self):
        if self.bocnt > 0:
            self.bocnt-=1
            self.delay+=1

    def ifready(self):
        return self.readytosend
