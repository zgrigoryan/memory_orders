#!/usr/bin/env python3
import pandas as pd
import matplotlib.pyplot as plt

def main():
    df = pd.read_csv("results.csv")
        df = df[df['Trial'] != "Average"]

    df['SeqCst(ns)'] = pd.to_numeric(df['SeqCst(ns)'])
    df['Relaxed(ns)'] = pd.to_numeric(df['Relaxed(ns)'])
    df['AcqRel(ns)'] = pd.to_numeric(df['AcqRel(ns)'])

    avg_seq = df['SeqCst(ns)'].mean()
    avg_relaxed = df['Relaxed(ns)'].mean()
    avg_acq_rel = df['AcqRel(ns)'].mean()

    # Bar chart: average times.
    labels = ['SeqCst', 'Relaxed', 'AcqRel']
    averages = [avg_seq, avg_relaxed, avg_acq_rel]

    plt.figure(figsize=(8, 6))
    plt.bar(labels, averages)
    plt.ylabel("Average Time (ns)")
    plt.title("Average Execution Time by Memory Order")
    plt.savefig("average_times.png")
    plt.show()

    # Line chart: execution time per trial.
    trials = df['Trial'].astype(int)
    plt.figure(figsize=(10, 6))
    plt.plot(trials, df['SeqCst(ns)'], marker='o', label='SeqCst')
    plt.plot(trials, df['Relaxed(ns)'], marker='o', label='Relaxed')
    plt.plot(trials, df['AcqRel(ns)'], marker='o', label='AcqRel')
    plt.xlabel("Trial")
    plt.ylabel("Execution Time (ns)")
    plt.title("Execution Time per Trial")
    plt.legend()
    plt.savefig("trial_times.png")
    plt.show()

if __name__ == "__main__":
    main()
