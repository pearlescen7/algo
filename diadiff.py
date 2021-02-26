#!/bin/python3

import math
import os
import random
import re
import sys
import itertools

# Complete the diagonalDifference function below.
def diagonalDifference(arr):
    
    i, j = 0, 0
    k = (n-1)
    pri_sum , sec_sum = 0, 0

    while(j < (n)):
        pri_sum += arr[i]
        i += (n+1)
        sec_sum += arr[k]
        k += (n-1)
        j += 1
    res = abs(pri_sum - sec_sum)
    return res
    
if __name__ == '__main__':


    n = int(input())

    arr = []

    for _ in range(n):
        arr.append(list(map(int, input().rstrip().split())))

    arr = list(itertools.chain(*arr))
    print(arr)

    result = diagonalDifference(arr)
    print(result)