#!/usr/bin/env python3
import sys
from pathlib import Path
import yfinance as yf
import pandas as pd
import argparse

class DataFetcher:

    def __init__(self, ticker: str, timeframe: str):
        self.ticker = ticker
        self.timeframe = timeframe

    
    def fetch_data(self): 
        """
        Fetches historical data using yfinance API.

        Returns
        -------
        data : pd.Dataframe
            A dataframe with all available OHLC bars for
        the given ticker and timeframe.
        """

        data = yf.download(
            tickers=self.ticker, 
            #start=start_date, 
            #end=end_date,
            period='max',
            interval=self.timeframe,
            progress=False
        )

        if isinstance(data.columns, pd.MultiIndex):
            data.columns = data.columns.droplevel(1)

        return data


    def save_df_csv(self, df: pd.DataFrame):
        """
        Saves a dataframe for a given ticker and timeframe
        in the data folder as a csv.

        Prameters
        ---------
        df : pd.Dataframe
            A dataframe with all availabale OHLC bars
        for the given ticker and timeframe.
        """

        base_dir = Path("data") / self.ticker
        base_dir.mkdir(parents=True, exist_ok=True)

        file_path = base_dir / f"{self.timeframe}.csv"
        df.to_csv(file_path)

    
"""
def main():
    print("fetch_yf.py: hello from python!")
    print("argv:", sys.argv)

    # Create data directory if it doesn't exist
    data_dir = Path("data")
    data_dir.mkdir(parents=True, exist_ok=True)

    ticker = 'TSLA'

    end_Date = datetime.today()

    print(end_Date)

    # Write a small proof file so C++ can verify execution
    out_file = data_dir / "python_was_here.txt"
    out_file.write_text("Python script executed successfully.\n")

    print(f"Wrote file: {out_file.resolve()}")
"""

def main():

    parser = argparse.ArgumentParser()
    parser.add_argument("--ticker", required=True)
    parser.add_argument("--timeframe", required=True)
    args = parser.parse_args()

    data_dir = Path("data")
    data_dir.mkdir(parents=True, exist_ok=True)

    ticker = args.ticker
    timeframe = args.timeframe

    data_fetcher = DataFetcher(ticker, timeframe)
    data = data_fetcher.fetch_data()
    data_fetcher.save_df_csv(data)

if __name__ == "__main__":
    main()



