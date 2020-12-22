from pylab import *
def low_graph(x, results, greatest):
    for i in range(2, 9):
        semilogy(x, results[i], label = str(i) + " threads")
    xlabel("number of batches")
    ylabel("probability of thread blocking")
    legend(loc=3)
    grid(True, which='both')
    savefig("low_end_threading_model_low.pdf")
    figure()

    for i in range(9, 17):
        semilogy(x, results[i], label = str(i) + " threads")
    xlabel("number of batches")
    ylabel("probability of thread blocking")
    legend(loc=1)
    grid(True, which='both')
    savefig("low_end_threading_model_middle_low.pdf")
    figure()

    for i in range(18, 33, 2):
        semilogy(x, results[i], label = str(i) + " threads")
    xlabel("number of batches")
    ylabel("probability of thread blocking")
    legend(loc=3)
    grid(True, which='both')
    savefig("low_end_threading_model_middle_high.pdf")
    figure()

    for i in range(36, greatest + 1, 4):
        semilogy(x, results[i], label = str(i) + " threads")
    xlabel("number of batches")
    ylabel("probability of thread blocking")
    ylim([.1, 1])
    grid(True, which='both')
    legend(loc=3)
    savefig("low_end_threading_model_high.pdf")

    show()

def high_graph(x,results, greatest):
    for i in range(2, 9):
        semilogy(x, results[i], label = str(i) + " threads")
    xlabel("number of batches")
    ylabel("probability of thread blocking")
    legend(loc=1)
    grid(True, which='both')
    savefig("threading_model_low.pdf")
    figure()

    for i in range(9, 17):
        semilogy(x, results[i], label = str(i) + " threads")
    xlabel("number of batches")
    ylabel("probability of thread blocking")
    legend(loc=1)
    grid(True, which='both')
    savefig("threading_model_middle_low.pdf")
    figure()

    for i in range(18, 33, 2):
        semilogy(x, results[i], label = str(i) + " threads")
    xlabel("number of batches")
    ylabel("probability of thread blocking")
    legend(loc=1)
    grid(True, which='both')
    savefig("threading_model_middle_high.pdf")
    figure()

    for i in range(36, greatest + 1, 4):
        semilogy(x, results[i], label = str(i) + " threads")
    xlabel("number of batches")
    ylabel("probability of thread blocking")
    legend(loc=1)
    grid(True, which='both')
    savefig("threading_model_high.pdf")

    show()
