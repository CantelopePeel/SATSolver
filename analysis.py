import pandas as pd
import matplotlib.pyplot as plt


def analyze():
    df = pd.read_csv("./gen_run.csv", index_col=0)
    print("DPLL correct:\n", (df["bt_state"] == df["dpll_state"]).value_counts())
    print("CDCL correct:\n", (df["bt_state"] == df["cdcl_state"]).value_counts())
    print("Schoning correct:\n", (df["bt_state"] == df["scho_state"]).value_counts())

    means_df = df.groupby(["num_var"]).mean()
    means_dec_df = means_df.loc[:, means_df.columns.str.endswith("_dec")]
    means_time_df = means_df.loc[:, means_df.columns.str.endswith("_time")]
    print(means_df)

    plt.semilogy(means_dec_df)
    plt.legend(means_dec_df.columns)
    plt.show()

    plt.semilogy(means_time_df)
    plt.legend(means_time_df.columns)
    plt.show()
    plt.savefig('results.png')
    print(df)


if __name__ == "__main__":
    analyze()
