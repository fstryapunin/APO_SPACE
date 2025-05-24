import random

PLANET_COUNT = 100
MAX_RADIUS = 10000
MIN_RADIUS = 1000

COORD_LIMIT = 100000


class Planet:
    def __init__(self, x, y, radius):
        self.x = x
        self.y = y
        self.radius = radius
    
    def __str__(self):
        return "{{{{{}, {}}}, {}, false, false}}".format(self.x, self.y, self.radius)
    
if __name__ == "__main__":
    for _ in range(PLANET_COUNT):
        print(Planet(random.randint(-COORD_LIMIT, COORD_LIMIT), random.randint(-COORD_LIMIT, COORD_LIMIT), random.randint(MIN_RADIUS, MAX_RADIUS)))