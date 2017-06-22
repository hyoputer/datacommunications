import sys
import random
from node import Node
Transmission_rate = 10*10**6
Packet_size = 1024
Transmission_time = Packet_size/Transmission_rate * 10**6 #unit us

nodelist = []
noden = int(sys.argv[1])
total_transmit = 0
total_packet_delay = 0
total_collision = 0

for i in range(0, noden):
    nodelist.append(Node(float(sys.argv[2])))

for j in range(0, 10000):

    for i in range(0, noden):
        nodelist[i].start()

    trlist = []

    for i in range(0, noden):
        if nodelist[i].ifready() is True:
            trlist.append(i)

    if len(trlist) > 1:
        for i in trlist:
            nodelist[i].collision()
            total_collision+=1
    else:
        for i in trlist:
            total_packet_delay += nodelist[i].success()*Transmission_time
            total_transmit+=1

    for i in range(0, noden):
        nodelist[i].wait()


print('Throughput: {}'.format(total_transmit/(Transmission_time/(10**6)*10000)))
print('mean packet delay: {}us'.format(total_packet_delay/total_transmit))
print('Transmission collision prob: {}'.format(total_collision/(total_transmit+total_collision))) 
