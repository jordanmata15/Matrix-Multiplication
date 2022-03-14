import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import FormatStrFormatter
import math
import os
import sys

path_to_script = os.path.dirname(__file__)

data_path = os.path.join(path_to_script, "../data") # data folder relative to this script
data_filename = os.path.join(data_path, "times.csv")


def crunch_data(filename):
    # num_threads, num_random_points, pi_approximation, time_elapsed
    df = pd.read_csv(filename)
    
    # create our plots
    plot_threads_to_time(df[['num_threads','square_sizes', 'time_elapsed']])


"""
Plot number of threads to the time elapsed. One line for each data size. 
"""
def plot_threads_to_time(times_df):
    sizes = times_df["square_sizes"].unique()

    # format the plot
    fig, axs = plt.subplots(math.ceil(len(sizes)/2),2, constrained_layout=True)
    
    for ax in axs.flat:
        ax.set(xlabel='Number of Threads', ylabel='Time (seconds)')
    
    for ax,size in zip(axs.flat, sizes):
        local_df = times_df[times_df["square_sizes"] == size]
        x = local_df['num_threads']
        x_labels=[str(i) for i in x]
        y = local_df['time_elapsed']
        ax.set_title("Matrix size n: " + str(size) +"^2", color="red")
        ax.plot(x_labels, y)

    fig.set_size_inches(18.5, 10.5)
    fig.savefig(os.path.join(data_path, "Plot_Threads_Time.pdf"), bbox_inches = "tight")


if __name__=="__main__":
    if len(sys.argv) > 1:
        data_filename = sys.argv[1]

    file_full_path = os.path.join(path_to_script, data_filename)

    crunch_data(file_full_path)