import batch_thread_graph as btg
import sys
if __name__ == '__main__':
    if len(sys.argv) >= 2:
        greatest = int(sys.argv[1])
    else:
        greatest = 64
    f = open('data.txt', 'r')
    low_x = []
    high_x = []
    low_results = {}
    high_results = {}
    #build low_x and high_x
    row = f.readline().split()
    for i in range(len(row)):
        if (i < 48):
            low_x.append(int(row[i]))
        else:
            high_x.append(int(row[i]))
    #build low_results and high_results
    for line in f:
        row = line.split()
        #indexes 1 to 48 correspond to low_results
        #indexes 49 to end correspond to high_results
        low_results[int(row[0])] = []
        high_results[int(row[0])] = []
        for i in range(1, len(row)):
            if ( i < 49):
                low_results[int(row[0])].append(float(row[i]))
            else:
                high_results[int(row[0])].append(float(row[i]))
    btg.low_graph(low_x, low_results, greatest)
    btg.high_graph(high_x, high_results, greatest)


