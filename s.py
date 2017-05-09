#!/usr/bin/python

import sys
from sys import argv

stock_listings = argv[1]

""" get a list of stock names """
def getStockNames():

    stock_names = []

    with open(stock_listings, 'r') as f:
        for line in f:
            TimeStamp, Symbol, Quantity, Price = line.strip().split(',')
            stock_names.append(Symbol)

    """ we want a set of unique stock names, so convert our
        list of names to that set of unique listings. """
    stock_set = set(stock_names)

    return stock_set


# getStockDetails() - given a stock listing, return the following:
#
#     Max Time Gap
#     Total Volume
#     Max Trade Price
#     Weighted Average
#
def getStockDetails(stock):

    timestamps = []
    volume     = []
    price      = []

    with open(stock_listings, 'r') as f:
        for line in f:
            if stock in line:
                TimeStamp, Symbol, Quantity, Price = line.strip().split(',')

                timestamps.append(TimeStamp)
                volume.append(Quantity)
                price.append(Price)

 
    """ convert strings to ints so we can process the list of timestamps. """
    timestamps_list = map(int, timestamps)

    gaps = [y-x for x,y in zip(timestamps_list, timestamps_list[1:])]

    volume_list = map(int, volume)
    
    price_list = map(int, price) 

    weighted_avg = sum(x*y for x,y in zip(volume_list, price_list)) / sum(volume_list)
    
    # return:
    # maxTimeGap, totalVolume, maxTradePrice, weightedAverage
    return max(gaps), sum(volume_list), max(price_list), weighted_avg


for stock in getStockNames():

    maxTimeGap, totalVolume, maxTradePrice, weightedAverage = getStockDetails(stock)
    print stock + "," + str(maxTimeGap) + "," + str(totalVolume) + "," + str(weightedAverage) + "," + str(maxTradePrice)
