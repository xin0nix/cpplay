from typing import List

class Solution:
    def trap(self, height: List[int]) -> int:
        # height: List[int]
        # Найдём maxLeft
        size = len(height)
        # Посчитать максимальные элементы слева
        maxLeft = [0] * size
        for i in range(1, size):
            maxLeft[i] = max(height[i-1], maxLeft[i-1])
        # Посчитать максимальные элементы справа
        maxRight = [0] * size
        for i in range(size-2, -1, -1):
            maxRight[i] = max(height[i+1], maxRight[i+1])
        totalWater = 0
        # Сложить кол-во воды по формуле max(0, min(maxL, maxR)-h)
        for [left, right, h] in zip(maxLeft, maxRight, height):
            diff = min(left, right) - h
            water = max(0, diff)
            totalWater += water
        return totalWater