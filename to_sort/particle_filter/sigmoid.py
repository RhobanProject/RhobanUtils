from math import *
nbSteps = 10
l = 20
offset = 0.5
for t in range(0, nbSteps):
    x = t / (nbSteps + 0.0);
    sigm = 1 / (1 + exp(- l *(x - offset)))
    print x, x + (1 - x) * sigm
